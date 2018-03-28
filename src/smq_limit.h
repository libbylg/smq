#ifndef __smq_limit_H_
#define __smq_limit_H_

#include "smq.h"




#define SMQ_FULL_MAPPING_NAME_LEN_MAX           (256)       ///<    ӳ���ڴ�ȫ����󳤶�

#define SMQ_LOG_CACHE_LEN_MAX                   (256)       ///<    ��־�������Ĵ�С



/// ������key�ķ�Χ
///@{
#define SMQ_PARAM_KEY_MIN                       (0)
#define SMQ_PARAM_KEY_MAX                       (9)         ///<    �ɿ��ƵĲ���������10��
///@}



/// ��ɫ
///@{
#define SMQ_ROLE_MIN                            (SMQ_ROLE_MASTER)
#define SMQ_ROLE_MAX                            (SMQ_ROLE_VIEWER)
#define SMQ_ROLE_COUNT                          ((SMQ_ROLE_MAX - SMQ_ROLE_MIN) - 1)
///@}




/// ��������־������صļ�������
///@{
#define SMQ_LOG_LEVEL_MIN                       (SMQ_LOG_LEVEL_DEBUG)   ///<    ��־������Сֵ
#define SMQ_LOG_LEVEL_DEF                       (SMQ_LOG_LEVEL_INFO)    ///<    ȱʡ����־����
#define SMQ_LOG_LEVEL_MAX                       (SMQ_LOG_LEVEL_ERROR)   ///<    ��־�������ֵ
///@}




/// ������ϵͳ����֧�ֵ��ڴ沼�ְ汾�ķ�Χ
///@{
#define SMQ_SUPPORTED_LAYOUT_VERSION_MIN        (0x0001)    ///<    ��ǰϵͳ����֧�ֵ���С�汾��
#define SMQ_SUPPORTED_LAYOUT_VERSION_MAX        (0x0001)    ///<    ��ǰϵͳ����֧�ֵ���С�汾��
///@}




/// �����˵�������֧��
///@{
#define SMQ_LOCALE_MIN                          (0)
#define SMQ_LOCALE_DEF                          (1)
#define SMQ_LOCALE_MAX                          (1)
#define SMQ_LOCALE_COUNT                        ((SMQ_LOCALE_MAX - SMQ_LOCALE_MIN) + 1)
///@}




/// ���ƹ����ڴ���ļ������Ĳ���
///@{
#define SMQ_ALLOC_QUEUES_COUNT                  (4)     ///<    ������еĸ���
#define SMQ_MSSGE_QUEUES_COUNT                  (2)     ///<    ��Ϣ���еĸ���
#define SMQ_MEMORY_BLOCK_SIZE_MAX               (4096)  ///<    ��Ϣ�������С
///@}




/// ��Ϣ�������ò���
///@{
#define SMQ_MESSAGE_QUEUE_SIZE_MIN              (100)
#define SMQ_MESSAGE_QUEUE_SIZE_DEF              (1000)
#define SMQ_MESSAGE_QUEUE_SIZE_MAX              (100000)
///@}




/// �����˹����ڴ��С����
///@{
#define SMQ_MEMORY_SIZE_MIN                     (1)
#define SMQ_MEMORY_SIZE_DEF                     (32)
#define SMQ_MEMORY_SIZE_MAX                     (100)
///@}


/// ��ǰ��֧�ֵ���С�汾
///@{
#define SMQ_VERSION_MIN                         (1)
#define SMQ_VERSION_MAX                         (1)
///@}




/// �����˿��ƹ����ڴ�ؼ��ṹ�ļ�������
///@{
#define SMQ_ALLOC_QUEUES_COUNT                  (4)
#define SMQ_MSSGE_QUEUES_COUNT                  (2)
#define SMQ_ALLOC_BLOCK_SIZE_MAX                (4096)
///@}




/// �����˴�С�˳���
///@{
#define SMQ_BYTE_ENDIAN_BIG                     (1)
#define SMQ_BYTE_ENDIAN_SMALL                   (0)
///@}




/// ������һ��������C���Եľ������Լ���ڶ���쳣����
///@{
#define SMQ_TRY(expr)       expr
#define SMQ_THROW(expr)     expr;       goto    SMQ_EXCEPTION
#define SMQ_THROW_IF(expr)  if (expr)   goto    SMQ_EXCEPTION
#define SMQ_CATCH(expr)     SMQ_EXCEPTION:  if (expr)
///@}


#endif

