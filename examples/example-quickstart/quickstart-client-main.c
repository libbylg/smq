#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "smq.h"

void printerr(smq_errno err, char* format, ...)
{
    if (SMQ_OK == err)
    {
        return;
    }

    smq_value_t desc = {0};
    smq_error(err, SMQ_LOCALE_ZH_CN, &desc);

    va_list vargs;
    va_start(vargs, format);
    vfprintf(stderr, format, vargs);
    va_end(vargs);

    fprintf(stderr, ":%s\n", desc.value_str);
}

#define RETURN_IF_ERR(err,msg,...)          \
    if (SMQ_OK != err)                      \
    {                                       \
        printerr(err, msg, __VA_ARGS__);    \
        return err;                         \
    } do{}while(0)

#define BREAK_IF_ERR(err,msg,...)           \
    if (SMQ_OK != err)                      \
    {                                       \
        printerr(err, msg, __VA_ARGS__);    \
        break;                              \
    } do{}while(0)



smq_errno commands_scan(smq_inst inst)
{
    smq_msg msg = SMQ_MSG_NULL;
    smq_errno err = smq_msg_new(inst, 30, &msg);
    RETURN_IF_ERR(err, "Create message failed");


    char cache[100];
    while (1)
    {
        char* p = fgets(cache, sizeof(cache), stdin);
        if (NULL == p)
        {
            break;
        }

        size_t size = strlen(p);
        if (0 == size)
        {
            continue;
        }

        if (size < sizeof(cache))
        {
            if ('\n' == cache[size])
            {
                err = smq_msg_post(inst, msg);
                BREAK_IF_ERR(err, "Post message failed");

                    err = smq_msg_new(inst, 30, &msg); 
                BREAK_IF_ERR(err, "Create message failed");

                    continue;
            }

            err = smq_msg_write(inst, msg, cache, size);
            BREAK_IF_ERR(err, "Write message failed");
        }
    }

    return SMQ_OK;
}


int main(char* args[])
{
    //  change the log level to debug
    smq_value_t level;
    level.value_uint32 = SMQ_LOG_LEVEL_DEBUG;
    smq_param_set(SMQ_PARAM_LOG_LEVEL, &level);


    //  create or open SMQ instance
    smq_inst inst = SMQ_INST_NULL;
    smq_errno err = smq_open("quick-start", SMQ_ROLE_LEADER, &inst);
    RETURN_IF_ERR(err, "Create or open the instance of SMQ failed");


    //  startup the command scan
    (void)commands_scan(inst);


    //  close the SMQ instance
    smq_close(inst);
    return 0;
}

