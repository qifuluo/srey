#include "task_coro_utils.h"

#if WITH_CORO

static int32_t _prt = 0;
static void _test_dns(task_ctx *task) {
    dns_ip *ips;
    size_t n;
    if (_prt) {
        LOG_INFO("%s", "coro_dns_lookup:");
    }
    ips = coro_dns_lookup(task, 1000, "8.8.8.8", "www.google.com", 0, &n);
    if (NULL != ips) {
        if (_prt) {
            for (size_t i = 0; i < n; i++) {
                LOG_INFO("%s", ips[i].ip);
            }
        }
        FREE(ips);
    }
    ips = coro_dns_lookup(task, 1000, "8.8.8.8", "www.google.com", 1, &n);
    if (NULL != ips) {
        if (_prt) {
            for (size_t i = 0; i < n; i++) {
                LOG_INFO("%s", ips[i].ip);
            }
        }
        FREE(ips);
    }
}
static void _timeout(task_ctx *task, uint64_t sess) {
    _test_dns(task);
}
static void _startup(task_ctx *task) {
    trigger_timeout(task, createid(), 3000, _timeout);
}
void task_coro_utils_start(scheduler_ctx *scheduler, name_t name, int32_t pt) {
    _prt = pt;
    task_ctx *task = task_new(scheduler, name, NULL, NULL, NULL);
    task_register(task, _startup, NULL);
}

#endif
