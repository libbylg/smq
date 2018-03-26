//SMQ_ERR(name,id,cn,en)
//SMQ_LOG(name,id,cn,en)
SMQ_ERR(SMQ_ERR_MALLOC_FAILED,                  200,    "初始化smq实例失败，分配内存失败",                "")
SMQ_ERR(SMQ_ERR_UNDEFINED_ERR,                  201,    "错误码未定义",                                 "")
SMQ_ERR(SMQ_ERR_UNDEFINED_LOC,                  202,    "本地语言类型未定义",                            "")
SMQ_ERR(SMQ_ERR_INIT_OPEN_OR_CREATE_SHM_FAILED, 203,    "新建或者打开共享内存失败",                      "")
SMQ_ERR(SMQ_ERR_INIT_LAYOUT_FAILED,             204,    "初始化共享内存的内存布局失败",                   "")
SMQ_ERR(SMQ_ERR_PARAM_UNSUPPORTED_KEY,          205,    "定制参数的key当前不支持",                       "")
SMQ_ERR(SMQ_ERR_PARAM_UNSUPPORTED_GET_KEY,      206,    "指定key的参数不支持获取操作",                   "")
SMQ_ERR(SMQ_ERR_PARAM_UNSUPPORTED_SET_KEY,      207,    "指定key的参数不支持设置操作",                   "")
SMQ_ERR(SMQ_ERR_LOG_LEVEL_OUT_OF_RANGE,         208,    "日志级别超出范围",                             "")
SMQ_ERR(SMQ_ERR_MEMORY_SIZE_OUT_OF_RANGE,       209,    "共享内存大小超出范围",                          "")
SMQ_ERR(SMQ_ERR_MESSAGE_QUEUE_SIZE_OUT_OF_RANGE,210,    "消息队列的大小超出范围",                        "")
SMQ_ERR(SMQ_ERR_LOCALE_OUT_OF_RANGE,            211,    "本地语言的值超出支持的范围",                    "")
SMQ_ERR(SMQ_ERR_LOG_FUNCTION_NULL,              212,    "日志函数不应该为NULL",                         "")







SMQ_LOG(SMQ_LOG_INIT_UNLOCK_FAILED,             1000,   "初始化完成之后，解锁全局锁失败",                  "")


