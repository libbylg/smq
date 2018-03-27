#ifndef __smq_errors_H_
#define __smq_errors_H_

#include "smq.h"
#include "smq_limit.h"

enum    smq_error_naming
{
#define SMQ_ERR(name,id,cn,en)  name = (id),
#define SMQ_LOG(name,id,cn,en)
#include    "smq_res.h"
#undef  SMQ_ERR
#undef  SMQ_LOG
    SMQ_ERR_UPPER_LIMIT,

    SMQ_ERR_COUNT = (SMQ_ERR_UPPER_LIMIT - SMQ_ERR_LOWER_LIMIT)
};

typedef struct
{
    smq_int32   id;
    smq_uint16  len[SMQ_LOCALE_COUNT];
    smq_char*   desc[SMQ_LOCALE_COUNT];
}smq_error_t;

extern  smq_error_t smq_errors[SMQ_ERR_COUNT];

#endif