
#编译目标
TARGET:=../ser

#编译器
CC:=gcc
CXX:=g++

#.c或.cpp文件所在的各个文件夹
SRCS:= $(wildcard src/*.c)
SRCS+= $(wildcard ./*.c)
SRCS+= $(wildcard src/*.cpp)
SRCS+= $(wildcard ./*.cpp)

#.h头文件所在的各个文件夹
INCS:= -I ../include -I ../include/json -I ../include/simpleIni -I ../include/DM

#各个文件夹下的.c和.cpp替换为.o文件
OBJ:=$(SRCS:.c=.o)
OBJS:=$(OBJ:.cpp=.o)

#编译器的一些参数
CFLAGS:= -Wall -std=c++11

#程序所用到的库
LIB_DIRS := -L ../lib
LIBS:= -lpthread -llfh234 -lsmapi -liniparser  -ldmdpi -ljsoncpp -lstdc++

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
	

