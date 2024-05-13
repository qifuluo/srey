#include "tasks/harbor.h"

#define HARBOR_KEY_LENS 128
typedef struct harbor_args {
    SOCKET fd;
    uint64_t sess;
    uint64_t skid;
    uint64_t timeout;
}harbor_args;
typedef struct harbor_ctx {
    uint16_t port;
    int32_t timeout;
    struct hashmap *mapargs;
    struct evssl_ctx *ssl;
    uint64_t lsnid;
    hmac_sha256_ctx macsha256;
    char ip[IP_LENS];
    char signkey[HARBOR_KEY_LENS + 1];
}harbor_ctx;

static uint64_t _map_args_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    return hash((const char *)&(((harbor_args *)item)->sess), sizeof(((harbor_args *)item)->sess));
}
static int _map_args_compare(const void *a, const void *b, void *ud) {
    return (int)(((harbor_args *)a)->sess - ((harbor_args *)b)->sess);
}
static void _map_args_set(task_ctx *harbor, uint64_t sess, SOCKET fd, uint64_t skid) {
    harbor_ctx *hbctx = harbor->arg;
    harbor_args arg;
    arg.sess = sess;
    arg.fd = fd;
    arg.skid = skid;
    arg.timeout = nowms() + hbctx->timeout;
    hashmap_set(hbctx->mapargs, &arg);
}
static int32_t _map_args_get(task_ctx *harbor, uint64_t sess, harbor_args *args) {
    harbor_ctx *hbctx = harbor->arg;
    harbor_args key;
    key.sess = sess;
    harbor_args *tmp = (harbor_args *)hashmap_get(hbctx->mapargs, &key);
    if (NULL == tmp) {
        return ERR_FAILED;
    }
    *args = *tmp;
    hashmap_delete(hbctx->mapargs, &key);
    return  ERR_OK;
}
static void _harbor_free(void *arg) {
    harbor_ctx *hbctx = arg;
    hashmap_free(hbctx->mapargs);
    FREE(hbctx);
}
static void _harbor_response(task_ctx *harbor, SOCKET fd, uint64_t skid, char *respdata, size_t lens, int32_t code) {
    buffer_ctx buffer;
    buffer_init(&buffer);
    http_pack_resp(&buffer, code);
    http_pack_head(&buffer, "Server", "Srey");
    if (NULL != respdata) {
        http_pack_head(&buffer, "Content-Type", "application/octet-stream");
        http_pack_content(&buffer, respdata, lens);
    } else {
        http_pack_head(&buffer, "Content-Type", "text/plain");
        const char *erro = http_error(code);
        http_pack_content(&buffer, (void *)erro, strlen(erro));
    }
    char *data;
    size_t size = buffer_size(&buffer);
    MALLOC(data, size);
    buffer_copyout(&buffer, 0, data, size);
    ev_send(&harbor->scheduler->netev, fd, skid, data, size, 0);
    buffer_free(&buffer);
}
static int32_t _check_sign(harbor_ctx *hbctx, struct http_pack_ctx *pack, buf_ctx *url, char *reqdata, size_t reqlens) {
    size_t klens = strlen(hbctx->signkey);
    if (0 == klens) {
        return ERR_OK;
    }
    size_t tlens = 0;
    char *tbuf = http_header(pack, "X-Timestamp", &tlens);
    if (NULL == tbuf
        || 0 == tlens) {
        LOG_WARN("not find X-Timestamp.");
        return ERR_FAILED;
    }
    size_t slens = 0;
    char *sign = http_header(pack, "Authorization", &slens);
    if (NULL == sign
        || 0 == slens) {
        LOG_WARN("not find Authorization.");
        return ERR_FAILED;
    }
    uint64_t tms = (uint64_t)atoll(tbuf);
    uint64_t tnow = nowsec();
    int32_t diff;
    if (tnow >= tms) {
        diff = (int32_t)(tnow - tms);
    } else {
        diff = (int32_t)(tms - tnow);
    }
    if (diff >= 5 * 60) {
        LOG_WARN("timestamp error.");
        return ERR_FAILED;
    }
    char *signstr;
    size_t total = url->lens + reqlens + tlens;
    MALLOC(signstr, total);
    memcpy(signstr, url->data, url->lens);
    if (0 != reqlens) {
        memcpy(signstr + url->lens, reqdata, reqlens);
    }
    memcpy(signstr + url->lens + reqlens, tbuf, tlens);
    unsigned char hs[SHA256_BLOCK_SIZE];
    char hexhs[HEX_ENSIZE(sizeof(hs))];
    hmac_sha256_init(&hbctx->macsha256);
    hmac_sha256_update(&hbctx->macsha256, (unsigned char *)signstr, total);
    hmac_sha256_final(&hbctx->macsha256, hs);
    tohex(hs, sizeof(hs), hexhs);
    FREE(signstr);
    if (strlen(hexhs) != slens
        || 0 != _memicmp(sign, hexhs, slens)) {
        LOG_WARN("check sign failed.");
        return ERR_FAILED;
    }
    return ERR_OK;
}
static void _harbor_net_recv(task_ctx *harbor, SOCKET fd, uint64_t skid, uint8_t pktype,
    uint64_t sess, uint8_t client, uint8_t slice, void *data, size_t size) {
    size_t lens;
    harbor_ctx *hbctx = harbor->arg;
    char *reqdata = http_data(data, &lens);
    if (NULL == reqdata
        || 0 == lens
        || 0 != http_chunked(data)) {
        ev_close(&harbor->scheduler->netev, fd, skid);
        return;
    }
    buf_ctx *hstatus = http_status(data);
    if (!buf_icompare(&hstatus[0], "post", strlen("post"))) {
        ev_close(&harbor->scheduler->netev, fd, skid);
        return;
    }
    url_ctx url;
    url_parse(&url, hstatus[1].data, hstatus[1].lens);
    if (buf_empty(&url.path)) {
        ev_close(&harbor->scheduler->netev, fd, skid);
        return;
    }
    if (ERR_OK != _check_sign(hbctx, data, &hstatus[1], reqdata, lens)) {
        ev_close(&harbor->scheduler->netev, fd, skid);
        return;
    }
    buf_ctx *bdst = url_get_param(&url, "dst");
    if (buf_empty(bdst)) {
        ev_close(&harbor->scheduler->netev, fd, skid);
        return;
    }
    buf_ctx *reqtype = url_get_param(&url, "type");
    if (buf_empty(reqtype)) {
        ev_close(&harbor->scheduler->netev, fd, skid);
        return;
    }
    name_t dst = (name_t)strtol(bdst->data, NULL, 10);
    uint8_t type = (uint8_t)strtol(reqtype->data, NULL, 10);
    if (buf_icompare(&url.path, "call", strlen("call"))) {
        task_ctx *to = task_grab(harbor->scheduler, dst);
        if (NULL == to){
            _harbor_response(harbor, fd, skid, NULL, 0, 404);
            return;
        }
        trigger_call(to, type, reqdata, lens, 1);
        task_ungrab(to);
        _harbor_response(harbor, fd, skid, NULL, 0, 200);
    } else if (buf_icompare(&url.path, "request", strlen("request"))) {
        task_ctx *to = task_grab(harbor->scheduler, dst);
        if (NULL == to) {
            _harbor_response(harbor, fd, skid, NULL, 0, 404);
            return;
        }
        uint64_t sess = createid();
        _map_args_set(harbor, sess, fd, skid);
        trigger_request(to, harbor, type, sess, (void *)reqdata, lens, 1);
        task_ungrab(to);
    } else {
        ev_close(&harbor->scheduler->netev, fd, skid);
    }
}
static void _harbor_onresponse(task_ctx *harbor, uint64_t sess, int32_t error, void *data, size_t size) {
    harbor_args arg;
    if (ERR_OK != _map_args_get(harbor, sess, &arg)) {
        return;
    }
    if (ERR_OK == error) {
        _harbor_response(harbor, arg.fd, arg.skid, data, size, 200);
    } else {
        _harbor_response(harbor, arg.fd, arg.skid, NULL, 0, 400);
    }
}
static void _harbor_timeout(task_ctx *harbor, uint64_t sess) {
    size_t iter = 0;
    uint64_t now = nowms();
    harbor_args *arg;
    harbor_ctx *hbctx = harbor->arg;
    while (hashmap_iter(hbctx->mapargs, &iter, (void **)&arg)) {
        if (now > arg->timeout) {
            _harbor_response(harbor, arg->fd, arg->skid, NULL, 0, 408);
            hashmap_delete(hbctx->mapargs, arg);
        }
    }
    trigger_timeout(harbor, 0, 100, _harbor_timeout);
}
static void _harbor_startup(task_ctx *harbor) {
    harbor_ctx *hbctx = harbor->arg;
    on_recved(harbor, _harbor_net_recv);
    on_responsed(harbor, _harbor_onresponse);
    if (hbctx->timeout > 0) {
        trigger_timeout(harbor, 0, 100, _harbor_timeout);
    }
    if (ERR_OK != trigger_listen(harbor, PACK_HTTP, hbctx->ssl, hbctx->ip, hbctx->port, &hbctx->lsnid, 0)) {
        LOG_ERROR("trigger_listen %s:%d error", hbctx->ip, hbctx->port);
    }
}
static void _harbor_closing(task_ctx *harbor) {
    harbor_ctx *hbctx = harbor->arg;
    if (0 != hbctx->lsnid) {
        ev_unlisten(&harbor->scheduler->netev, hbctx->lsnid);
        hbctx->lsnid = 0;
    }
}
int32_t harbor_start(scheduler_ctx *scheduler, name_t tname, name_t ssl,
    const char *host, uint16_t port, const char *key, int32_t ms) {
    if (INVALID_TNAME == tname) {
        return ERR_OK;
    }
    size_t klens = strlen(key);
    ASSERTAB(klens <= HARBOR_KEY_LENS, "harbor key too long.");
    harbor_ctx *hbctx;
    CALLOC(hbctx, 1, sizeof(harbor_ctx));
    if (klens > 0) {
        memcpy(hbctx->signkey, key, klens);
        hmac_sha256_key(&hbctx->macsha256, (unsigned char *)hbctx->signkey, klens);
    }
    strcpy(hbctx->ip, host);
    hbctx->port = port;
    hbctx->timeout = ms;
#if WITH_SSL
    hbctx->ssl = srey_ssl_qury(scheduler, ssl);
#else
    hbctx->ssl = NULL;
#endif
    hbctx->mapargs = hashmap_new_with_allocator(_malloc, _realloc, _free,
        sizeof(harbor_args), ONEK, 0, 0, _map_args_hash, _map_args_compare, NULL, NULL);
    task_ctx *harbor = task_new(scheduler, tname, NULL, _harbor_free, (void *)hbctx);
    if (ERR_OK != task_register(harbor, _harbor_startup, _harbor_closing)) {
        task_free(harbor);
        return ERR_FAILED;
    }
    return ERR_OK;
}
static void _harbor_sign(buffer_ctx *buf, const char *key, const char *url, void *data, size_t size) {
    size_t klens = strlen(key);
    if (0 == klens) {
        return;
    }
    char tms[64] = {0};
    SNPRINTF(tms, sizeof(tms) - 1, "%"PRIu64, nowsec());
    size_t ulens = strlen(url);
    size_t tslens = strlen(tms);
    size_t lens = ulens + size + tslens;
    char *sbuf;
    MALLOC(sbuf, lens);
    memcpy(sbuf, url, ulens);
    if (NULL != data) {
        memcpy(sbuf + ulens, data, size);
    }
    memcpy(sbuf + ulens + size, tms, tslens);
    hmac_sha256_ctx macsha256;
    unsigned char hs[SHA256_BLOCK_SIZE];
    char hexhs[HEX_ENSIZE(sizeof(hs))];
    hmac_sha256_key(&macsha256, (unsigned char *)key, klens);
    hmac_sha256_init(&macsha256);
    hmac_sha256_update(&macsha256, (unsigned char *)sbuf, lens);
    hmac_sha256_final(&macsha256, hs);
    tohex(hs, sizeof(hs), hexhs);
    FREE(sbuf);
    http_pack_head(buf, "X-Timestamp", tms);
    http_pack_head(buf, "Authorization", hexhs);
}
void *harbor_pack(name_t task, int32_t call, uint8_t reqtype, const char *key, void *data, size_t size, size_t *lens) {
    char url[128] = {0};
    if (0 != call) {
        SNPRINTF(url, sizeof(url) - 1, "/call?dst=%d&type=%d", task, reqtype);
    } else {
        SNPRINTF(url, sizeof(url) - 1, "/request?dst=%d&type=%d", task, reqtype);
    }
    buffer_ctx buf;
    buffer_init(&buf);
    http_pack_req(&buf, "POST", url);
    http_pack_head(&buf, "Connection", "Keep-Alive");
    http_pack_head(&buf, "Content-Type", "application/octet-stream");
    _harbor_sign(&buf, key, url, data, size);
    http_pack_content(&buf, data, size);
    char *hpack;
    *lens = buffer_size(&buf);
    MALLOC(hpack, *lens);
    buffer_copyout(&buf, 0, hpack, *lens);
    buffer_free(&buf);
    return hpack;
}
