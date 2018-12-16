#make 文件

-include vars.mk

TARGET = xlib.a
AR = ar
# compile msg
SRC_EXT = cpp

CXX ?= g++
CXX_LIBS = -pthread
CXX_TAGS = -std=c++11 -Wall -Wextra -g
R_TAGS = -D NDEBUG
D_TAGS = -D DEBUG

#define sh cmd

MKDIR = mkdir -p
RM = rm -f
RM_RF = rm -rf
MV = mv 
print-%:
	 @echo $*=$($*)

#定义路径
DIR_EXT = /
DIR = .
SRC_PATH = $(DIR)/
OBJ_PATH = $(DIR)/obj
DIRS = $(OBJ_PATH)
INCLUDES_CXX = -I $(SRC_PATH)/src
INCLUDES_H = -I $(SRC_PATH)/include
#define system
UNAME_S:=$(shell uname -s)
rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard,$d/,$2) \
						$(filter $(subst *,%,$2), $d))

#macros for timing compilation
ifeq ($(UNAME_S),Darwin)
	CUR_TIME = awk 'BEGIN{srand(); print srand()}'
	TIME_FILE = $(dir $@).$(notdir $@)_time
	START_TIME = $(CUR_TIME) > $(TIME_FILE)
	END_TIME = read st < $(TIME_FILE) ; \
		$(RM) $(TIME_FILE) ; \
		st=$$((`$(CUR_TIME)` - $$st)) ; \
		echo $$st
else
	TIME_FILE = $(dir $@).$(notdir $@)_time
	START_TIME = date '+%s' > $(TIME_FILE)
	END_TIME = read st < $(TIME_FILE) ; \
		$(RM) $(TIME_FILE) ; \
		st=$$((`date '+%s'` - $$st - 86400)) ; \
		echo `date -u -d @$$st '+%H:%M:%S'`
endif

#define export 
export V:= false
export CMD_PRE := @
ifeq ($V),true)
	CMD_PRE :=
endif

SRCS := $(call rwildcard, $(SRC_PATH),*.$(SRC_EXT))
#OBJS:=$(patsubst %.$(SRC_EXT),%.o,$(SRCS))
OBJS = $(SRCS:$(SRC_PATH)%.$(SRC_EXT)=$(OBJ_PATH)/%.o)
DEPS = $(patsubst %.o,%.d,$(OBJS))
all:$(TARGET)
	@echo "build successed!"
$(TARGET):$(OBJS)
	@echo 'Linking: $@'
	@$(START_TIME)
	@$(AR) -rcus $@ $^
	@$(RM) $(OBJS) && $(RM_RF) $(DIRS)
	@$(MKDIR) lib && $(MV) $(TARGET) lib/$(TARGET)
	@echo -en 'Link time:'
	@$(END_TIME)
-include $(DEPS)

$(OBJ_PATH)/%.o:$(SRC_PATH)%.$(SRC_EXT)
	@$(MKDIR) $(dir $(OBJS))
	@echo 'Compiling: $< --> $@' 
	@$(START_TIME)
	@$(CXX) $(CXX_TAGS) $(CXX_LIBS) $(INCLUDES_CXX) $(INCLUDES_H) -MP -MMD -c -o $@ $^
	@echo -en 'Compile time: '
	@$(END_TIME)
clean:
	@$(RM) $(TARGET) $(OBJS) && $(RM_RF) $(DIRS)
cleanobj:
	@$(RM) $(OBJS)
.PHONY:debug clean cleanobj
