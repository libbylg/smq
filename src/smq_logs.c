#include "smq_logs.h"
#include "smq_limit.h"
#include "smq_os.h"


SMQ_EXTERN  smq_log_t smq_logs[SMQ_LOG_COUNT] = 
{
#define SMQ_ERR(name,id,cn,en)
#define SMQ_LOG(name,id,cn,en)  {name, {sizeof(en), sizeof(cn)}, {en, cn}},
#include    "smq_res.h"
#undef  SMQ_ERR
#undef  SMQ_LOG
};


static  SMQ_TLS smq_char    smq_log_cache[SMQ_LOG_CACHE_LEN_MAX];


SMQ_EXTERN  smq_void    smq_write_log(smq_uint32 loc, smq_uint32 level, smq_uint32 id, smq_char* format, ...)
{
    
}

