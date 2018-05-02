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



inline      smq_char*   smq_str_append(smq_char* name, smq_uint32* len, smq_char* app, smq_uint32 app_len)
{
    smq_memcpy(name + *len, app, app_len);
    *len += app_len;
    return  name;
}



SMQ_EXTERN  smq_errno   smq_shm_open(smq_char* name, smq_uint32 name_len, smq_uint32 size, SMQ_BOOL writable, smq_shm_t* shm)
{
    //  生成映射文件全名
    smq_uint32 len = 0;
    smq_str_append(shm->full_name, &len, SMQ_PREFIX_GLOBAL,  sizeof(SMQ_PREFIX_GLOBAL) - 1);
    smq_str_append(shm->full_name, &len, name,               name_len);
    smq_str_append(shm->full_name, &len, SMQ_SUFFIX_MAPPING, sizeof(SMQ_SUFFIX_MAPPING) - 1);
    shm->full_name[len] = '\0';

    /// 创建成功后，shm->addr必须不为NULL，以便于smq_shm_close能够正确地执行释放操作
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


    //  如果该内存映射文件已经存在，那么映射文件大小应该为0
    smq_uint32 mapping_size = size;
    if (ERROR_ALREADY_EXISTS == last_error)
    {
        mapping_size = 0;
    }


    //  将对象映射到进程内存地址空间
    DWORD dwDesiredAccess = FILE_MAP_READ | (writable?FILE_MAP_WRITE:0);
    LPVOID addr = MapViewOfFile(handle, dwDesiredAccess, 0, 0, mapping_size);
    if (NULL == addr)
    {
        shm->os_error = (smq_uint32)GetLastError();
        CloseHandle(handle);
        return  SMQ_ERR_CREATE_MAPPING_FAIELD; 
    }

    //  查询共享内存真实大小
    MEMORY_BASIC_INFORMATION info = {0};
    if (VirtualQuery(addr, &info, sizeof(info)) != sizeof(info))
    {
        shm->os_error = (smq_uint32)GetLastError();
        CloseHandle(handle);
        return SMQ_ERR_CREATE_MAPPING_FAIELD; 
    }

    //  填充shm结构
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
    SMQ_ASSERT((NULL != shm), "指针入参由外部保证");

    if (NULL == shm->addr)
    {
        return;
    }

    if (NULL != shm->handle)
    {
        CloseHandle(shm->handle);
    }
}

SMQ_EXTERN  smq_errno   smq_proc_mutex_open(smq_char* name, smq_uint32 name_len, smq_proc_mutex_t* mutex)
{
    //  生成映射文件全名
    smq_uint32 len = 0;
    smq_str_append(mutex->full_name, &len, SMQ_PREFIX_GLOBAL,  sizeof(SMQ_PREFIX_GLOBAL) - 1);
    smq_str_append(mutex->full_name, &len, name,               name_len);
    smq_str_append(mutex->full_name, &len, SMQ_SUFFIX_MAPPING, sizeof(SMQ_SUFFIX_MAPPING) - 1);
    mutex->full_name[len] = '\0';


    //  创建命名互斥体
    mutex->handle = CreateMutex(NULL, FALSE, mutex->full_name);
    if (NULL == mutex->handle)
    {
        return SMQ_ERR_CREATE_OR_OPEN_MUTEX_FAILED;
    }

    return  SMQ_OK;
}

SMQ_EXTERN  smq_errno   smq_proc_mutex_lock(smq_proc_mutex_t* mutex)
{
    //  等着占用锁
    DWORD r = WaitForSingleObject(mutex->handle, INFINITE);
    if (WAIT_OBJECT_0 != r)
    {
        return  SMQ_ERR_LOCK_FAILED;
    }

    return  SMQ_OK;
}

SMQ_EXTERN  smq_errno   smq_proc_mutex_unlock(smq_proc_mutex_t* mutex)
{
    BOOL r = ReleaseMutex(mutex->handle);
    if (FALSE == r)
    {
        return SMQ_ERR_UNLOCK_FAILED;
    }

    return  SMQ_OK;
}

SMQ_EXTERN  smq_void    smq_proc_mutex_close(smq_proc_mutex_t* mutex)
{
    if (NULL == mutex)
    {
        return;
    }

    if (NULL == mutex->handle)
    {
        return;
    }

    CloseHandle(mutex->handle);
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
