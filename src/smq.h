#ifndef _smq_
#define _smq_



/// 定义了外部引用宏
#if  defined(__cplusplus)
#define SMQ_EXTERN  extern  "C"
#else
#define SMQ_EXTERN  extern
#endif




/// 定义了符号导入和导出宏
#if   defined(WIN32) || defined(WIN64)
#ifdef SMQ_EXPORTS
#define SMQ_API __declspec(dllexport)
#else
#define SMQ_API __declspec(dllimport)
#endif
#elif defined(__linux)
#define SMQ_API
#endif




/// 定义了SMQ的调用协定
#define SMQ_CALL    __stdcall




/// 定义了所有的基本常量
#if defined(WIN32) || defined(WIN64)
#define smq_uint32  unsigned int
#define smq_uint16  unsigned short
#define smq_uint8   unsigned char
#define smq_int32   int
#define smq_int16   short
#define smq_int8    char
#define smq_void    void
#define smq_char    char
#else
#define smq_uint32  unsigned int
#define smq_uint16  unsigned short
#define smq_uint8   unsigned char
#define smq_int32   int
#define smq_int16   short
#define smq_int8    char
#define smq_void    void
#define smq_char    char
#endif




/// 定义了接口中用到的几个重要类型，以及这些类型相关的几个基本常量
#define smq_errno               smq_int32       ///<    错误码类型
#define smq_msg                 smq_uint32      ///<    消息类型
#define smq_inst                smq_void*       ///<    smq实例类型
#define SMQ_OK                  (0)             ///<    表示成功的错误码
#define SMQ_MSG_NULL            (0)             ///<    无效消息
#define SMQ_INST_NULL           NULL            ///<    定义了无效实例对象




/// 下面定义了获取和设置参数相关的信息
/// @{
#define SMQ_PARAM_LOG_LEVEL             0   ///<    (get/set smq_uint32)日志级别
#define SMQ_PARAM_LOG_TARGET            1   ///<    (---/set smq_ptr[2])日志回调函数
#define SMQ_PARAM_MEMORY_SIZE           2   ///<    (get/set smq_uint32)共享内存大小
#define SMQ_PARAM_QUEUE_SIZE            3   ///<    (get/set smq_uint32)消息队列大小
#define SMQ_PARAM_SUPPORTED_VERSIONS    4   ///<    (get/--- smq_uint32[2])当前所支持的共享内存的版本号范围
#define SMQ_PARAM_LOCALE                5   ///<    (get/set smq_uint32)本地语言信息
/// @}




/// 定义了通用的值类型
#if defined(WIN32) || defined(WIN64)
#pragma warning(disable:4200)
#endif
#define SMQ_VALUE_LEN_MAX       (256)   ///<    定义了参数的最大长度
typedef union
{
    smq_char    value_str[SMQ_VALUE_LEN_MAX];   ///<    字符串类型的值
    smq_uint32  value_uint32;                   ///<    uint32类型的值
    smq_uint16  value_uint16;                   ///<    uint16类型的值
    smq_uint8   value_uint8;                    ///<    uint8类型的值
    smq_void*   value_ptr;                      ///<    指针类型的值
    smq_uint32  value_uint32s[0];               ///<    uint32类型的数组
    smq_uint16  value_uint16s[0];               ///<    uint16类型的数组
    smq_uint8   value_uint8s[0];                ///<    uint8类型的数组
    smq_void*   value_ptrs[0];                  ///<    指针数组
}smq_value_t;
#if defined(WIN32) || defined(WIN64)
#pragma warning(default:4200)
#endif
///@}




/// 定义了语言相关的几个宏
///@{
#define SMQ_LOCALE_ENUS             (0) ///<    英文
#define SMQ_LOCALE_ZHCN             (1) ///<    中文
///@}




/// 角色分类
///@{
#define SMQ_ROLE_MASTER             (0)         ///<    主控对象
#define SMQ_ROLE_SLAVE              (1)         ///<    从控对象
#define SMQ_ROLE_VIEWER             (2)         ///<    观察者对象
///@}




/// 定义了日志级别
///@{
#define SMQ_LOG_LEVEL_OFF           (0)         ///<    关闭日志
#define SMQ_LOG_LEVEL_ERROR         (1)         ///<    错误级别
#define SMQ_LOG_LEVEL_WARN          (2)         ///<    警告级别
#define SMQ_LOG_LEVEL_INFO          (3)         ///<    信息级别
#define SMQ_LOG_LEVEL_DEBUG         (4)         ///<    调试级别
///@}



/// 定义了几个基本常量
///@{
#define SMQ_MAPPING_NAME_LEN_MAX    (100)       ///<    共享内存映射名称的最大长度(不含\0)
///@}




typedef smq_int32   (SMQ_CALL *SMQ_LOGGER_FUNC)(smq_void* context, smq_uint32 id, smq_uint32 level, smq_uint32 loc, smq_char* desc, smq_uint32 desc_len, smq_char* synamic, smq_uint32 synamic_len);
typedef smq_int32   (SMQ_CALL *SMQ_DUMPER_FUNC)(smq_void* context, smq_uint32 flag, smq_void* data, smq_uint32 len);




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_get(smq_uint32 key, smq_value_t* val);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_set(smq_uint32 key, smq_value_t* val);




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_error(smq_errno err, smq_uint32 loc, smq_value_t* desc);



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_open(smq_char* name, smq_uint32 role, smq_uint32 size, smq_inst* inst);
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_close(smq_inst inst);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_version(smq_inst inst, smq_uint32* ver);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_dump(smq_inst inst, smq_uint32 range, smq_void* context, SMQ_DUMPER_FUNC f);



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_new(smq_inst inst, smq_uint32 size, smq_msg* msg);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_del(smq_inst inst, smq_msg* msg);



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_merge(smq_inst inst, smq_msg msg, smq_msg sub);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next);



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_data(smq_inst inst, smq_msg msg, smq_void** data, smq_uint32* len, smq_uint32* cap);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_fix(smq_inst inst, smq_msg msg, smq_uint32 len);




/// 定义了一个适用于C语言的编译期断言宏，用于对编译环境进行一些基本的检查
///@{
#if defined(WIN32) || defined(WIN64)
#define SMQ_STATIC_ASSERT(expr,message) C_ASSERT(expr)
#else
#define SMQ_STATIC_ASSERT(expr,message) ((void)sizeof(char[1 - 2*!!(expr)]))
#endif

// SMQ_STATIC_ASSERT((4 == sizeof(smq_uint32)), "smq_uint32长度必须是4字节");
// SMQ_STATIC_ASSERT((2 == sizeof(smq_uint16)), "smq_uint32长度必须是4字节");
// SMQ_STATIC_ASSERT((1 == sizeof(smq_uint8 )), "smq_uint32长度必须是4字节");
// SMQ_STATIC_ASSERT((4 == sizeof(smq_int32 )), "smq_uint32长度必须是4字节");
// SMQ_STATIC_ASSERT((2 == sizeof(smq_int16 )), "smq_uint32长度必须是4字节");
// SMQ_STATIC_ASSERT((1 == sizeof(smq_int8  )), "smq_uint32长度必须是4字节");
// SMQ_STATIC_ASSERT((1 == sizeof(smq_char  )), "smq_uint32长度必须是4字节");
///@}



#endif//_smq_


