cmake_minimum_required(VERSION 3.0)
project(smq)


#   Define some global options
option(JNI "Need JNI support" OFF)
option(BITS64 "Generate 32-bits or 64-bits target" OFF)
option(DEBUG "Generate symbols for debug" OFF)


#   Define some vars, they will be used leter
SET(SRCS smq_errors.c
        smq_inst.c
        smq_logs.c
        smq_msg.c
        smq_os.c
        smq_params.c
        smq_utils.c)
SET(INCS .)
SET(CCDEFS -DSMQ_EXPORTS)


#   Change the vars by the global-options
if (JNI)
    LIST(APPEND SRCS jni/smq_jni.c)
    LIST(APPEND INCS jni)
endif (JNI)

if (BITS64)
endif (BITS64)

if (DEBUG)
    LIST(APPEND CCDEFS -DDEBUG)
else (DEBUG)
    LIST(APPEND CCDEFS -DNDEBUG)
endif (DEBUG)

#   Sprcial the language to c++ for VC
if (defined MSVC)
set_source_files_properties(${SRCS} PROPERTIES LANGUAGE CXX)
endif(defined MSVC)


#   Defined the targets
add_library(smq-shared SHARED ${SRCS})
add_library(smq-static STATIC ${SRCS})


#   Special the include dirs and defines
include_directories(${INCS})
add_definitions(${CCDEFS})




