#ifndef __smq_params_H_
#define __smq_params_H_

#include "smq.h"

typedef struct
{
    smq_uint32          log_level;
    smq_void*           log_context;
    SMQ_LOGGER_FUNC     log_func;

    smq_uint32          memory_size;

    smq_uint32          queue_size;

    smq_uint32          locale;
}smq_params_t;

extern  smq_params_t    smq_params;

#endif//__smp_params_H_
