#ifndef _smq_
#define _smq_



/// 定义了外部引用宏
#if  defined(__cplusplus)
#define SMQ_EXTERN  extern  "C"
#else
#define SMQ_EXTERN  extern
#endif




/// 定义了符号导入和导出宏
#if   defined(_WIN32)
#ifdef SMQ_EXPORTS
#define SMQ_API __declspec(dllexport)
#else
#define SMQ_API __declspec(dllimport)
#endif
#else
#define SMQ_API
#endif




/// 定义了SMQ的调用协定
#if   defined(_WIN32)
#define SMQ_CALL    __stdcall
#else
#define SMQ_CALL    __attribute__((__stdcall__))
#endif




/// 定义了所有的基本常量
///@{
#define smq_uint32  unsigned int
#define smq_uint16  unsigned short
#define smq_uint8   unsigned char
#define smq_int32   int
#define smq_int16   short
#define smq_int8    char
#define smq_char    char
#define smq_void    void
///@}




/// 定义了 SMQ_NULL 常量
///@{
#if defined(NULL)
#define SMQ_NULL    NULL
#else 
#ifdef __cplusplus
#define SMQ_NULL    (0)
#else
#define SMQ_NULL    ((void *)0)
#endif
#endif
///@}




/// 定义了接口中用到的几个重要类型，以及这些类型相关的几个基本常量
#define smq_errno               smq_int32       ///<    错误码类型
#define smq_msg                 smq_uint32      ///<    消息类型
#define smq_inst                smq_void*       ///<    SMQ 实例类型
#define SMQ_OK                  (0)             ///<    表示成功的错误码
#define SMQ_MSG_NULL            (0)             ///<    无效消息
#define SMQ_INST_NULL           SMQ_NULL        ///<    定义了无效实例对象




/// 下面定义了获取和设置参数相关的信息，主要用于 #smq_param_xxx 三个函数
/// @{
#define SMQ_PARAM_LOG_LEVEL         (0)         ///<    (get/set smq_uint32)日志级别，允许的日志级别参见 #SMQ_LOG_LEVEL_XXX 系列常量，缺省日志级别是 #SMQ_LOG_LEVEL_INFO
#define SMQ_PARAM_LOG_TARGET        (1)         ///<    (---/set smq_ptr[2])日志回调函数，输入参数为两个指针，第一个为上下回调上下文参数，第二个为回调函数，函数原型为 #SMQ_LOGGER_FUNC
#define SMQ_PARAM_MEMORY_SIZE       (2)         ///<    (get/set smq_uint32)共享内存大小，单位是 M，取值范围参见 #SMQ_MEMORY_SIZE_XXX 系列常量
#define SMQ_PARAM_QUEUE_SIZE        (3)         ///<    (get/set smq_uint32)消息队列大小，该参数决定了允许缓存的消息队列的个数，取值范围及缺省值参见 #SMQ_MESSAGE_QUEUE_SIZE_XXX 系列常量
#define SMQ_PARAM_VERSIONS          (4)         ///<    (get/--- smq_uint32[2])当前所支持的共享内存的版本号范围，该参数为内部常量，不支持设置
#define SMQ_PARAM_LOCALE            (5)         ///<    (get/set smq_uint32)本地语言信息，参见 #SMQ_LOCALE_XXX 系列常量
/// @}




/// 定义了语言相关的几个宏，用于 #smq_param_xxx 系列函数，当输入参数的 key 为 #SMQ_PARAM_LOCALE 时起作用。
///@{
#define SMQ_LOCALE_ZH_CN            (0)         ///<    中文
#define SMQ_LOCALE_EN_US            (1)         ///<    英文
///@}




/// 角色分类，用于 #smq_open 函数
///@{
#define SMQ_ROLE_LEADER             (0)         ///<    主控对象，可以对打开的 SMQ 实例执行修改
#define SMQ_ROLE_FOLLOWER           (1)         ///<    从控对象，可以对打开的 SMQ 实例执行修改
#define SMQ_ROLE_VIEWER             (2)         ///<    观察者对象，使用这种角色得开或者创建的 SMQ 实例是只读的，任何一个 SMQ 实例，只能有一个主和一个从对象控制，但是可以有多个观察者对象
///@}




/// 定义了日志级别
///@{
#define SMQ_LOG_LEVEL_DEBUG         (0)         ///<    调试级别
#define SMQ_LOG_LEVEL_INFO          (1)         ///<    信息级别
#define SMQ_LOG_LEVEL_WARN          (2)         ///<    警告级别
#define SMQ_LOG_LEVEL_ERROR         (3)         ///<    错误级别
#define SMQ_LOG_LEVEL_OFF           (4)         ///<    关闭日志，日志级别可以用于 #smq_param_xxx 系列函数，但是不能用于写日志
///@}




/// 定义了几个基本常量
///@{
#define SMQ_MAPPING_NAME_LEN_MAX    (100)       ///<    共享内存映射名称的最大长度(不含\0)，用于 #smq_open 函数
///@}



/// 定义了 dump 数据的范围，用于 #smq_dump 和 #smq_parser 的回调函数
///@{
#define SMQ_DUMP_RANGE_HEAP_HEAD    (0x0001)    ///<    Dump 共享内存头部
#define SMQ_DUMP_RANGE_HEAP_DATA    (0x0002)    ///<    Dump 共享内存数据区
///@}




/// 定义了通用的值类型
///@{
#if   defined(_MSC_VER)
#pragma warning(disable:4200)   ///<    VC 编译器特殊处理
#endif
typedef union
{
    smq_char    value_str[256];                 ///<    字符串类型的值
    smq_uint32  value_uint32;                   ///<    uint32 类型的值
    smq_uint32  value_int32;                    ///<    int32 类型的值
    smq_void*   value_ptr;                      ///<    指针类型的值
    smq_uint32  value_uint32s[0];               ///<    uint32 类型的数组
    smq_uint32  value_int32s[0];                ///<    int32 类型的数组
    smq_void*   value_ptrs[0];                  ///<    指针数组
}smq_value_t;
#if   defined(_MSC_VER)
#pragma warning(default:4200)   ///<    VC 编译器特殊处理
#endif
///@}




/// smq 输出日志时的回调函数
///
/// \param  context     [in]    日志上下文，通过 key 为 SMQ_PARAM_LOG_TARGET 的参数调用 #smq_param_set 函数时，第一个指针输入的就是context.
/// \param  id          [in]    日志编号.
/// \param  level       [in]    日志级别.
/// \param  loc         [in]    本地语言支持.
/// \param  desc        [in]    与 id 对应的，由 loc 指定的语言的日志静态描述.
/// \param  desc_len    [in]    desc 的长度，不包括\0.
/// \param  dynamic     [in]    日志的动态信息，一般是日志上下文的参数，参数的是 key1=vaule1,key2=vaule2,... 形式的多个 key-value 对，但并不保证 value 部分不包括字符'='或者','.
/// \param  dynamic_len [in]    dynamic 的长度，不包括\0.
typedef smq_void    (SMQ_CALL *SMQ_LOGGER_FUNC)(smq_void* context, smq_int32 id, smq_uint32 level, smq_uint32 loc, smq_char* desc, smq_uint32 desc_len, smq_char* dynamic, smq_uint32 dynamic_len);




/// Dump SMQ 实例中共享内存的数据
///
/// \param  context     [in]    Dump 上下文，该参数来源自调用 #smq_dump 时的同名输入参数
/// \param  flag        [in]    用于标记 Dump 的阶段，0 表示 Dump 开始，0xFFFFFFFF 表示 Dump 结束，其他值表示 Dump 的数据输出阶段。当处于 Dump 开始和结束阶段时，输入参数 data 和 len 分别为 NULL 和 0.
/// \param  data        [in]    Dump 的数据缓冲区指针。当 flag 指明当前处于 Dump 开始或者结束阶段时，该参数值为 NULL.
/// \param  len         [in]    该参数用于指明 data 参数中的数据区域的长度。当 flag 指明当前处于 Dump 开始或者结束阶段时，该参数值为 0.
/// \return 返回 0 表示执行需要继续执行后面的 Dump 操作，否则，将终止后续的操作。
typedef smq_int32   (SMQ_CALL *SMQ_DUMPER_FUNC)(smq_void* context, smq_uint32 flag, smq_void* data, smq_uint32 len);




//  对 Dump 出来的数据进行解析
//
//  \param  context     [in]    解析上下文参数，来自于 #smq_parse 的同名输入参数
//  \param  flag        [in]    用于标记 Dump 的阶段，0 表示解析开始，0xFFFFFFFF 表示解析结束，其他值表示解析的数据输出阶段。当处于解析开始和结束阶段时，输入参数 data 和 len 分别为 NULL 和 0.
//  \param  action      [in]    解析采用了 Access 模式，该参数用于决定当前的解析动作，参见 #SMQ_ACTION_XXX 系列宏
//  \param  data_name   [in]    数据的内部名称，以\0结束
//  \param  data_type   [in]    数据的类型
//  \param  data        [in]    数据的内容
//  \param  len         [in]    数据的长度
//  \return 返回 0 表示执行需要继续执行后面的解析操作，否则，将终止后续的操作。
typedef smq_int32   (SMQ_CALL *SMQ_PARSER_FUNC)(smq_void* context, smq_uint32 flag, smq_int32 action, smq_char* data_name, smq_uint32 data_type, smq_void* data, smq_uint32 len);




/// 读取由 key 指定的全局参数的值
///
/// \param  key         [in]    key 用来标识需要获取哪个参数
/// \param  val         [out]   获取的参数会被放到 val 变量中
/// \return 返回 SMQ_OK 表示获取成功，其他表示获取失败
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_get(smq_uint32 key, smq_value_t* val);




/// 修改由 key 指定的全局参数的值
///
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
///
/// \param  err         [in]    错误码
/// \param  loc         [in]    本地语言编码
/// \param  desc        [out]   错误描述文本，该文本存储在 #smq_value_t 的 value_str 成员中，且以 \0 结尾
/// \return 返回 SMQ_OK 表示检查通过，其他表示获取错误描述失败
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_error(smq_errno err, smq_uint32 loc, smq_value_t* desc);




/// 创建 smq 共享内存实例，或者打开已经存在的 smq 共享内存实例
///
/// \param  name        [in]    smq 共享内存实例的名称
/// \param  role        [in]    实例的角色，可参见 SMQ_ROLE_xxx 系列宏了解有哪些角色
/// \param  inst        [out]   smq 共享内存实例对象
/// \return 返回 SMQ_OK 表示创建或者打开成功，其他表示打开失败的错误码
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_open(smq_char* name, smq_uint32 role, smq_inst* inst);





/// 关闭已经打开或者创建的 smq 共享内存实例
///
/// \param  inst        [in]    待关闭的 smq 共享内存实例
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_close(smq_inst inst);




/// 查询 SMQ 实例的共享内存布局的版本
///
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



/// Dump 数据解析器
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_parse(smq_inst inst, smq_msg msg, smq_void* context, SMQ_PARSER_FUNC f);




/// 申请一个新的消息缓冲区
///
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
///
/// \param  inst        [in]    SMQ 实例
/// \param  msg         [out]   需要释放到的消息缓冲区，所有通过 #smq_msg_merge 函数合并到 msg 的子消息都会被自
///                             动释放，而不需要通过本函数来主动释放。
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_del(smq_inst inst, smq_msg msg);




/// 将一个 sub 消息拼接到 msg 消息链的末尾，作为其子消息
///
/// \param  inst        [in]    SMQ 实例
/// \param  msg         [in]    sub 消息将追加到 msg 参数指定的消息链的末尾
/// \param  sub         [in]    被追加的子消息。
/// \return 拼接成功，返回 SMQ_OK；当拼接失败时，返回错误码，且系统确保 msg 和 sub 不会被破坏
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_merge(smq_inst inst, smq_msg msg, smq_msg sub);




/// 通过迭代的方式，逐个遍历 msg 的所有子消息
/// \param  inst        [in]    SMQ 实例
/// \param  msg         [in]    对 msg 的子消息进行遍历，在遍历的多次调用过程中 msg 参数不能改变
/// \param  next        [in,out]    迭代参数，如果获取子消息成功，那么 *next 将指向获取到的子消息；如果 *next 为 SMQ_SMQ_NULL，
///                                 表示已经无后继消息，此时应该停止遍历操作。当 next 做输入参数时，*next 的值，如果等于 msg，
///                                 表示获取 msg 的第一个子消息，下面是一个通过遍历 msg 及其子消息，并获取消息中的数据的管用法示例：
///
///     smq_errno err = SMQ_OK;
///     for (smq_msg* next = &msg; (err == SMQ_OK) && (next != SMQ_MSG_NULL); err = smq_msg_next(inst, msg, next)
///     {
///         
///         smq_void*  data = NULL;
///         smq_uint32 len  = 0;
///         smq_uint32 cap  = 0;
///         smq_errno  ret  = smq_msg_data(inst, *next, &data, &len, &cap);
///
///         // your code
///
///     }
///     
/// \return 获取成功，返回 SMQ_OK，可通过 *next 取得消息；获取失败
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next);




//  获取 msg 消息的数据存储地址、数据长度以及容量信息
/// \param  inst        [in]        SMQ 实例
/// \param  msg         [in]        指定获取对象
/// \param  data        [out]       存放取到的数据地址，当不需要取得该信息时，可以填写 NULL
/// \param  len         [out]       存放取到的数据长度，当不需要取得该信息时，可以填写 NULL
/// \paran  cap         [out]       存放取到的数据容量，如果消息缓冲区并没有存满，那么 cap 会大于 len。，当不需要取得该信息时，可以填写 NULL
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_data(smq_inst inst, smq_msg msg, smq_void** data, smq_uint32* len, smq_uint32* cap);




/// 修订 msg 的数据区的长度
///
/// \param  inst        [in]        SMQ 实例
/// \param  msg         [in]        本函数操作的消息对象
/// \param  len         [in]        消息数据长度将设定为 len
/// \return 如果设定成功，返回 SMQ_OK；如果设置失败，返回错误码
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_fix(smq_inst inst, smq_msg msg, smq_uint32 len);




/// 投递（发送）一个消息
///
/// \param  inst        [in]        SMQ 实例
/// \param  msg         [in]        把 msg 消息投递到 inst 实例的另一端
/// \return 投递成功，返回 SMQ_OK；投递失败，返回错误码。
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_post(smq_inst inst, smq_msg msg);




/// 接收发送到本端的消息
///
/// \param  inst        [in]        SMQ 实例
/// \param  timeout     [in]        等待超时时长，单位是微秒，如果 timeout 的值为 0，表示永远不超时，除非收到消息或者系统退出
/// \param  msg         [out]       接收到的新消息
/// \return 如果接收到新消息或者等待超时，均返回 SMQ_OK，但等待消息超时时，*msg 的值为 SMQ_MSG_NULL
///         如果接收过程中遇到错误，返回错误码。
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_wait(smq_inst inst, smq_int32 timeout, smq_msg* msg);




/// 获取一下消息队列中还有多少消息未收取
///
/// \param  inst        [in]        SMQ 实例
/// \param  count       [out]       存放消息数量的值
/// \return 如果获取成功，返回 SMQ_OK，否则，返回失败错误码。
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_peek(smq_inst inst, smq_uint32* count);



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_write(smq_inst inst, smq_msg msg, smq_void* data, smq_uint32 len);

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_read(smq_inst inst, smq_msg msg, smq_msg* itr, smq_void** data, smq_uint32* len);



/// 定义了一个适用于C语言的编译期断言宏，用于对编译环境进行一些基本的检查
///@{
#if     defined(__cplusplus)
#define SMQ_STATIC_ASSERT(expr,message)     static_assert(expr,message)
#else
#if   defined(_MSC_VER)
#define SMQ_STATIC_ASSERT(expr,message)     C_ASSERT(expr)
#else
//#define SMQ_STATIC_ASSERT(expr,message)     ((void)sizeof(char[1 - 2*!!(expr)]))
#define SMQ_STATIC_ASSERT(expr,message)     _Static_assert(expr,message)
#endif
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


