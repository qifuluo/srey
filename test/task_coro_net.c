#include "task_coro_net.h"

#if WITH_CORO

static int32_t _prt = 1;

static void _test_syn_send(task_ctx *task) {
    uint64_t skid;
    SOCKET fd = coro_connect(task, PACK_CUSTZ, NULL, "127.0.0.1", 15000, &skid, 0);
    if (INVALID_SOCK == fd) {
        LOG_ERROR("%s", "syn_connect error");
        return;
    }
    const char *msg = "this is tcp task_coro_net.";
    size_t size;
    struct custz_pack_ctx *pack = custz_pack((void*)msg, strlen(msg), &size);
    void *data = coro_send(task, fd, skid, pack, size, &size, 0);
    if (NULL == data) {
        LOG_ERROR("%s", "syn_send error");
        ev_close(&task->scheduler->netev, fd, skid);
        return;
    }
    data = custz_data(data, &size);
    ASSERTAB(0 == _memicmp(data, msg, strlen(msg)), "syn_send error");
    ev_close(&task->scheduler->netev, fd, skid);
    if (_prt) {
        LOG_INFO("_test_syn_send ok.");
    }
}
static void _test_syn_ssl1_send(task_ctx *task) {
#if WITH_SSL
    uint64_t skid;
    SOCKET fd = coro_connect(task, PACK_CUSTZ, NULL, "127.0.0.1", 15001, &skid, NETEV_AUTHSSL);
    if (INVALID_SOCK == fd) {
        LOG_ERROR("%s", "syn_connect error");
        return;
    }
    struct evssl_ctx *ssl = srey_ssl_qury(task->scheduler, 101);
    if (ERR_OK != coro_auth_ssl(task, fd, skid, 1, ssl)) {
        LOG_ERROR("%s", "coro_auth_ssl error");
        return;
    }
    const char *msg = "this is tcp task_coro_net.";
    size_t size;
    struct custz_pack_ctx *pack = custz_pack((void*)msg, strlen(msg), &size);
    void *data = coro_send(task, fd, skid, pack, size, &size, 0);
    if (NULL == data) {
        LOG_ERROR("%s", "syn_send error");
        ev_close(&task->scheduler->netev, fd, skid);
        return;
    }
    data = custz_data(data, &size);
    ASSERTAB(0 == _memicmp(data, msg, strlen(msg)), "syn_send error");
    ev_close(&task->scheduler->netev, fd, skid);
    if (_prt) {
        LOG_INFO("_test_syn_ssl1_send ok.");
    }
#endif
}
static void _test_syn_ssl2_send(task_ctx *task) {
#if WITH_SSL
    uint64_t skid;
    struct evssl_ctx *ssl = srey_ssl_qury(task->scheduler, 101);
    SOCKET fd = coro_connect(task, PACK_CUSTZ, ssl, "127.0.0.1", 15001, &skid, NETEV_AUTHSSL);
    if (INVALID_SOCK == fd) {
        LOG_ERROR("%s", "syn_connect error");
        return;
    }
    const char *msg = "this is tcp task_coro_net.";
    size_t size;
    struct custz_pack_ctx *pack = custz_pack((void*)msg, strlen(msg), &size);
    void *data = coro_send(task, fd, skid, pack, size, &size, 0);
    if (NULL == data) {
        LOG_ERROR("%s", "syn_send error");
        ev_close(&task->scheduler->netev, fd, skid);
        return;
    }
    data = custz_data(data, &size);
    ASSERTAB(0 == _memicmp(data, msg, strlen(msg)), "syn_send error");
    ev_close(&task->scheduler->netev, fd, skid);
    if (_prt) {
        LOG_INFO("_test_syn_ssl2_send ok.");
    }
#endif
}
static void _test_syn_sendto(task_ctx *task) {
    uint64_t skid;
    SOCKET fd = trigger_udp(task, "0.0.0.0", 0, &skid);
    if (INVALID_SOCK == fd) {
        LOG_ERROR("%s", "trigger_udp error");
        return;
    }
    const char *msg = "this is udp task_coro_net.";
    size_t size;
    void *data = coro_sendto(task, fd, skid, "127.0.0.1", 15002, (void*)msg, strlen(msg), &size);
    if (NULL == data) {
        LOG_ERROR("%s", "syn_sendto error");
        ev_close(&task->scheduler->netev, fd, skid);
        return;
    }
    ASSERTAB(0 == _memicmp(data, msg, strlen(msg)), "syn_sendto error");
    ev_close(&task->scheduler->netev, fd, skid);
    if (_prt) {
        LOG_INFO("_test_syn_sendto ok.");
    }
}
static void _net_close(task_ctx *task, SOCKET fd, uint64_t skid, uint8_t pktype,
    uint8_t client) { }
static void _timeout(task_ctx *task, uint64_t sess) {
    _test_syn_send(task);
    _test_syn_sendto(task);
    _test_syn_ssl1_send(task);
    _test_syn_ssl2_send(task);
    trigger_timeout(task, 0, 1000, _timeout);
}
static void _startup(task_ctx *task) {
    on_closed(task, _net_close);
    trigger_timeout(task, 0, 1000, _timeout);
}
void task_coro_net_start(scheduler_ctx *scheduler, name_t name, int32_t pt) {
    _prt = pt;
    task_ctx *task = task_new(scheduler, name, NULL, NULL, NULL);
    task_register(task, _startup, NULL);
}

#endif
