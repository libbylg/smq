#include <stdio.h>
#include "smq.h"

int main(char* args[])
{
    smq_inst inst = SMQ_INST_NULL;
    smq_errno err = smq_open("quick-start", SMQ_ROLE_LEADER, &inst);
    if (SMQ_OK != err)
    {

    }

    return 0;
}

