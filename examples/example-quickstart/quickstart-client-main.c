#include <stdio.h>
#include <stdarg.h>

#include "smq.h"

void printerr(smq_errno err, char* format, ...)
{
    smq_value_t desc = {0};
    smq_error(err, SMQ_LOCALE_ZH_CN, &desc);

    va_list vargs;
    va_start(vargs, format);
    vfprintf(stderr, format, vargs);
    va_end(vargs);

    fprintf(stderr, ":%s\n", desc.value_str);
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
    if (SMQ_OK != err)
    {
        printerr(err, "Create or open the instance of SMQ failed");
        return err;
    }

    while (1)
    {
        fgets()
    }
    smq_msg msg;
    smq_msg_new(inst, 0, &msg);

    //  close the SMQ instance
    smq_close(inst);

    return 0;
}

