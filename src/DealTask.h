#pragma once

#include "SimpleIni.h"
#include <iostream>
#include "Packet.h"
#include "RequestJson.h"
#include "sqlGradoperate.h"
#include "LogPrint.h"
#include"ThreadSafeQueue.h"
#include"ThreadPool.h"
#define D_WORK_THREADS_MIN_NUM 5 //最少工作线程数
#define D_WORK_THREADS_MAX_NUM 15//最大工作线程数


/// @brief 处理任务需要的参数
struct DataTask
{
    CPacket pack;//数据
    DataTask(CPacket packet)
    {
        pack = packet;
    }
};
class CDealTask
{

public:
    CDealTask(CThreadSafeQueue<CPacket>* taskQueue);
    ~CDealTask();
    int StartDealTask();
    void DealTask();
    int DealTaskThread();
    //接收连接的入口线程
	static void* DealTask(void* arg);

private:
    // 解密加密的数据
    int DecSm4Data(const std::string &data, std::string &dataOut);
    // 将数据存到数据库中
    int SaveDataInDB(const DataInfoBase &data);
private:
    CThreadPool* m_pthreadPool;//线程池
    std::thread m_thread;//队列管理线程，如果队列不满就会分配任务
    CThreadSafeQueue<CPacket>* m_ptaskQueue;//要处理的任务队列
};
