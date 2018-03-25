#ifndef __smq_errors_H_
#define __smq_errors_H_

#include "smq.h"
#include "smq_limit.h"

enum    smq_error_naming
{
    SMQ_ERR_MIN = 200,
#define SMQ_ERR(name,id,cn,en)  name = (id),
#define SMQ_LOG(name,id,cn,en)
#include    "smq_res.h"
#undef  SMQ_ERR
#undef  SMQ_LOG
    SMQ_ERR_MAX,

    SMQ_ERR_COUNT = (SMQ_ERR_MAX - SMQ_ERR_MIN),
};

typedef struct
{
    smq_int32   id;
    smq_uint16  len[SMQ_LOCALE_COUNT];
    smq_char*   desc[SMQ_LOCALE_COUNT];
}smq_error_t;

extern  smq_error_t smq_errors[SMQ_ERR_COUNT];

#endif