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
#endif




/// ������SMQ�ĵ���Э��
#define SMQ_CALL    __stdcall




/// ���������еĻ�������
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




/// �����˽ӿ����õ��ļ�����Ҫ���ͣ��Լ���Щ������صļ�����������
#define smq_errno               smq_int32       ///<    ����������
#define smq_msg                 smq_uint32      ///<    ��Ϣ����
#define smq_inst                smq_void*       ///<    smqʵ������
#define SMQ_OK                  (0)             ///<    ��ʾ�ɹ��Ĵ�����
#define SMQ_MSG_NULL            (0)             ///<    ��Ч��Ϣ
#define SMQ_INST_NULL           NULL            ///<    ��������Чʵ������




/// ���涨���˻�ȡ�����ò�����ص���Ϣ
/// @{
#define SMQ_PARAM_LOG_LEVEL             0   ///<    (get/set smq_uint32)��־����
#define SMQ_PARAM_LOG_TARGET            1   ///<    (---/set smq_ptr[2])��־�ص�����
#define SMQ_PARAM_MEMORY_SIZE           2   ///<    (get/set smq_uint32)�����ڴ��С
#define SMQ_PARAM_QUEUE_SIZE            3   ///<    (get/set smq_uint32)��Ϣ���д�С
#define SMQ_PARAM_SUPPORTED_VERSIONS    4   ///<    (get/--- smq_uint32[2])��ǰ��֧�ֵĹ����ڴ�İ汾�ŷ�Χ
#define SMQ_PARAM_LOCALE                5   ///<    (get/set smq_uint32)����������Ϣ
/// @}




/// ������ͨ�õ�ֵ����
#if defined(WIN32) || defined(WIN64)
#pragma warning(disable:4200)
#endif
#define SMQ_VALUE_LEN_MAX       (256)   ///<    �����˲�������󳤶�
typedef union
{
    smq_char    value_str[SMQ_VALUE_LEN_MAX];   ///<    �ַ������͵�ֵ
    smq_uint32  value_uint32;                   ///<    uint32���͵�ֵ
    smq_uint16  value_uint16;                   ///<    uint16���͵�ֵ
    smq_uint8   value_uint8;                    ///<    uint8���͵�ֵ
    smq_void*   value_ptr;                      ///<    ָ�����͵�ֵ
    smq_uint32  value_uint32s[0];               ///<    uint32���͵�����
    smq_uint16  value_uint16s[0];               ///<    uint16���͵�����
    smq_uint8   value_uint8s[0];                ///<    uint8���͵�����
    smq_void*   value_ptrs[0];                  ///<    ָ������
}smq_value_t;
#if defined(WIN32) || defined(WIN64)
#pragma warning(default:4200)
#endif
///@}




/// ������������صļ�����
///@{
#define SMQ_LOCALE_ENUS             (0) ///<    Ӣ��
#define SMQ_LOCALE_ZHCN             (1) ///<    ����
///@}




/// ��ɫ����
///@{
#define SMQ_ROLE_MASTER             (0)         ///<    ���ض���
#define SMQ_ROLE_SLAVE              (1)         ///<    �ӿض���
#define SMQ_ROLE_VIEWER             (2)         ///<    �۲��߶���
///@}




/// ��������־����
///@{
#define SMQ_LOG_LEVEL_OFF           (0)         ///<    �ر���־
#define SMQ_LOG_LEVEL_ERROR         (1)         ///<    ���󼶱�
#define SMQ_LOG_LEVEL_WARN          (2)         ///<    ���漶��
#define SMQ_LOG_LEVEL_INFO          (3)         ///<    ��Ϣ����
#define SMQ_LOG_LEVEL_DEBUG         (4)         ///<    ���Լ���
///@}



/// �����˼�����������
///@{
#define SMQ_MAPPING_NAME_LEN_MAX    (100)       ///<    �����ڴ�ӳ�����Ƶ���󳤶�(����\0)
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




/// ������һ��������C���Եı����ڶ��Ժ꣬���ڶԱ��뻷������һЩ�����ļ��
///@{
#if defined(WIN32) || defined(WIN64)
#define SMQ_STATIC_ASSERT(expr,message) C_ASSERT(expr)
#else
#define SMQ_STATIC_ASSERT(expr,message) ((void)sizeof(char[1 - 2*!!(expr)]))
#endif

// SMQ_STATIC_ASSERT((4 == sizeof(smq_uint32)), "smq_uint32���ȱ�����4�ֽ�");
// SMQ_STATIC_ASSERT((2 == sizeof(smq_uint16)), "smq_uint32���ȱ�����4�ֽ�");
// SMQ_STATIC_ASSERT((1 == sizeof(smq_uint8 )), "smq_uint32���ȱ�����4�ֽ�");
// SMQ_STATIC_ASSERT((4 == sizeof(smq_int32 )), "smq_uint32���ȱ�����4�ֽ�");
// SMQ_STATIC_ASSERT((2 == sizeof(smq_int16 )), "smq_uint32���ȱ�����4�ֽ�");
// SMQ_STATIC_ASSERT((1 == sizeof(smq_int8  )), "smq_uint32���ȱ�����4�ֽ�");
// SMQ_STATIC_ASSERT((1 == sizeof(smq_char  )), "smq_uint32���ȱ�����4�ֽ�");
///@}



#endif//_smq_


