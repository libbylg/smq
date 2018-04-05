
PROJ_ROOT=@{PROJ_ROOT}
VERION=@{VERSION}
COMPILER=@{CC}
COMPILER_OPTS=@{CC_OPTS}
LINKER=@{LINKER}
LINKER_OPTS=@{LINKER_OPTS}
AR=@{AR}
AR_OPTS=@{AR_OPTS}
OPTIONAL_SRCS=@{OPTIONAL_SRCS}
STATIC_LIB_NAME=@{STATIC_LIB_NAME}
DYNAMIC_LIB_NAME=@{DYNAMIC_LIB_NAME}
PACKAGE_NAME=@{PACKAGE_NAME}
OUTPUT_DIR=@{OUTPUT_DIR}
ENTRY_TARGETS=@{ENTRY_TARGETS}


#=============================================

STATIC_LIB=$(OUTPUT_DIR)/$(STATIC_LIB_NAME)

DYNAMIC_LIB=$(OUTPUT_DIR)/$(DYNAMIC_LIB_NAME)

PACKAGE=$(OUTPUT_DIR)/@{PACKAGE_NAME}

SRCS=\
    $(PROJ_ROOT)/smq_errors.c   \
    $(PROJ_ROOT)/smq_inst.c     \
    $(PROJ_ROOT)/smq_logs.c     \
    $(PROJ_ROOT)/smq_msg.c      \
    $(PROJ_ROOT)/smq_os.c       \
    $(PROJ_ROOT)/smq_params.c   \
    $(PROJ_ROOT)/smq_utils.c    \
    $(OPTIONAL_SRCS)

OBJS=$(SRC:.c=.o)



go:$(ENTRY_TARGETS)

#=============================================
go:$(ENTRY_TARGETS)

clean:${PACKAGE} $(STATIC_LIB) $(DYNAMIC_LIB) ${OBJS}
	rm  -rf  @^

build:$(STATIC_LIB) $(DYNAMIC_LIB)
	

package:$(STATIC_LIB) $(DYNAMIC_LIB)
	

#   Create the static library
$(STATIC_LIB):$(OBJS)
	$(AR)   -o  $(STATIC_LIB)


#   Create the dynamic library
$(DYNAMIC_LIB):$(OBJS)
	$(AR)   -o  $(STATIC_LIB)




#   Create the object files
.c.o：
	$(CC)  $(CC_OPTS)  $@  -o  $<

	