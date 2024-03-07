
#编译目标
TARGET:=../ser

#编译器
CC:=gcc
CXX:=g++

# 源文件目录
SRC_DIR := .
MUDUO_INCLUDE  = ../build/release-install-cpp11/include/muduo

#.c或.cpp文件所在的各个文件夹
SRCS = $(wildcard *.c)
SRCS += $(wildcard data/dataBase/*.cpp)
SRCS += $(wildcard data/DataRequest/*.cpp)
#SRCS += $(wildcard data/DMdata/*.cpp)
SRCS += $(wildcard data/Packet/*.cpp)

SRCS += $(wildcard dealTask/*.cpp)

SRCS += $(wildcard dm/*.cpp)
SRCS += $(wildcard dm/DmCtrBase/*.cpp)
SRCS += $(wildcard dm/DmCtrBase/*.c)
SRCS += $(wildcard dm/DMTableInfo/*.cpp)

SRCS += $(wildcard ftp/*.cpp)

SRCS += $(wildcard server/*.cpp)

SRCS += $(wildcard ThreadPool/*.cpp)

SRCS += $(wildcard tools/epoll/*.cpp)
SRCS += $(wildcard tools/simpleIni/*.cpp)
SRCS += $(wildcard tools/simpleIni/*.c)
SRCS += $(wildcard tools/timer/*.cpp)
SRCS += $(wildcard tools/*.cpp)

SRCS += $(wildcard *.cpp)

#.h头文件所在的各个文件夹

INCS := -I ./tools -I $(SRC_DIR) -I $(SRC_DIR)/data/dataBase -I $(SRC_DIR)/data/DataRequest -I $(SRC_DIR)/data/DMdata -I $(SRC_DIR)/data/Packet -I $(SRC_DIR)/dealTask -I $(SRC_DIR)/dm -I $(SRC_DIR)/dm/DMTableInfo -I $(SRC_DIR)/dm/DmCtrBase   -I $(SRC_DIR)/ftp -I $(SRC_DIR)/server -I $(SRC_DIR)/ThreadPool -I $(SRC_DIR)/tools/epoll -I $(SRC_DIR)/tools/json -I $(SRC_DIR)/tools/simpleIni -I $(SRC_DIR)/tools/sm4 -I $(SRC_DIR)/tools/timer -I $(SRC_DIR)/tools  -I ../fineftp-server/fineftp-server/include -I ../fineftp-server/_build/fineftp-server/include -I$(MUDUO_INCLUDE)

#各个文件夹下的.c和.cpp替换为.o文件
OBJ:=$(SRCS:.c=.o)
OBJS:=$(OBJ:.cpp=.o)

#编译器的一些参数
CFLAGS:= -Wall -std=c++17

#程序所用到的库
LIB_DIRS := -L ../lib
LIBS:=  -llfh234 -lsmapi -liniparser  -ldmdpi -ljsoncpp -lfineftp-serverd -lstdc++ -lboost_date_time -lmuduo_net -lmuduo_base -lrt -lpthread

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
	

