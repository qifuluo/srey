#include "http.h"
#include "sarray.h"
#include "loger.h"

typedef enum parse_status{
    INIT = 0,
    CONTENT,
    CHUNKED
}parse_status;
ARRAY_DECL(http_header_ctx, arr_header);
typedef struct http_pack_ctx {
    char *first;
    char *hdata;
    char *data;
    int32_t chunked;
    size_t flens;
    size_t hlens;
    size_t lens;
    arr_header header;
}http_pack_ctx;
typedef struct http_parser_ctx{
    parse_status status;
    http_pack_ctx *pack;
}http_parser_ctx;
#define MAX_HEADLENS ONEK * 4
#define FLAG_CRLF "\r\n"
#define FLAG_HEND "\r\n\r\n"
#define FLAG_CONTENT "content-length"
#define FLAG_CHUNKED "transfer-encoding"
#define CHUNKED_KEY "chunked"
#define RTN_PACK() http_pack_ctx *_pack = parser->pack; parser->pack = NULL

static inline char *_skip_empty(char *data) {
    while (' ' == *data) {
        data++;
    };
    return data;
}
static inline void _check_fileld(http_parser_ctx *parser, http_header_ctx *field) {
    switch (tolower(*(field->key))) {
    case 'c':
        if (0 == _memicmp(field->key, FLAG_CONTENT, strlen(FLAG_CONTENT))) {
            if (CHUNKED != parser->status) {
                parser->status = CONTENT;
                parser->pack->lens = atoi(field->value);
            }
        }
        break;
    case 't':
        if (0 == _memicmp(field->key, FLAG_CHUNKED, strlen(FLAG_CHUNKED))
            && 0 == _memicmp(field->value, CHUNKED_KEY, strlen(CHUNKED_KEY))) {
            parser->status = CHUNKED;
            parser->pack->lens = 0;
            parser->pack->chunked = 1;
        }
        break;
    default:
        break;
    }
}
static inline int32_t _http_parse_head(http_parser_ctx *parser) {
    char *head = parser->pack->hdata;
    size_t flens = strlen(FLAG_CRLF);
    char *pos = strstr(head, FLAG_CRLF);
    if (NULL == pos) {
        return ERR_FAILED;
    }
    parser->pack->first = head;
    parser->pack->flens = pos - head;

    head = pos + flens;
    http_header_ctx field;
    while (1) {
        pos = strstr(head, ":");
        if (NULL == pos) {
            break;
        }
        head = _skip_empty(head);
        field.key = head;
        field.klen = pos - head;
        head = pos + 1;
        head = _skip_empty(head);

        pos = strstr(head, FLAG_CRLF);
        if (NULL == pos) {
            return ERR_FAILED;
        }
        field.value = head;
        field.vlen = pos - head;
        head = pos + flens;
        _check_fileld(parser, &field);
        arr_header_push_back(&parser->pack->header, &field);
    }
    return ERR_OK;
}
static inline void *_http_content(buffer_ctx *buf, ud_cxt *ud, int32_t *closefd) {
    http_parser_ctx *parser = ud->extra;
    if (buffer_size(buf) >= parser->pack->lens) {
        MALLOC(parser->pack->data, parser->pack->lens);
        ASSERTAB(parser->pack->lens == buffer_remove(buf, parser->pack->data, parser->pack->lens), "copye buffer failed.");
        parser->status = INIT;
        RTN_PACK();
        return _pack;
    } else {
        return NULL;
    }
}
static inline size_t _http_headlens(buffer_ctx *buf, int32_t *closefd) {
    size_t flens = strlen(FLAG_HEND);
    int32_t pos = buffer_search(buf, 0, 0, 0, FLAG_HEND, flens);
    if (ERR_FAILED == pos) {
        if (buffer_size(buf) >= MAX_HEADLENS) {
            *closefd = 1;
            LOG_WARN("%s", "http head too long.");
        }
        return 0;
    }
    return pos + flens;
}
static inline http_pack_ctx *_http_headpack(size_t lens) {
    char *pack;
    CALLOC(pack, 1, sizeof(http_pack_ctx) + lens);
    ((http_pack_ctx *)pack)->hdata = pack + sizeof(http_pack_ctx);
    ((http_pack_ctx *)pack)->hlens = lens;
    arr_header_init(&((http_pack_ctx *)pack)->header, ARRAY_INIT_SIZE);
    return (http_pack_ctx *)pack;
}
static inline void *_http_header(buffer_ctx *buf, ud_cxt *ud, int32_t *closefd) {
    size_t size = _http_headlens(buf, closefd);
    if (0 == size) {
        return NULL;
    }
    http_parser_ctx *parser;
    if (NULL == ud->extra) {
        CALLOC(parser, 1, sizeof(http_parser_ctx));
        ud->extra = parser;
    } else {
        parser = ud->extra;
    }
    parser->pack = _http_headpack(size);
    ASSERTAB(size == buffer_remove(buf, parser->pack->hdata, size), "copye buffer failed.");
    if (ERR_OK != _http_parse_head(parser)) {
        *closefd = 1;
        return NULL;
    }
    if (CONTENT == parser->status) {
        return _http_content(buf, ud, closefd);
    } else {
        RTN_PACK();
        return _pack;
    }
}
static inline http_pack_ctx *_http_chunkedpack(size_t lens) {
    char *pack;
    CALLOC(pack, 1, sizeof(http_pack_ctx) + lens);
    if (lens > 0) {
        ((http_pack_ctx *)pack)->data = pack + sizeof(http_pack_ctx);
        ((http_pack_ctx *)pack)->lens = lens;
    }
    ((http_pack_ctx *)pack)->chunked = 1;
    return (http_pack_ctx *)pack;
}
static inline void *_http_chunked(buffer_ctx *buf, ud_cxt *ud, int32_t *closefd) {
    size_t drain;
    size_t flens = strlen(FLAG_CRLF);
    http_parser_ctx *parser = ud->extra;
    if (NULL == parser->pack) {
        int32_t keypos = buffer_search(buf, 0, 0, 0, FLAG_CRLF, flens);
        if (ERR_FAILED == keypos) {
            return NULL;
        }
        char buflens[64] = { 0 };
        ASSERTAB(keypos == buffer_copyout(buf, 0, buflens, keypos), "copye buffer failed.");
        drain = keypos + flens;
        ASSERTAB(drain == buffer_drain(buf, drain), "drain buffer failed.");
        parser->pack = _http_chunkedpack(atoi(buflens));
    }
    drain = parser->pack->lens + flens;
    if (buffer_size(buf) < drain) {
        return NULL;
    }
    if (parser->pack->lens > 0) {
        ASSERTAB(parser->pack->lens == 
            buffer_copyout(buf, 0, parser->pack->data, parser->pack->lens), "copye buffer failed.");
    } else {
        parser->status = INIT;
    }
    ASSERTAB(drain == buffer_drain(buf, drain), "drain buffer failed.");
    RTN_PACK();
    return _pack;
}
void http_pkfree(void *data) {
    if (NULL == data) {
        return;
    }
    http_pack_ctx *pack = data;
    if (NULL != pack->hdata) {
        FREE(pack->data);
        arr_header_free(&pack->header);
    }
    FREE(pack);
}
void http_udfree(ud_cxt *ud) {
    if (NULL == ud->extra) {
        return;
    }
    http_pkfree(((http_parser_ctx *)ud->extra)->pack);
    FREE(ud->extra);
}
void *http_unpack(buffer_ctx *buf, size_t *size, ud_cxt *ud, int32_t *closefd) {
    parse_status status;
    if (NULL == ud->extra) {
        status = INIT;
    } else {
        status = ((http_parser_ctx *)ud->extra)->status;
    }
    void *data;
    switch (status) {
    case INIT:
        data = _http_header(buf, ud, closefd);
        break;
    case CONTENT:
        data = _http_content(buf, ud, closefd);
        break;
    case CHUNKED:
        data = _http_chunked(buf, ud, closefd);
        break;
    default:
        data = NULL;
        *closefd = 1;
        break;
    }
    return data;
}
const char *http_method(void *data, size_t *lens) {
    http_pack_ctx *pack = data;
    if (NULL == pack->hdata) {
        return NULL;
    }
    *lens = pack->flens;
    return pack->first;
}
size_t http_nheader(void *data) {
    return arr_header_size(&((http_pack_ctx *)data)->header);
}
http_header_ctx *http_header_at(void *data, size_t pos) {
    return arr_header_at(&((http_pack_ctx *)data)->header, pos);
}
const char *http_header(void *data, const char *header, size_t *lens) {
    http_pack_ctx *pack = data;
    if (NULL == pack->hdata) {
        return NULL;
    }
    http_header_ctx *filed;
    for (size_t i = 0; i < arr_header_size(&pack->header); i++) {
        filed = arr_header_at(&pack->header, i);
        if (0 == _memicmp(filed->key, header, strlen(header))) {
            *lens = filed->vlen;
            return filed->value;
        }
    }
    return NULL;
}
int32_t http_chunked(void *data) {
    return ((http_pack_ctx *)data)->chunked;
}
void *http_data(void *data, size_t *lens) {
    http_pack_ctx *pack = data;
    *lens = pack->lens;
    return pack->data;
}
