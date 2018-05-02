CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(smq)


# Include the sub directorys to build
ADD_SUBDIRECTORY(examples)


#   Define some global options
OPTION(JNI "Need JNI support" OFF)
OPTION(DEBUG "Generate symbols for debug" OFF)


#   All headers
SET(HRDS smq_alignpop.h
        smq_alignpush.h
        smq_errors.h
        smq_inst.h
        smq_libc.h
        smq_limit.h
        smq_logs.h
        smq_os.h
        smq_params.h
        smq_res.h
        smq_utils.h
	smq.h)
SET_SOURCE_FILES_PROPERTIES(${HRDS} PROPERTIES HEADER_FILE_ONLY YES)



#   Define some vars, they will be used leter
SET(SRCS smq_errors.c
        smq_inst.c
        smq_logs.c
        smq_msg.c
        smq_os.c
        smq_params.c
        smq_utils.c
        ${HRDS}
        )
SET(INCS .)
SET(CCDEFS -DSMQ_EXPORTS)



#   Change the vars by the global-options
IF    (JNI)
    LIST(APPEND SRCS jni/smq_jni.c)
    LIST(APPEND INCS jni)
ENDIF (JNI)


#   Need the debug symbols or not
IF    (DEBUG)
    LIST(APPEND CCDEFS -DDEBUG)
ELSE  (DEBUG)
    LIST(APPEND CCDEFS -DNDEBUG)
ENDIF (DEBUG)


#   Sprcial the language to c++ for VC
IF   (MSVC)
    SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES LANGUAGE CXX)
    LIST(APPEND SRCS os/win.msvc/smq.def)
ENDIF(MSVC)


#   Special the include dirs and defines
INCLUDE_DIRECTORIES(${INCS})
ADD_DEFINITIONS(${CCDEFS})


#   Defined the targets
ADD_LIBRARY(smq        SHARED ${SRCS})
ADD_LIBRARY(smq-static STATIC ${SRCS})


#   Set the output dirs
SET_TARGET_PROPERTIES(smq        PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/out)
SET_TARGET_PROPERTIES(smq        PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/out)
SET_TARGET_PROPERTIES(smq        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/out)
SET_TARGET_PROPERTIES(smq        PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/out)
SET_TARGET_PROPERTIES(smq        PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/out)
SET_TARGET_PROPERTIES(smq        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/out)
SET_TARGET_PROPERTIES(smq-static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/out)
SET_TARGET_PROPERTIES(smq-static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/out)
