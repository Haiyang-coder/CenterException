#pragma once

#include <iostream>
#include "./socket/SeverSocket.h"
#include "./data/Packet.h"
#include "./data/RequestJson.h"
#include "./dm/sqlGradoperate.h"
#include "./tools/LogPrint.h"
#include "./tools/ThreadSafeQueue.h"
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
#include "./data/DataQuery.h"
#include "./simpleIni/SimpleIni.h"
#define D_WORK_THREADS_MIN_NUM 5  // 最少工作线程数
#define D_WORK_THREADS_MAX_NUM 15 // 最大工作线程数

/// @brief 处理任务需要的参数
struct DataTask
{
    CPacket pack; // 数据
    int fd;       // 通信套接字，用这个套接字给对方发送消息
    DataTask(CPacket packet, int fd)
    {
        pack = packet;
        this->fd = fd;
    }
};
class CDealTask
{

public:
    CDealTask(CThreadSafeQueue<dataInQueue> *taskQueue);
    ~CDealTask();
    // 开启任务处理线程
    int StartDealTask();
    // 设置用到的网络通信模块
    int SetSocket(CSeverSocket *socket);
    // 设置数据库操作模块
    int SetSqlCtl(CSqlGradOperate *pdbCtl);

private:
    // 任务处理的具体实现
    int DealTaskThread();
    // 开启数据备份线程
    int StartBackup();
    // 解密加密的数据
    int DecSm4Data(const std::string &data, std::string &dataOut);
    // 将数据存到数据库中
    int SaveDataInDB(const DataInfoBase &data);
    // 将数据库查询出的数据保存到ftp服务器的指定目录中
    int SaveDataInFtp(const std::string &tableName, const std::string &startTime, const std::string &EndTime);
    // 初始化配置文件读对象
    int InitConfigReader();
    // 初始化version和表名的映射
    int InitVerTableName();

private:
    CThreadPool *m_pthreadPool;                  // 线程池
    std::thread m_thread;                        // 任务处理的线程
    CThreadSafeQueue<dataInQueue> *m_ptaskQueue; // 要处理的任务队列
    CSqlGradOperate *m_pdbCtl;                   // 对数据库进行操作
    CSeverSocket *m_psocket;                     // 用来发送数据
    std::string m_filepath;                      // ftp服务器的目录
    CSimpleIni m_systemIni;                      // 配置文件读取
    std::map<short, std::string> m_mapTableName; // version和表名的映射
};
