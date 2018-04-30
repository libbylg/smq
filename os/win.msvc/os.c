#include "smq_os.h"
#include "smq_libc.h"
#include "smq_errors.h"




BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



SMQ_EXTERN  smq_errno   smq_shm_open(smq_char* name, smq_uint32 name_len, smq_uint32 size, SMQ_BOOL writable, smq_shm_t* shm)
{
    //  ����ӳ���ļ�ȫ��
    smq_memcpy(shm->full_name, SMQ_MAPPING_PREFIX, sizeof(SMQ_MAPPING_PREFIX) - 1);
    smq_memcpy(shm->full_name + (sizeof(SMQ_MAPPING_PREFIX) - 1), name, name_len);
    shm->full_name[(sizeof(SMQ_MAPPING_PREFIX) - 1) + name_len] = '\0';

    /// �����ɹ���shm->addr���벻ΪNULL���Ա���smq_shm_close�ܹ���ȷ��ִ���ͷŲ���
    HANDLE handle = CreateFileMapping(INVALID_HANDLE_VALUE,
                                      NULL,
                                      (writable?PAGE_READWRITE:PAGE_READONLY),
                                      0,
                                      size,
                                      shm->full_name);
    DWORD last_error = GetLastError();
    if (NULL == handle)
    {
        shm->os_error = (smq_uint32)last_error;
        return  SMQ_ERR_CREATE_MAPPING_FAIELD;
    }


    //  ������ڴ�ӳ���ļ��Ѿ����ڣ���ôӳ���ļ���СӦ��Ϊ0
    smq_uint32 mapping_size = size;
    if (ERROR_ALREADY_EXISTS == last_error)
    {
        mapping_size = 0;
    }


    //  ������ӳ�䵽�����ڴ��ַ�ռ�
    DWORD dwDesiredAccess = FILE_MAP_READ | (writable?FILE_MAP_WRITE:0);
    LPVOID addr = MapViewOfFile(handle, dwDesiredAccess, 0, 0, mapping_size);
    if (NULL == addr)
    {
        shm->os_error = (smq_uint32)GetLastError();
        CloseHandle(handle);
        return  SMQ_ERR_CREATE_MAPPING_FAIELD; 
    }

    //  ��ѯ�����ڴ���ʵ��С
    MEMORY_BASIC_INFORMATION info = {0};
    if (VirtualQuery(addr, &info, sizeof(info)) != sizeof(info))
    {
        shm->os_error = (smq_uint32)GetLastError();
        CloseHandle(handle);
        return SMQ_ERR_CREATE_MAPPING_FAIELD; 
    }

    //  ���shm�ṹ
    shm->handle     = handle;
    shm->addr       = (smq_uint8*)addr;
    shm->real_size  = info.RegionSize;
    shm->is_exist   = ((ERROR_ALREADY_EXISTS == last_error)?SMQ_TRUE:SMQ_FALSE);
    smq_memcpy(shm->base_name,  name, name_len);
    shm->base_name[name_len] = '\0';
    shm->os_error   = 0;
    return  SMQ_OK;
}

SMQ_EXTERN  smq_void    smq_shm_close(smq_shm_t* shm)
{
    SMQ_ASSERT((NULL != shm), "ָ��������ⲿ��֤");

    if (NULL == shm->addr)
    {
        return;
    }

    if (NULL != shm->handle)
    {
        CloseHandle(shm->handle);
    }
}

SMQ_EXTERN  smq_errno   smq_proc_mutex_open(smq_char* name, smq_proc_mutex_t* mutex)
{
    return  SMQ_OK;
}

SMQ_EXTERN  smq_errno   smq_proc_mutex_lock(smq_proc_mutex_t* mutex)
{
    return  SMQ_OK;
}

SMQ_EXTERN  smq_errno   smq_proc_mutex_unlock(smq_proc_mutex_t* mutex)
{
    return  SMQ_OK;
}

SMQ_EXTERN  smq_void    smq_proc_mutex_close(smq_proc_mutex_t* mutex)
{
    return;
}

SMQ_EXTERN  smq_uint32  smq_get_pid()
{
    return  GetCurrentProcessId();
}

SMQ_EXTERN  smq_tid_t   smq_get_tid()
{
    return GetCurrentThreadId();   
}
