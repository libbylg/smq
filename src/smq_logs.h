#ifndef __smq_log_H_
#define __smq_log_H_

#include "smq.h"

#include "smq_limit.h"

#include "smq_params.h"

enum    smq_log_naming
{
#define SMQ_ERR(name,id,cn,en)  
#define SMQ_LOG(name,id,cn,en)  name = (id),
#include    "smq_res.h"
#undef  SMQ_ERR
#undef  SMQ_LOG
    SMQ_LOG_UPPER_LIMIT,

    SMQ_LOG_COUNT = (SMQ_LOG_UPPER_LIMIT - SMQ_LOG_LOWER_LIMIT)
};

typedef struct
{
    smq_int32   id;
    smq_uint16  len[SMQ_LOCALE_COUNT];
    smq_char*   desc[SMQ_LOCALE_COUNT];
}smq_log_t;

extern  smq_log_t smq_logs[SMQ_LOG_UPPER_LIMIT - SMQ_LOG_LOWER_LIMIT];



SMQ_EXTERN  smq_void    smq_log_writer(smq_uint32 loc, smq_uint32 level, smq_uint32 id, smq_char* format, ...);


/// 使用下面这组宏，输出日志效率更高
///@{
#define SMQ_ERROR(id,format,...)    if ((SMQ_LOG_LEVEL_ERROR) >= smq_params.log_level) smq_log_writer(smq_params.locale, (SMQ_LOG_LEVEL_ERROR), (id), format, __VA_ARGS__)
#define SMQ_WARN(id,format,...)     if ((SMQ_LOG_LEVEL_WARN)  >= smq_params.log_level) smq_log_writer(smq_params.locale, (SMQ_LOG_LEVEL_WARN),  (id), format, __VA_ARGS__)
#define SMQ_INFO(id,format,...)     if ((SMQ_LOG_LEVEL_INFO)  >= smq_params.log_level) smq_log_writer(smq_params.locale, (SMQ_LOG_LEVEL_INFO),  (id), format, __VA_ARGS__)
#define SMQ_DEBUG(id,format,...)    if ((SMQ_LOG_LEVEL_DEBUG) >= smq_params.log_level) smq_log_writer(smq_params.locale, (SMQ_LOG_LEVEL_DEBUG), (id), format, __VA_ARGS__)
///@}

#endif

