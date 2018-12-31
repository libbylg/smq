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


void echo_server(smq_inst inst)
{
    smq_msg msg = SMQ_MSG_NULL;
    while (SMQ_OK == smq_msg_wait(inst, 0, &msg))
    {
        smq_errno err = SMQ_OK;
        for (smq_msg itr = msg; (SMQ_OK == err) && (itr != SMQ_MSG_NULL); err = smq_msg_next(inst, msg, &itr))
        {
            void*      data = NULL;
            smq_uint32 len  = 0;
            smq_msg_data(inst, itr, &data, &len, NULL);
            fwrite(data, len, 1, stdout);
        }

        fputc('\n', stdout);

        //  return the same message
        smq_msg_post(inst, msg);
    }
}


int main(char* args[])
{
    //  change the log level to debug
    smq_value_t level;
    level.value_uint32 = SMQ_LOG_LEVEL_DEBUG;
    smq_param_set(SMQ_PARAM_LOG_LEVEL, &level);


    //  create or open SMQ instance
    smq_inst inst = SMQ_INST_NULL;
    smq_errno err = smq_open("quick-start", SMQ_ROLE_FOLLOWER, &inst);
    if (SMQ_OK != err)                      \
    {                                       \
        printerr(err, "Create or open the instance of SMQ failed");
        return err;
    }


    //  startup the command scan
    (void)echo_server(inst);


    //  close the SMQ instance
    smq_close(inst);
    return 0;
}

