#make 文件

-include vars.mk
F_TYPE =cpp
CXX = g++
CXX_LIBS = -lpthread
CXX_TAGS = -std=c++11
#define sh cmd
TARGET = xlib
MKDIR = mkdir
RM = rm -f
RM_RF = rm -rf
MV = mv 

#定义路径
DIR = $(shell pwd)
SRC_DIR = $(DIR)
OBJ_DIR = $(DIR)/obj
DIRS = $(OBJ_DIR)

define walk
	$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef


ALL_FILES := $(call walk, $(SRC_DIR))
SRCS:=$(filter %.$(F_TYPE),$(ALL_FILES))
OBJS:=$(patsubst %.$(F_TYPE),%.o,$(SRCS))
#OBJS:=$(patsubst %.$(F_TYPE),$(OBJ_DIR)/%.o,$(notdir $(SRCS)))
#OBJS_PRE:=$(subprefix $(SRC_DIR),$(OBJS))
all:$(TARGET) $(DIRS) 

$(DIRS):
	@$(MKDIR) $(DIRS) 
$(TARGET):$(OBJS)
	@$(CXX) -o $@ $^ $(CXX_TAGS) $(CXX_LIBS)
%.o:%.$(F_TYPE)
	$(CXX) -c -o $@ $< $(CXX_TAGS) $(CXX_LIBS)
.PHONY:clean
clean:
	@$(RM) $(TARGET) $(OBJS) && $(RM_RF) $(DIRS)
.PHONY:clean-o
clean-o:
	@$(RM) $(OBJS)
