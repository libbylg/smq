#ifndef __smq_limit_H_
#define __smq_limit_H_

#include "smq.h"




#define SMQ_FULL_MAPPING_NAME_LEN_MAX           (256)       ///<    映射内存全名最大长度

#define SMQ_LOG_CACHE_LEN_MAX                   (256)       ///<    日志缓冲区的大小



/// 参数的key的范围
///@{
#define SMQ_PARAM_KEY_MIN                       (0)
#define SMQ_PARAM_KEY_MAX                       (9)         ///<    可控制的参数不超过10个
///@}



/// 角色
///@{
#define SMQ_ROLE_MIN                            (SMQ_ROLE_MASTER)
#define SMQ_ROLE_MAX                            (SMQ_ROLE_VIEWER)
#define SMQ_ROLE_COUNT                          ((SMQ_ROLE_MAX - SMQ_ROLE_MIN) - 1)
///@}




/// 定义了日志级别相关的几个参数
///@{
#define SMQ_LOG_LEVEL_MIN                       (SMQ_LOG_LEVEL_DEBUG)   ///<    日志级别最小值
#define SMQ_LOG_LEVEL_DEF                       (SMQ_LOG_LEVEL_INFO)    ///<    缺省的日志级别
#define SMQ_LOG_LEVEL_MAX                       (SMQ_LOG_LEVEL_ERROR)   ///<    日志级别最大值
///@}




/// 定义了系统所能支持的内存布局版本的范围
///@{
#define SMQ_SUPPORTED_LAYOUT_VERSION_MIN        (0x0001)    ///<    当前系统所能支持的最小版本号
#define SMQ_SUPPORTED_LAYOUT_VERSION_MAX        (0x0001)    ///<    当前系统所能支持的最小版本号
///@}




/// 定义了当地语言支持
///@{
#define SMQ_LOCALE_MIN                          (0)
#define SMQ_LOCALE_DEF                          (1)
#define SMQ_LOCALE_MAX                          (1)
#define SMQ_LOCALE_COUNT                        ((SMQ_LOCALE_MAX - SMQ_LOCALE_MIN) + 1)
///@}




/// 控制共享内存规格的几个核心参数
///@{
#define SMQ_ALLOC_QUEUES_COUNT                  (4)     ///<    分配队列的个数
#define SMQ_MSSGE_QUEUES_COUNT                  (2)     ///<    消息队列的个数
#define SMQ_MEMORY_BLOCK_SIZE_MAX               (4096)  ///<    消息块的最大大小
///@}




/// 消息队列配置参数
///@{
#define SMQ_MESSAGE_QUEUE_SIZE_MIN              (100)
#define SMQ_MESSAGE_QUEUE_SIZE_DEF              (1000)
#define SMQ_MESSAGE_QUEUE_SIZE_MAX              (100000)
///@}




/// 定义了共享内存大小配置
///@{
#define SMQ_MEMORY_SIZE_MIN                     (1)
#define SMQ_MEMORY_SIZE_DEF                     (32)
#define SMQ_MEMORY_SIZE_MAX                     (100)
///@}


/// 当前所支持的最小版本
///@{
#define SMQ_VERSION_MIN                         (1)
#define SMQ_VERSION_MAX                         (1)
///@}




/// 定义了控制共享内存关键结构的几个参数
///@{
#define SMQ_ALLOC_QUEUES_COUNT                  (4)
#define SMQ_MSSGE_QUEUES_COUNT                  (2)
#define SMQ_ALLOC_BLOCK_SIZE_MAX                (4096)
///@}




/// 定义了大小端常量
///@{
#define SMQ_BYTE_ENDIAN_BIG                     (1)
#define SMQ_BYTE_ENDIAN_SMALL                   (0)
///@}




/// 定义了一种适用于C语言的精简版且约束众多的异常机制
///@{
#define SMQ_TRY(expr)       expr
#define SMQ_THROW(expr)     expr;       goto    SMQ_EXCEPTION
#define SMQ_THROW_IF(expr)  if (expr)   goto    SMQ_EXCEPTION
#define SMQ_CATCH(expr)     SMQ_EXCEPTION:  if (expr)
///@}


#endif

