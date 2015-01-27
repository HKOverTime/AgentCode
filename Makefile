DEFS = -D_DEBUG_PRINT_
DEBUG_FLAGS += \
	$(DEFS)

# 基础库代码文件的位置
LIB_SRCS = ./Serial/Serial_Base_API.c \
			./CMD_Protocol/CMDProtocol.c
# agent 相关代码文件位置
AGENT_SRC = agent.c
# client 相关文件位置
HOST_CON_SRC = client.c
# 最终产品文件名
PRODUCT = agent agent.exe
HOST_PRODUCT = host_client host_client.exe


LIB_DIRS = $(dir $(LIB_SRCS))
LIB_INCS = $(foreach n,$(LIB_DIRS),-I$(n))
LIB_OBJS = $(foreach n,$(LIB_SRCS:.c=.o),`basename $(n)`)

AGENT_DIR = $(dir $(AGENT_SRC))
AGENT_INC = $(foreach n,$(AGENT_DIR),-I$(n))
AGENT_OBJ = $(foreach n,$(AGENT_SRC:.c=.o),`basename $(n)`)

HOST_CON_DIR = $(dir $(HOST_CON_SRC))
HOST_CON_INC = $(foreach n,$(HOST_CON_DIR),-I$(n))
HOST_CON_OBJ = $(HOST_CON_SRC:.c=.o)


INCS   = $(LIB_INCS) $(AGENT_INC)
SRCS   = $(LIB_SRCS) $(AGENT_SRC)
TARGET = $(LIB_OBJS) $(AGENT_OBJ)

HOST_INCS   = $(LIB_INCS) $(HOST_CON_INC)
HOST_SRCS   = $(LIB_SRCS) $(HOST_CON_SRC)
HOST_TARGET = $(LIB_OBJS) $(HOST_CON_OBJ)

$(TARGET) : $(SRCS)
	cc $(DEBUG_FLAGS) $(INCS) -c $(SRCS) 
	cc $(DEBUG_FLAGS) $(INCS) -o agent $(TARGET) 
	rm $(TARGET)
pro  : $(PRO_SRCS)
	cc $(INCS) -c $(SRCS) 
	cc $(INCS) -o agent $(TARGET) 
	rm $(TARGET)
client : $(HOST_SRCS)
	cc $(DEBUG_FLAGS) $(HOST_INCS) -c $(HOST_SRCS) 
	cc $(DEBUG_FLAGS) $(HOST_INCS) -o host_client $(HOST_TARGET) 
	rm $(HOST_TARGET)
clean:
	rm $(PRODUCT) $(HOST_PRODUCT) $(TARGET) $(HOST_TARGET)
