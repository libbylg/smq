//SMQ_ERR(name,id,cn,en)
//SMQ_LOG(name,id,cn,en)
SMQ_ERR(SMQ_ERR_MALLOC_FAILED,                  200,    "初始化smq实例失败，分配内存失败",                "")
SMQ_ERR(SMQ_ERR_UNDEFINED_ERR,                  201,    "错误码未定义",                                 "")
SMQ_ERR(SMQ_ERR_UNDEFINED_LOC,                  202,    "本地语言类型未定义",                            "")
SMQ_ERR(SMQ_ERR_INIT_OPEN_OR_CREATE_SHM_FAILED, 203,    "新建或者打开共享内存失败",                      "")
SMQ_ERR(SMQ_ERR_INIT_LAYOUT_FAILED,             204,    "初始化共享内存的内存布局失败",                   "")

SMQ_LOG(SMQ_LOG_INIT_UNLOCK_FAILED,             1000,   "初始化完成之后，解锁全局锁失败",                  "")


