#ifndef TASK_CORO_COMM1_H_
#define TASK_CORO_COMM1_H_

#include "lib.h"

#if WITH_CORO
void task_coro_comm1_start(loader_ctx *loader, name_t name, int32_t pt);
#endif
#endif//TASK_CORO_COMM1_H_
