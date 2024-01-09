
#编译目标
TARGET:=../ser

#编译器
CC:=gcc
CXX:=g++

#.c或.cpp文件所在的各个文件夹
SRCS := $(wildcard *.c)
SRCS += $(wildcard data/*.cpp)
SRCS += $(wildcard *.cpp)
SRCS += $(wildcard ThreadPool/*.cpp)

#.h头文件所在的各个文件夹
INCS:= -I ../include -I ../include/json -I ../include/simpleIni -I ../include/DM -I /root/CenterExceptional/fineftp-server/fineftp-server/include -I /root/CenterExceptional/fineftp-server/_build/fineftp-server/include

#各个文件夹下的.c和.cpp替换为.o文件
OBJ:=$(SRCS:.c=.o)
OBJS:=$(OBJ:.cpp=.o)

#编译器的一些参数
CFLAGS:= -Wall -std=c++17

#程序所用到的库
LIB_DIRS := -L ../lib
LIBS:= -lpthread -llfh234 -lsmapi -liniparser  -ldmdpi -ljsoncpp -lfineftp-serverd -lstdc++

all: $(TARGET)
%.o: %.c
	$(CC) -g -c  $< $(INCS) -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -g -c $< $(INCS) -o $@ $(CFLAGS)
	
$(TARGET): $(OBJS)
	$(info target:$@)
	$(info all:$^)
	$(info first:$<)
	
	$(CXX) -o $@ $^ $(LIB_DIRS) $(LIBS)
	rm -f $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)
	

