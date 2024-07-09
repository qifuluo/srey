#include "srey/loader.h"
#include "containers/hashmap.h"
#include "srey/task.h"

loader_ctx *g_loader;

static uint64_t _map_task_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    return hash((const char *)(*(name_t **)item), sizeof(name_t));
}
static int _map_task_compare(const void *a, const void *b, void *ud) {
    return *(*(name_t **)a) - *(*(name_t **)b);
}
static void _map_task_free(void *item) {
    task_free(UPCAST(*((name_t **)item), task_ctx, name));
}
static int32_t _max_task_index(loader_ctx *loader) {
    uint16_t index = 0;
    uint32_t max = qu_task_size(&loader->worker[0].qutasks);
    uint32_t count;
    for (uint16_t i = 1; i < loader->nworker; i++) {
        count = qu_task_size(&loader->worker[i].qutasks);
        if (count > max) {
            index = i;
            max = count;
        }
    }
    return 0 == max ? -1 : (int32_t)index;
}
static void _worker_wakeup_all(loader_ctx *loader) {
    worker_ctx *worker;
    for (uint16_t i = 0; i < loader->nworker; i++) {
        worker = &loader->worker[i];
        mutex_lock(&worker->mutex);
        if (worker->waiting > 0) {
            cond_signal(&worker->cond);
        }
        mutex_unlock(&worker->mutex);
    }
}
static void _worker_wakeup(loader_ctx *loader, name_t *task) {
    worker_ctx *worker = &loader->worker[
        (1 == loader->nworker) ? 0 : (ATOMIC64_ADD(&loader->index, 1) % loader->nworker)];
    spin_lock(&worker->lcktasks);
    qu_task_push(&worker->qutasks, task);
    spin_unlock(&worker->lcktasks);
    mutex_lock(&worker->mutex);
    if (worker->waiting > 0) {
        cond_signal(&worker->cond);
    }
    mutex_unlock(&worker->mutex);
}
void _task_message_push(task_ctx *task, message_ctx *msg) {
    int32_t add = 0;
    spin_lock(&task->lckmsg);
    qu_message_push(&task->qumsg, msg);
    if (0 == task->global) {
        add = 1;
        task->global = 1;
    }
    spin_unlock(&task->lckmsg);
    if (0 != add) {
        _worker_wakeup(task->loader, &task->name);
    }
}
static name_t _task_name_get(loader_ctx *loader, worker_ctx *worker) {
    name_t *ptr;
    name_t name = INVALID_TNAME;
    spin_lock(&worker->lcktasks);
    ptr = qu_task_pop(&worker->qutasks);
    if (NULL != ptr) {
        name = *ptr;
    }
    spin_unlock(&worker->lcktasks);
    if (INVALID_TNAME == name) {
        int32_t index = _max_task_index(loader);
        if (-1 != index) {
            spin_lock(&loader->worker[index].lcktasks);
            ptr = qu_task_pop(&loader->worker[index].qutasks);
            if (NULL != ptr) {
                name = *ptr;
            }
            spin_unlock(&loader->worker[index].lcktasks);
        }
    }
    return name;
}
static int32_t _task_message_pop(task_ctx *task, message_ctx *msg) {
    message_ctx *tmp;
    spin_lock(&task->lckmsg);
    tmp = qu_message_pop(&task->qumsg);
    if (NULL == tmp) {
        spin_unlock(&task->lckmsg);
        return ERR_FAILED;
    }
    *msg = *tmp;
    spin_unlock(&task->lckmsg);
    return ERR_OK;
}
static void _task_run(loader_ctx *loader, worker_ctx *worker,
    worker_version *version, task_dispatch_arg *runarg) {
    //ִ��
    version->name = runarg->task->name;
    while (ERR_OK == _task_message_pop(runarg->task, &runarg->msg)) {
        ++version->ver;
        version->msgtype = runarg->msg.mtype;
        runarg->task->_task_dispatch(runarg);
        version->msgtype = MSG_TYPE_NONE;
    }
    //�ӻض���
    int32_t add = 1;
    spin_lock(&runarg->task->lckmsg);
    if (0 == qu_message_size(&runarg->task->qumsg)) {
        add = 0;
        runarg->task->global = 0;
    }
    spin_unlock(&runarg->task->lckmsg);
    if (0 != add) {
        _worker_wakeup(loader, &runarg->task->name);
    }
}
static void _worker_hang(loader_ctx *loader, worker_ctx *worker) {
    mutex_lock(&worker->mutex);
    ++worker->waiting;
    cond_wait(&worker->cond, &worker->mutex);
    --worker->waiting;
    mutex_unlock(&worker->mutex);
}
static void _worker_loop(void *arg) {
    name_t name;
    worker_ctx *worker = (worker_ctx *)arg;
    loader_ctx *loader = worker->loader;
    worker_version *version = &loader->monitor.version[worker->index];
    task_dispatch_arg runarg;
    while (0 == loader->stop) {
        //�Ӷ���ȡһ����
        name = _task_name_get(loader, worker);
        if (INVALID_TNAME != name) {
            runarg.task = task_grab(loader, name);
            if (NULL == runarg.task) {
                continue;
            }
            _task_run(loader, worker, version, &runarg);
            task_ungrab(runarg.task);
            continue;
        }
        _worker_hang(loader, worker);
    }
    LOG_INFO("worker thread %d exited.", worker->index);
}
//�����ѭ��
static void _monitor_check(loader_ctx *loader) {
    worker_version *version;
    for (uint16_t i = 0; i < loader->nworker; i++) {
        version = &loader->monitor.version[i];
        if (version->ckver == version->ver
            && MSG_TYPE_NONE != version->msgtype) {
            LOG_WARN("task: %d message type: %d, maybe in an endless loop.",
                version->name, version->msgtype);
        } else {
            version->ckver = version->ver;
        }
    }
}
static void _monitor_loop(void *arg) {
    loader_ctx *loader = (loader_ctx *)arg;
    uint64_t time = 0;
    while (0 == loader->monitor.stop) {
        MSLEEP(100);
        time += 100;
        if (0 == time % 5000) {
            _monitor_check(loader);
        }
    }
    LOG_INFO("%s", "worker monitor thread exited.");
}
loader_ctx *loader_init(uint16_t nnet, uint16_t nworker) {
    loader_ctx *loader;
    CALLOC(loader, 1, sizeof(loader_ctx));
    protos_init(_message_handshaked_push);
#if WITH_CORO
    _mcoro_init(0);
#endif
#if WITH_SSL
    evssl_init();
    evssl_pool_init();
#endif
    loader->nworker = 0 == nworker ? 1 : nworker;
    CALLOC(loader->worker, 1, sizeof(worker_ctx) * loader->nworker);
    CALLOC(loader->monitor.version, 1, sizeof(worker_version) * loader->nworker);
    rwlock_init(&loader->lckmaptasks);
    loader->maptasks = hashmap_new_with_allocator(_malloc, _realloc, _free,
                                                  sizeof(name_t *), ONEK, 0, 0,
                                                  _map_task_hash, _map_task_compare, _map_task_free, NULL);
    loader->monitor.thread_monitor = thread_creat(_monitor_loop, loader);
    worker_ctx *worker;
    for (uint16_t i = 0; i < loader->nworker; i++) {
        worker = &loader->worker[i];
        worker->index = i;
        worker->loader = loader;
        spin_init(&worker->lcktasks, SPIN_CNT_LOADER);
        qu_task_init(&worker->qutasks, ONEK);
        mutex_init(&worker->mutex);
        cond_init(&worker->cond);
        worker->thread_worker = thread_creat(_worker_loop, worker);
    }
    tw_init(&loader->tw);
    ev_init(&loader->netev, nnet);
    return loader;
}
static bool _closing_push(const void *item, void *udata) {
    task_ctx *task = UPCAST(*((name_t **)item), task_ctx, name);
    if (ATOMIC_CAS(&task->closing, 0, 1)) {
        _task_message_push(task, udata);
    }
    return true;
}
static bool _closing_timeout(const void *item, void *udata) {
    task_ctx *task = UPCAST(*((name_t **)item), task_ctx, name);
    LOG_WARN("task %d close timeout, ref %d.", task->name, task->ref);
    return true;
}
static void _task_closing(loader_ctx *loader) {
    message_ctx closing;
    closing.mtype = MSG_TYPE_CLOSING;
    rwlock_rdlock(&loader->lckmaptasks);
    hashmap_scan(loader->maptasks, _closing_push, &closing);
    rwlock_unlock(&loader->lckmaptasks);
    size_t n;
    uint32_t time = 0;
    for (;;) {
        rwlock_rdlock(&loader->lckmaptasks);
        n = hashmap_count(loader->maptasks);
        if (0 == n) {
            rwlock_unlock(&loader->lckmaptasks);
            break;
        }
        if (time >= 5 * 1000) {
            time = 0;
            hashmap_scan(loader->maptasks, _closing_timeout, NULL);
        }
        rwlock_unlock(&loader->lckmaptasks);
        MSLEEP(50);
        time += 50;
    }
}
void loader_free(loader_ctx *loader) {
    _task_closing(loader);
    loader->stop = 1;
    worker_ctx *worker;
    _worker_wakeup_all(loader);
    for (uint16_t i = 0; i < loader->nworker; i++) {
        worker = &loader->worker[i];
        thread_join(worker->thread_worker);
    }
    loader->monitor.stop = 1;
    thread_join(loader->monitor.thread_monitor);
    ev_free(&loader->netev);
    tw_free(&loader->tw);
    for (uint16_t i = 0; i < loader->nworker; i++) {
        worker = &loader->worker[i];
        spin_free(&worker->lcktasks);
        qu_task_free(&worker->qutasks);
        mutex_free(&worker->mutex);
        cond_free(&worker->cond);
    }
    hashmap_free(loader->maptasks);
    rwlock_free(&loader->lckmaptasks);
    protos_free();
#if WITH_SSL
    evssl_pool_free();
#endif
    FREE(loader->worker);
    FREE(loader->monitor.version);
    FREE(loader);
}
