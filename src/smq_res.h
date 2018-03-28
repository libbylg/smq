//SMQ_ERR(name,id,cn,en)
//SMQ_LOG(name,id,cn,en)
SMQ_ERR(SMQ_ERR_LOWER_LIMIT,                    200,    "(占位)",                                     "")
SMQ_ERR(SMQ_ERR_MALLOC_FAILED,                  201,    "初始化smq实例失败，分配内存失败",                "")
SMQ_ERR(SMQ_ERR_UNDEFINED_ERR,                  202,    "错误码未定义",                                 "")
SMQ_ERR(SMQ_ERR_UNDEFINED_LOC,                  203,    "本地语言类型未定义",                            "")
SMQ_ERR(SMQ_ERR_INIT_OPEN_OR_CREATE_SHM_FAILED, 204,    "新建或者打开共享内存失败",                      "")
SMQ_ERR(SMQ_ERR_INIT_LAYOUT_FAILED,             205,    "初始化共享内存的内存布局失败",                   "")
SMQ_ERR(SMQ_ERR_PARAM_UNSUPPORTED_KEY,          206,    "定制参数的key当前不支持",                       "")
SMQ_ERR(SMQ_ERR_PARAM_UNSUPPORTED_GET_KEY,      207,    "指定key的参数不支持获取操作",                   "")
SMQ_ERR(SMQ_ERR_PARAM_UNSUPPORTED_SET_KEY,      208,    "指定key的参数不支持设置操作",                   "")
SMQ_ERR(SMQ_ERR_LOG_LEVEL_OUT_OF_RANGE,         209,    "日志级别超出范围",                             "")
SMQ_ERR(SMQ_ERR_MEMORY_SIZE_OUT_OF_RANGE,       210,    "共享内存大小超出范围",                          "")
SMQ_ERR(SMQ_ERR_MESSAGE_QUEUE_SIZE_OUT_OF_RANGE,211,    "消息队列的大小超出范围",                        "")
SMQ_ERR(SMQ_ERR_LOCALE_OUT_OF_RANGE,            212,    "本地语言的值超出支持的范围",                    "")
SMQ_ERR(SMQ_ERR_LOG_FUNCTION_NULL,              213,    "日志函数不应该为NULL",                         "")
SMQ_ERR(SMQ_ERR_LEN_LARGE_THAN_CAP,             214,    "数据长度超过了消息块的内存限制",                 "")





SMQ_LOG(SMQ_LOG_LOWER_LIMIT,                    1000,   "",                                            "")
SMQ_LOG(SMQ_LOG_INIT_UNLOCK_FAILED,             1001,   "初始化完成之后，解锁全局锁失败",                  "")


