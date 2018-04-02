#ifndef _smq_
#define _smq_



/// �������ⲿ���ú�
#if  defined(__cplusplus)
#define SMQ_EXTERN  extern  "C"
#else
#define SMQ_EXTERN  extern
#endif




/// �����˷��ŵ���͵�����
#if   defined(WIN32) || defined(WIN64)
#ifdef SMQ_EXPORTS
#define SMQ_API __declspec(dllexport)
#else
#define SMQ_API __declspec(dllimport)
#endif
#elif defined(__linux)
#define SMQ_API
#elif defined(__clang__)
#define SMQ_API
#else
#error  "Unsupported platform for 'SMQ_API'"
#endif




/// ������SMQ�ĵ���Э��
#if defined(WIN32) || defined(WIN64)
#define SMQ_CALL    __stdcall
#else
#define SMQ_CALL    __attribute__(__stdcall__)
#endif



/// ���������еĻ�������
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
#elif defined(__clang__)
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




/// ������ SMQ_NULL ����
///@{
#if defined(NULL)
#define SMQ_NULL    NULL
#else 
#ifdef __cplusplus
#define SMQ_NULL	0
#else
#define SMQ_NULL	((void *)0)
#endif
#endif
///@}



/// �����˽ӿ����õ��ļ�����Ҫ���ͣ��Լ���Щ������صļ�����������
#define smq_errno               smq_int32       ///<    ����������
#define smq_msg                 smq_uint32      ///<    ��Ϣ����
#define smq_inst                smq_void*       ///<    smqʵ������
#define SMQ_OK                  (0)             ///<    ��ʾ�ɹ��Ĵ�����
#define SMQ_MSG_NULL            (0)             ///<    ��Ч��Ϣ
#define SMQ_INST_NULL           SMQ_NULL        ///<    ��������Чʵ������




/// ���涨���˻�ȡ�����ò�����ص���Ϣ
/// @{
#define SMQ_PARAM_LOG_LEVEL         (0)         ///<    (get/set smq_uint32)��־����
#define SMQ_PARAM_LOG_TARGET        (1)         ///<    (---/set smq_ptr[2])��־�ص�����
#define SMQ_PARAM_MEMORY_SIZE       (2)         ///<    (get/set smq_uint32)�����ڴ��С
#define SMQ_PARAM_QUEUE_SIZE        (3)         ///<    (get/set smq_uint32)��Ϣ���д�С
#define SMQ_PARAM_VERSIONS          (4)         ///<    (get/--- smq_uint32[2])��ǰ��֧�ֵĹ����ڴ�İ汾�ŷ�Χ
#define SMQ_PARAM_LOCALE            (5)         ///<    (get/set smq_uint32)����������Ϣ
/// @}




/// ������������صļ�����
///@{
#define SMQ_LOCALE_ENUS             (0)         ///<    Ӣ��
#define SMQ_LOCALE_ZHCN             (1)         ///<    ����
///@}




/// ��ɫ����
///@{
#define SMQ_ROLE_LEADER             (0)         ///<    ���ض���
#define SMQ_ROLE_FOLLOWER           (1)         ///<    �ӿض���
#define SMQ_ROLE_VIEWER             (2)         ///<    �۲��߶���
///@}




/// ��������־����
///@{
#define SMQ_LOG_LEVEL_DEBUG         (0)         ///<    ���Լ���
#define SMQ_LOG_LEVEL_INFO          (1)         ///<    ��Ϣ����
#define SMQ_LOG_LEVEL_WARN          (2)         ///<    ���漶��
#define SMQ_LOG_LEVEL_ERROR         (3)         ///<    ���󼶱�
#define SMQ_LOG_LEVEL_OFF           (4)         ///<    �ر���־
///@}



/// �����˼�����������
///@{
#define SMQ_MAPPING_NAME_LEN_MAX    (100)       ///<    �����ڴ�ӳ�����Ƶ���󳤶�(����\0)
///@}



/// ������dump���ݵķ�Χ
///@{
#define SMQ_DUMP_RANGE_HEAP_HEAD    (0x0001)    ///<    Dump �����ڴ�ͷ��
#define SMQ_DUMP_RANGE_HEAP_DATA    (0x0002)    ///<    Dump �����ڴ�������
///@}




/// ������ͨ�õ�ֵ����
///@{
#if defined(WIN32) || defined(WIN64)
#pragma warning(disable:4200)
#endif
typedef union
{
    smq_char    value_str[256];                 ///<    �ַ������͵�ֵ
    smq_uint32  value_uint32;                   ///<    uint32 ���͵�ֵ
    smq_uint16  value_int32;                    ///<    int32 ���͵�ֵ
    smq_void*   value_ptr;                      ///<    ָ�����͵�ֵ
    smq_uint32  value_uint32s[0];               ///<    uint32 ���͵�����
    smq_uint16  value_int32s[0];                ///<    int32 ���͵�����
    smq_void*   value_ptrs[0];                  ///<    ָ������
}smq_value_t;
#if defined(WIN32) || defined(WIN64)
#pragma warning(default:4200)
#endif
///@}




/// smq �����־ʱ�Ļص�����
///
/// \param  context     [in]    ��־�����ģ�ͨ�� key Ϊ SMQ_PARAM_LOG_TARGET �Ĳ������� #smq_param_set ����ʱ����һ��ָ������ľ���context.
/// \param  id          [in]    ��־���.
/// \param  level       [in]    ��־����.
/// \param  loc         [in]    ��������֧��.
/// \param  desc        [in]    �� id ��Ӧ�ģ��� loc ָ�������Ե���־��̬����.
/// \param  desc_len    [in]    desc �ĳ��ȣ�������\0.
/// \param  dynamic     [in]    ��־�Ķ�̬��Ϣ��һ������־�����ĵĲ������������� key1=vaule1,key2=vaule2,... ��ʽ�Ķ�� key-value �ԣ���������֤ value ���ֲ������ַ�'='����','.
/// \param  dynamic_len [in]    dynamic �ĳ��ȣ�������\0.
typedef smq_void    (SMQ_CALL *SMQ_LOGGER_FUNC)(smq_void* context, smq_uint32 id, smq_uint32 level, smq_uint32 loc, smq_char* desc, smq_uint32 desc_len, smq_char* dynamic, smq_uint32 dynamic_len);




/// Dump�����ڴ�����
///
/// \param  context     [in]    Dump �����ģ��ò�����Դ�Ե��� #smq_dump ʱ��ͬ���������
/// \param  flag        [in]    ���ڱ�� Dump �Ľ׶Σ�0 ��ʾ Dump ��ʼ��0xFFFFFFFF ��ʾ Dump ����������ֵ��ʾ Dump ����������׶Ρ������� Dump ��ʼ�ͽ����׶�ʱ��������� data �� len �ֱ�Ϊ NULL �� 0.
/// \param  data        [in]    Dump �����ݻ�����ָ�롣�� flag ָ����ǰ���� Dump ��ʼ���߽����׶�ʱ���ò���ֵΪ NULL.
/// \param  len         [in]    �ò�������ָ�� data �����е���������ĳ��ȡ��� flag ָ����ǰ���� Dump ��ʼ���߽����׶�ʱ���ò���ֵΪ 0.
/// \return ���� 0 ��ʾִ����Ҫ����ִ�к���� Dump ���������򣬽���ֹ�����Ĳ�����
typedef smq_int32   (SMQ_CALL *SMQ_DUMPER_FUNC)(smq_void* context, smq_uint32 flag, smq_void* data, smq_uint32 len);




/// ��ȡ�� key ָ����ȫ�ֲ�����ֵ
///
/// \param  key         [in]    key ������ʶ��Ҫ��ȡ�ĸ�����
/// \param  val         [out]   ��ȡ�Ĳ����ᱻ�ŵ� val ������
/// \return ���� SMQ_OK ��ʾ��ȡ�ɹ���������ʾ��ȡʧ��
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_get(smq_uint32 key, smq_value_t* val);




/// �޸��� key ָ����ȫ�ֲ�����ֵ
///
/// \param  key         [in]    key ������ʶ��Ҫ�޸��ĸ�������ֵ
/// \param  val         [in]    �µ�ֵ���� val ������
/// \return ���� SMQ_OK ��ʾ���óɹ���������ʾ��ȡʧ�ܼ�ԭ��
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_set(smq_uint32 key, smq_value_t* val);




/// ����Ƿ���Խ��� key ��ʶ�Ĳ�����ֵ�޸�Ϊ val
/// \param  key         [in]    key ������ʶ��Ҫ�޸��ĸ�������ֵ
/// \param  val         [in]    �µ�ֵ���� val ������
/// \return ���� SMQ_OK ��ʾ���ͨ����������ʾ��ȡʧ�ܣ�����ͨ�� #smq_error ������ȡ�ô������ϸ�Ĵ��������ı�
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_check(smq_uint32 key, smq_value_t* val);




/// ��ȡ�����Ĵ�������
///
/// \param  err         [in]    ������
/// \param  loc         [in]    �������Ա���
/// \param  desc        [out]   ���������ı������ı��洢�� #smq_value_t �� value_str ��Ա�У����� \0 ��β
/// \return ���� SMQ_OK ��ʾ���ͨ����������ʾ��ȡ��������ʧ��
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_error(smq_errno err, smq_uint32 loc, smq_value_t* desc);




/// ���� smq �����ڴ�ʵ�������ߴ��Ѿ����ڵ� smq �����ڴ�ʵ��
///
/// \param  name        [in]    smq �����ڴ�ʵ��������
/// \param  role        [in]    ʵ���Ľ�ɫ���ɲμ� SMQ_ROLE_xxx ϵ�к��˽�����Щ��ɫ
/// \param  inst        [out]   smq �����ڴ�ʵ������
/// \return ���� SMQ_OK ��ʾ�������ߴ򿪳ɹ���������ʾ��ʧ�ܵĴ�����
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_open(smq_char* name, smq_uint32 role, smq_inst* inst);





/// �ر��Ѿ��򿪻��ߴ����� smq �����ڴ�ʵ��
///
/// \param  inst        [in]    ���رյ� smq �����ڴ�ʵ��
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_close(smq_inst inst);




/// ��ѯ SMQ ʵ���Ĺ����ڴ沼�ֵİ汾
///
/// \param  inst        [in]    SMQ ʵ��
/// \param  ver         [out]   �汾��
/// \return ��ѯ�ɹ������� SMQ_OK�����򷵻�ʧ�ܴ�����
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_version(smq_inst inst, smq_uint32* ver);




/// Dump SMQ ʵ���������Ĺ����ڴ��е���������
/// \param  inst        [in]    SMQ ʵ��
/// \param  range       [in]    Dump ���ݷ�Χ
/// \param  context     [in]    Dump �����ģ��ò������մ��ݸ��ص����� f����Ϊ��� context ����
/// \param  f           [in]    Dump �ص��������ú����ᱻ���ö�Σ�ÿ�ε������������������ͬ���μ� #SMQ_DUMPER_FUNC ��˵��
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_dump(smq_inst inst, smq_uint32 range, smq_void* context, SMQ_DUMPER_FUNC f);




/// ����һ���µ���Ϣ������
///
/// \param  inst        [in]    SMQ ʵ��
/// \param  size        [in]    �����Ļ�������С������ע�⣬�ô�С���������뻺�����Ĳο���SMQ �������������ܹ������� size �ֽڵ����ݵĻ�������
///                             ���������ڴ治��ʱ��ʵ�ʷ��ص����ݻ�����Ҳ����С�� size�����ԣ��ڿ�������֮ǰ������һ����Ҫͨ�� #smq_msg_data 
///                             ������ȡ�����������������ܾ�����Ҫ�򻺳��������������ݡ������������̫С�����Լ����������ñ�������ȡ�µĻ�������
///                             Ȼ���µĻ�����ͨ�� #smq_msg_cat �����ϲ�����һ����Ϣ�������ϣ���Ϊ������Ϣ�����������ŵ�һ����Ϣһ����
///                             ��ȥ��
/// \param  msg         [out]   ��ȡ������Ϣ������
/// \return ��Ϣ����ɹ�������ֵΪ SMQ_OK����ʱ *msg ��ֵһ�����ã��������ʧ�ܣ��᷵�ش�����
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_new(smq_inst inst, smq_uint32 size, smq_msg* msg);




/// �ͷ�һ����Ϣ������
///
/// \param  inst        [in]    SMQ ʵ��
/// \param  msg         [out]   ��Ҫ�ͷŵ�����Ϣ������������ͨ�� #smq_msg_cat �����ϲ��� msg ������Ϣ���ᱻ��
///                             ���ͷţ�������Ҫͨ���������������ͷš�
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_del(smq_inst inst, smq_msg msg);




/// ��һ�� sub ��Ϣƴ�ӵ� msg ��Ϣ����ĩβ����Ϊ������Ϣ
///
/// \param  inst        [in]    SMQ ʵ��
/// \param  msg         [in]    sub ��Ϣ��׷�ӵ� msg ����ָ������Ϣ����ĩβ
/// \param  sub         [in]    ��׷�ӵ�����Ϣ��
/// \return ƴ�ӳɹ������� SMQ_OK����ƴ��ʧ��ʱ�����ش����룬��ϵͳȷ�� msg �� sub ���ᱻ�ƻ�
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_cat(smq_inst inst, smq_msg msg, smq_msg sub);




/// ͨ�������ķ�ʽ��������� msg ����������Ϣ
/// \param  inst        [in]    SMQ ʵ��
/// \param  msg         [in]    �� msg ������Ϣ���б������ڱ����Ķ�ε��ù����� msg �������ܸı�
/// \param  next        [in,out]    ���������������ȡ����Ϣ�ɹ�����ô *next ��ָ���ȡ��������Ϣ����� *next Ϊ SMQ_SMQ_NULL��
///                                 ��ʾ�Ѿ��޺����Ϣ����ʱӦ��ֹͣ������������ next ���������ʱ��*next ��ֵ��������� msg��
///                                 ��ʾ��ȡ msg �ĵ�һ������Ϣ��������һ��ͨ������ msg ��������Ϣ������ȡ��Ϣ�е����ݵĹ��÷�ʾ����
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
/// \return ��ȡ�ɹ������� SMQ_OK����ͨ�� *next ȡ����Ϣ����ȡʧ��
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next);




//  ��ȡ msg ��Ϣ�����ݴ洢��ַ�����ݳ����Լ�������Ϣ
/// \param  inst        [in]        SMQ ʵ��
/// \param  msg         [in]        ָ����ȡ����
/// \param  data        [out]       ���ȡ�������ݵ�ַ��������Ҫȡ�ø���Ϣʱ��������д NULL
/// \param  len         [out]       ���ȡ�������ݳ��ȣ�������Ҫȡ�ø���Ϣʱ��������д NULL
/// \paran  cap         [out]       ���ȡ�������������������Ϣ��������û�д�������ô cap ����� len����������Ҫȡ�ø���Ϣʱ��������д NULL
SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_data(smq_inst inst, smq_msg msg, smq_void** data, smq_uint32* len, smq_uint32* cap);




/// �޶� msg ���������ĳ���
///
/// \param  inst        [in]        SMQ ʵ��
/// \param  msg         [in]        ��������������Ϣ����
/// \param  len         [in]        ��Ϣ���ݳ��Ƚ��趨Ϊ len
/// \return ����趨�ɹ������� SMQ_OK���������ʧ�ܣ����ش�����
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_fix(smq_inst inst, smq_msg msg, smq_uint32 len);




/// Ͷ�ݣ����ͣ�һ����Ϣ
///
/// \param  inst        [in]        SMQ ʵ��
/// \param  msg         [in]        �� msg ��ϢͶ�ݵ� inst ʵ������һ��
/// \return Ͷ�ݳɹ������� SMQ_OK��Ͷ��ʧ�ܣ����ش����롣
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_post(smq_inst inst, smq_msg msg);




/// ���շ��͵����˵���Ϣ
///
/// \param  inst        [in]        SMQ ʵ��
/// \param  timeout     [in]        �ȴ���ʱʱ������λ��΢�룬��� timeout ��ֵΪ 0����ʾ��Զ����ʱ�������յ���Ϣ����ϵͳ�˳�
/// \param  msg         [out]       ���յ�������Ϣ
/// \return ������յ�����Ϣ���ߵȴ���ʱ�������� SMQ_OK�����ȴ���Ϣ��ʱʱ��*msg ��ֵΪ SMQ_MSG_NULL
///         ������չ������������󣬷��ش����롣
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_wait(smq_inst inst, smq_int32 timeout, smq_msg* msg);




/// ��ȡһ����Ϣ�����л��ж�����Ϣδ��ȡ
///
/// \param  inst        [in]        SMQ ʵ��
/// \param  count       [out]       �����Ϣ������ֵ
/// \return �����ȡ�ɹ������� SMQ_OK�����򣬷���ʧ�ܴ����롣
SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_peek(smq_inst inst, smq_uint32* count);




/// ������һ��������C���Եı����ڶ��Ժ꣬���ڶԱ��뻷������һЩ�����ļ��
///@{
#if defined(__cplusplus)
#define SMQ_STATIC_ASSERT(expr,message)     static_assert(expr,message)
#elif defined(WIN32) || defined(WIN64)
#define SMQ_STATIC_ASSERT(expr,message)     C_ASSERT(expr)
#else
#define SMQ_STATIC_ASSERT(expr,message)     ((void)sizeof(char[1 - 2*!!(expr)]))
#endif
SMQ_STATIC_ASSERT((4 == sizeof(smq_uint32)), "�����뻷���Ĺؼ����ݳ���һ����");
SMQ_STATIC_ASSERT((2 == sizeof(smq_uint16)), "�����뻷���Ĺؼ����ݳ���һ����");
SMQ_STATIC_ASSERT((1 == sizeof(smq_uint8 )), "�����뻷���Ĺؼ����ݳ���һ����");
SMQ_STATIC_ASSERT((4 == sizeof(smq_int32 )), "�����뻷���Ĺؼ����ݳ���һ����");
SMQ_STATIC_ASSERT((2 == sizeof(smq_int16 )), "�����뻷���Ĺؼ����ݳ���һ����");
SMQ_STATIC_ASSERT((1 == sizeof(smq_int8  )), "�����뻷���Ĺؼ����ݳ���һ����");
SMQ_STATIC_ASSERT((1 == sizeof(smq_char  )), "�����뻷���Ĺؼ����ݳ���һ����");
///@}



#endif//_smq_

