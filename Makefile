
CROSS_COMPILE = #交叉编译工具链前缀——arm-linux-gnueabihf-
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -O2 -g

CFLAGS += -I $(shell pwd)/include#头文件路径——当前为该目录下的include文件夹

LDFLAGS = #初始化链接器配置变量，用于指定库路径、链接库等选项

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

BUILD_DIR := build
export BUILD_DIR

TARGET := target#链接成为的可执行文件名称

# 要编译的目录或根目录
#obj-y += main.o
obj-y += source/main.o
obj-y += source/ini.o


all : start_recursive_build $(TARGET)
	@echo $(TARGET) has been built!

start_recursive_build: | $(BUILD_DIR)
	make -C ./ -f $(TOPDIR)/Makefile.build

$(TARGET) : start_recursive_build
	$(CC) -o $@ $(BUILD_DIR)/built-in.o $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)
	