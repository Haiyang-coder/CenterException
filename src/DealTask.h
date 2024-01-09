#pragma once

#include "SimpleIni.h"
#include <iostream>
#include "./data/Packet.h"
#include "./data/RequestJson.h"
#include "sqlGradoperate.h"
#include "LogPrint.h"
#include "ThreadSafeQueue.h"
#include "./ThreadPool/ThreadPool.h"
#include "./data/DelData.h"
#include "./data/DelCopyData.h"
#include "./data/NoticData.h"
#include "./data/NoticAckData.h"
#include "./data/UnCpareDelData.h"
#include "./data/UnTrigerDelData.h"
#include "./data/UnNotificationCfirm.h"
#include "./data/ConsistData.h"
#include "./data/AlgStandardDiff.h"
#include "./data/UnRecoverable.h"
#include "./data/UnEfficaAlg.h"
#include "./data/DelDupComple.h"
#include "./data/CmpliDelFail.h"
#define D_WORK_THREADS_MIN_NUM 5  // 最少工作线程数
#define D_WORK_THREADS_MAX_NUM 15 // 最大工作线程数

/// @brief 处理任务需要的参数
struct DataTask
{
    CPacket pack; // 数据
    DataTask(CPacket packet)
    {
        pack = packet;
    }
};
class CDealTask
{

public:
    CDealTask(CThreadSafeQueue<CPacket> *taskQueue);
    ~CDealTask();
    int StartDealTask();
    void DealTask();
    int DealTaskThread();

private:
    // 解密加密的数据
    int DecSm4Data(const std::string &data, std::string &dataOut);
    // 将数据存到数据库中
    int SaveDataInDB(const DataInfoBase &data);

private:
    CThreadPool *m_pthreadPool;              // 线程池
    std::thread m_thread;                    // 队列管理线程，如果队列不满就会分配任务
    CThreadSafeQueue<CPacket> *m_ptaskQueue; // 要处理的任务队列
};
