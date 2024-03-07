#pragma once

#include <iostream>
#include "data/Packet/Packet.h"
#include <DataInfoBase.h>
#include "dm/sqlGradoperate.h"

#include <DmTableInfo.h>
#include <DataRequest.h>
#include <ftpServerEx.h>

#include "muduo/base/ThreadPool.h"
// 这个类提供一个计算线程池，用来处理计算内容

using namespace muduo;
class CTaskDeal
{

public:
    CTaskDeal(int threadNumer);
    ~CTaskDeal();
    // 开启任务处理线程池
    int Start();
    // 开始处理任务
    int SolveTask(CPacket packet);

    // 设置数据库操作模块
    int SetSqlCtl(CSqlGradOperate *pdbCtl);
    // 设置ftp服务器
    int SetFtpCtl(CftpServerEx *pftpCtl);

private:
    // 执行任务
    void ExcuteTask(CPacket packet);
    // 解密加密的数据
    int DecSm4Data(const std::string &data, std::string &dataOut);
    // 初始化配置文件读对象
    int InitConfigReader();
    // 初始化version和表名的映射
    int InitVerTableName();
    // 将数据存到数据库中
    int SaveDataInDB(const DataInfoBase &data, const std::string &ModelName, const std::string &tableName);

    // //  将查询出来的数据以csv文件的格式存到指定的地方
    // int SavaDataAsCsv(const CDbData &dataAll, std::string fullName);
    // // 任务处理的具体实现
    // int DealTaskThread();
    // // 开启数据备份线程
    // int StartBackup();

    // // 将数据库查询出的数据保存到ftp服务器的指定目录中
    // int SaveDataInFtp(const std::string &tableName, const std::string &startTime, const std::string &EndTime);

private:
    // 计算线程池
    ThreadPool m_threadPool;
    int m_threadNum;

    CSqlGradOperate *m_pdbCtl;
    CftpServerEx *m_pftpCtl;
    std::map<short, std::string> m_mapTableName; // version和表名的映射

    // std::string m_filepath;                      // ftp服务器的目录
    // CSimpleIni m_systemIni;                      // 配置文件读取

    // std::string TimeStart;                       // 查询数据库的初始时间
    // std::string TimeEnd;                         // 查询数据库的截至时间
    // std::string TimeSpan;                        // 查询数据库的时间间隔
};
