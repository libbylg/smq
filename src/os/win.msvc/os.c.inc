// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息

// Windows 头文件:
#include <windows.h>

#include "os/win.msvc/os.h"


BOOL APIENTRY DllMain( HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
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

SMQ_EXTERN  smq_errno   smq_shm_open(smq_char* name, smq_uint32 size, smq_shm_t* shm)
{
    /// 创建成功后，shm->addr必须不为NULL，以便于smq_shm_close能够正确地执行释放操作
    return  SMQ_OK;
}

SMQ_EXTERN  smq_void    smq_shm_close(smq_shm_t* shm)
{
    SMQ_ASSERT((NULL != shm), "指针入参由外部保证");

    if (NULL == shm->addr)
    {
        return;
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

