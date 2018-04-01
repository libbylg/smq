#include "smq_utils.h"

SMQ_EXTERN  smq_uint16 SMQ_CALL smq_checksum(smq_uint8* data, smq_uint32 size)
{ 
    register smq_uint16* buffer = (smq_uint16*)data;
    smq_uint32  cksum  = 0;
    while (size > 1) 
    { 
        cksum += *buffer++;
        size  -= sizeof(smq_uint16);
    }

    if (size) 
    { 
        cksum += *(smq_uint8*)buffer;
    }

    //将32位数转换成16 
    while (cksum >> 16)
    {
        cksum = (cksum >> 16) + (cksum & 0xffff);
    }
    
    return (smq_uint16) (~cksum); 
}


