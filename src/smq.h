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
#else
#error  "Unsupported platform for 'SMQ_API'"
#endif




/// 定义了SMQ的调用协定
#if defined(WIN32) || defined(WIN64)
#define SMQ_CALL    __stdcall
#else
#define SMQ_CALL    __attribute__(__stdcall__)
#endif



/// 定义了所有的基本常量
#if defined(WIN32) || defined(WIN64)
#define smq_uint32  unsigned int
#define smq_uint16  unsigned short
#define smq_uint8   unsigned char
#define smq_int32   int
#define smq_int16   short
#define smq_int8    char
#define smq_char    char
#define smq_void    void
#elif defined(__gcc)
#define smq_uint32  unsigned int
#define smq_uint16  unsigned short
#define smq_uint8   unsigned char
#define smq_int32   int
#define smq_int16   short
#define smq_int8    char
#define smq_char    char
#define smq_void    void
#else
#error  "Unsupported os or platform"
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






/// 定义了语言相关的几个宏
///@{
#define SMQ_LOCALE_ENUS             (0) ///<    英文
#define SMQ_LOCALE_ZHCN             (1) ///<    中文
///@}




/// 角色分类
///@{
#define SMQ_ROLE_LEADER             (0)         ///<    主控对象
#define SMQ_ROLE_FOLLOWER           (1)         ///<    从控对象
#define SMQ_ROLE_VIEWER             (2)         ///<    观察者对象
///@}




/// 定义了日志级别
///@{
#define SMQ_LOG_LEVEL_DEBUG         (0)         ///<    调试级别
#define SMQ_LOG_LEVEL_INFO          (1)         ///<    信息级别
#define SMQ_LOG_LEVEL_WARN          (2)         ///<    警告级别
#define SMQ_LOG_LEVEL_ERROR         (3)         ///<    错误级别
#define SMQ_LOG_LEVEL_OFF           (4)         ///<    关闭日志
///@}



/// 定义了几个基本常量
///@{
#define SMQ_MAPPING_NAME_LEN_MAX    (100)       ///<    共享内存映射名称的最大长度(不含\0)
///@}



/// 定义了dump数据的范围
///@{
#define SMQ_DUMP_RANGE_HEAP_HEAD    (0x0001)
#define SMQ_DUMP_RANGE_HEAP_DATA    (0x0002)
///@}




/// 定义了通用的值类型
///@{
#if defined(WIN32) || defined(WIN64)
#pragma warning(disable:4200)
#endif
#define SMQ_VALUE_LEN_MAX       (256)   ///<    定义了参数的最大长度
typedef union
{
    smq_char    value_str[SMQ_VALUE_LEN_MAX];   ///<    字符串类型的值
    smq_uint32  value_uint32;                   ///<    uint32 类型的值
    smq_uint16  value_int32;                    ///<    int32 类型的值
    smq_void*   value_ptr;                      ///<    指针类型的值
    smq_uint32  value_uint32s[0];               ///<    uint32 类型的数组
    smq_uint16  value_int32s[0];                ///<    int32 类型的数组
    smq_void*   value_ptrs[0];                  ///<    指针数组
}smq_value_t;
#if defined(WIN32) || defined(WIN64)
#pragma warning(default:4200)
#endif
///@}




/// smq 输出日志时的回调函数
/// \param  context     [in]    日志上下文，通过 key 为 SMQ_PARAM_LOG_TARGET 的参数调用 #smq_param_set 函数时，第一个指针输入的就是context.
/// \param  id          [in]    日志编号.
/// \param  level       [in]    日志级别.
/// \param  loc         [in]    本地语言支持.
/// \param  desc        [in]    与 id 对应的，由 loc 指定的语言的日志静态描述.
/// \param  desc_len    [in]    desc 的长度，不包括\0.
/// \param  dynamic     [in]    日志的动态信息，一般是日志上下文的参数，参数的是 key1=vaule1,key2=vaule2,... 形式的多个 key-value 对，但并不保证 value 部分不包括字符'='或者','.
/// \param  dynamic_len [in]    dynamic 的长度，不包括\0.
typedef smq_void    (SMQ_CALL *SMQ_LOGGER_FUNC)(smq_void* context, smq_uint32 id, smq_uint32 level, smq_uint32 loc, smq_char* desc, smq_uint32 desc_len, smq_char* dynamic, smq_uint32 dynamic_len);




/// Dump共享内存详情
/// \param  context     [in]    Dump 上下文，该参数来源自调用 #smq_dump 时的同名输入参数
/// \param  flag        [in]    用于标记 Dump 的阶段，0 表示 Dump 开始，0xFFFFFFFF 表示 Dump 结束，其他值表示 Dump 的数据输出阶段。当处于 Dump 开始和结束阶段时，输入参数 data 和 len 分别为 NULL 和 0.
/// \param  data        [in]    Dump 的数据缓冲区指针。当 flag 指明当前处于 Dump 开始或者结束阶段时，该参数值为 NULL.
/// \param  len         [in]    该参数用于指明 data 参数中的数据区域的长度。当 flag 指明当前处于 Dump 开始或者结束阶段时，该参数值为 0.
/// \return 返回 0 表示执行需要继续执行后面的 Dump 操作，否则，将终止后续的操作。
typedef smq_int32   (SMQ_CALL *SMQ_DUMPER_FUNC)(smq_void* context, smq_uint32 flag, smq_void* data, smq_uint32 len);




/// 读取由 key 指定的全局参数的值
/// \param  key         [in]    key 用来标识需要获取哪个参数
/// \param  val         [out]   获取的参数会被放到 val 变量中
/// \return 返回 SMQ_OK 表示获取成功，其他表示获取失败
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_get(smq_uint32 key, smq_value_t* val);




/// 修改由 key 指定的全局参数的值
/// \param  key         [in]    key 用来标识需要修改哪个参数的值
/// \param  val         [in]    新的值放在 val 参数中
/// \return 返回 SMQ_OK 表示设置成功，其他表示获取失败及原因
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_set(smq_uint32 key, smq_value_t* val);




/// 检查是否可以将由 key 标识的参数的值修改为 val
/// \param  key         [in]    key 用来标识需要修改哪个参数的值
/// \param  val         [in]    新的值放在 val 参数中
/// \return 返回 SMQ_OK 表示检查通过，其他表示获取失败，可以通过 #smq_error 函数获取该错误的详细的错误描述文本
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_check(smq_uint32 key, smq_value_t* val);




/// 获取参数的错误描述
/// \param  err         [in]    错误码
/// \param  loc         [in]    本地语言编码
/// \param  desc        [out]   错误描述文本，该文本存储在 #smq_value_t 的 value_str 成员中，且以 \0 结尾
/// \return 返回 SMQ_OK 表示检查通过，其他表示获取错误描述失败
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_error(smq_errno err, smq_uint32 loc, smq_value_t* desc);




/// 创建 smq 共享内存实例，或者打开已经存在的 smq 共享内存实例
/// \param  name        [in]    smq 共享内存实例的名称
/// \param  role        [in]    实例的角色，可参见 SMQ_ROLE_xxx 系列宏了解有哪些角色
/// \param  inst        [out]   smq 共享内存实例对象
/// \return 返回 SMQ_OK 表示创建或者打开成功，其他表示打开失败的错误码
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_open(smq_char* name, smq_uint32 role, smq_inst* inst);





/// 关闭已经打开或者创建的 smq 共享内存实例
/// \param  inst        [in]    待关闭的 smq 共享内存实例
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_close(smq_inst inst);




/// 查询 SMQ 实例的共享内存布局的版本
/// \param  inst        [in]    SMQ 实例
/// \param  ver         [out]   版本号
/// \return 查询成功，返回 SMQ_OK，否则返回失败错误码
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_version(smq_inst inst, smq_uint32* ver);




/// Dump SMQ 实例所管理的共享内存中的所有数据
/// \param  inst        [in]    SMQ 实例
/// \param  range       [in]    Dump 数据范围
/// \param  context     [in]    Dump 上下文，该参数最终传递给回调函数 f，作为其的 context 参数
/// \param  f           [in]    Dump 回调函数，该函数会被调用多次，每次调用其输入参数均不相同，参见 #SMQ_DUMPER_FUNC 的说明
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_dump(smq_inst inst, smq_uint32 range, smq_void* context, SMQ_DUMPER_FUNC f);




/// 申请一个新的消息缓冲区
/// \param  inst        [in]    SMQ 实例
/// \param  size        [in]    期望的缓冲区大小。必须注意，该大小仅仅是申请缓冲区的参考，SMQ 尽量分配至少能够容纳下 size 字节的数据的缓冲区。
///                             但当共享内存不足时，实际返回的数据缓冲区也可能小于 size；所以，在拷贝数据之前，我们一般需要通过 #smq_msg_data 
///                             函数获取缓冲区的容量，才能决定需要向缓冲区拷贝多少数据。如果数据区域太小，可以继续继续调用本函数获取新的缓冲区。
///                             然后将新的缓冲区通过 #smq_msg_merge 函数合并到第一个消息缓冲区上，作为其子消息，并最终随着第一个消息一起发送
///                             出去。
/// \param  msg         [out]   获取到的消息缓冲区
/// \return 消息分配成功，返回值为 SMQ_OK，此时 *msg 的值一定可用；如果分配失败，会返回错误码
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_new(smq_inst inst, smq_uint32 size, smq_msg* msg);




/// 释放一个消息缓冲区
/// \param  inst        [in]    SMQ 实例
/// \param  msg         [out]   需要释放到的消息缓冲区，所有通过 #smq_msg_merge 函数合并到 msg 的子消息都会被一起自
///                             动释放。所有被合并（#smq_msg_merge）的消息都不需要通过本函数来主动释放。
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_del(smq_inst inst, smq_msg msg);



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_merge(smq_inst inst, smq_msg msg, smq_msg sub);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next);



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_data(smq_inst inst, smq_msg msg, smq_void** data, smq_uint32* len, smq_uint32* cap);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_fix(smq_inst inst, smq_msg msg, smq_uint32 len);


SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_post(smq_inst inst, smq_msg msg);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_wait(smq_inst inst, smq_int32 timeout, smq_msg* msg);
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_peek(smq_inst inst, smq_uint32* count);


/// 定义了一个适用于C语言的编译期断言宏，用于对编译环境进行一些基本的检查
///@{
#if defined(__cplusplus)
#define SMQ_STATIC_ASSERT(expr,message)     static_assert(expr,message)
#elif defined(WIN32) || defined(WIN64)
#define SMQ_STATIC_ASSERT(expr,message)     C_ASSERT(expr)
#else
#define SMQ_STATIC_ASSERT(expr,message)     ((void)sizeof(char[1 - 2*!!(expr)]))
#endif
SMQ_STATIC_ASSERT((4 == sizeof(smq_uint32)), "检查编译环境的关键数据长度一致性");
SMQ_STATIC_ASSERT((2 == sizeof(smq_uint16)), "检查编译环境的关键数据长度一致性");
SMQ_STATIC_ASSERT((1 == sizeof(smq_uint8 )), "检查编译环境的关键数据长度一致性");
SMQ_STATIC_ASSERT((4 == sizeof(smq_int32 )), "检查编译环境的关键数据长度一致性");
SMQ_STATIC_ASSERT((2 == sizeof(smq_int16 )), "检查编译环境的关键数据长度一致性");
SMQ_STATIC_ASSERT((1 == sizeof(smq_int8  )), "检查编译环境的关键数据长度一致性");
SMQ_STATIC_ASSERT((1 == sizeof(smq_char  )), "检查编译环境的关键数据长度一致性");
///@}



#endif//_smq_


