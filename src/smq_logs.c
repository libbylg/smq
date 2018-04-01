#include "smq_logs.h"
#include "smq_os.h"
#include "smq_libc.h"
#include "smq_params.h"


extern  smq_log_t smq_logs[SMQ_LOG_COUNT] = 
{
#define SMQ_ERR(name,id,cn,en)
#define SMQ_LOG(name,id,cn,en)  {name, {sizeof(en), sizeof(cn)}, {en, cn}},
#include    "smq_res.h"
#undef  SMQ_ERR
#undef  SMQ_LOG
};


static  SMQ_TLS smq_char    smq_log_cache[SMQ_LOG_CACHE_LEN_MAX];


SMQ_EXTERN  smq_void    smq_log_writer(smq_uint32 loc, smq_uint32 level, smq_uint32 id, smq_char* format, ...)
{
    if ((loc < SMQ_LOCALE_MIN) || (loc > SMQ_LOCALE_MAX))
    {
        loc = SMQ_LOCALE_DEF;
    }

    if (level < SMQ_LOG_LEVEL_MIN)
    {
        level = SMQ_LOG_LEVEL_MIN;
    }

    if (level > SMQ_LOG_LEVEL_MAX)
    {
        level = SMQ_LOG_LEVEL_MAX;
    }

    //  如果日志id超出范围，直接使用日志SMQ_LOG_LOWER_LIMIT的日志id输出日志，
    smq_log_t* log = NULL;
    if ((id < SMQ_LOG_LOWER_LIMIT) || (id > SMQ_LOG_UPPER_LIMIT))
    {
        log = &(smq_logs[SMQ_LOG_LOWER_LIMIT]);
    }
    else
    {
        log = &(smq_logs[id]);
    }

    va_list vargs;
    va_start(vargs, format);
    smq_int32 n = smq_vsprintf_s(smq_log_cache, sizeof(smq_log_cache), format, vargs);
    va_end(vargs);

    (*(smq_params.log_func))(smq_params.log_context, id, level, loc, log->desc[loc], log->len[loc], smq_log_cache, n);
}
