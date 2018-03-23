#ifndef __smq_log_H_
#define __smq_log_H_

#include "smq.h"

#include "smq_limit.h"

enum    smq_log_naming
{
    SMQ_LOG_MIN = 200,
#define SMQ_ERR(name,id,cn,en)  
#define SMQ_LOG(name,id,cn,en)  name = (id),
#include    "smq_res.h"
#undef  SMQ_ERR
#undef  SMQ_LOG
    SMQ_LOG_MAX,

    SMQ_LOG_COUNT = (SMQ_LOG_MAX - SMQ_LOG_MIN),
};

typedef struct
{
    smq_int32   id;
    smq_uint16  len[SMQ_LOCALE_COUNT];
    smq_char*   desc[SMQ_LOCALE_COUNT];
}smq_log_t;

SMQ_EXTERN  smq_log_t smq_logs[SMQ_LOG_COUNT];

SMQ_EXTERN  smq_void    smq_log_writer(smq_uint32 loc, smq_uint32 level, smq_uint32 id, smq_char* format, ...);

#define SMQ_ERROR(id,format,...)    
#define SMQ_WARN(id,format,...)    
#define SMQ_INFO(id,format,...)    
#define SMQ_DEBUG(id,format,...)    

#endif