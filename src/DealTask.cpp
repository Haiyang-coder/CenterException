#include "DealTask.h"
CDealTask::CDealTask(CThreadSafeQueue<dataInQueue> *taskQueue) : m_ptaskQueue(taskQueue)
{
    m_pthreadPool = &CThreadPool::getInstance();
    int ret = InitConfigReader();
    if (ret < 0)
    {
        exit(ret);
    }
    m_filepath = m_systemIni.GetValue("ftp", "CSV_PATH", "/dm8/test");
    InitVerTableName();
}

CDealTask::~CDealTask()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

/// @brief 开始处理任务队列中的任务
/// @return
int CDealTask::StartDealTask()
{
    // RAII开启一个线程
    m_thread = std::thread(&CDealTask::DealTaskThread, this);
    return 0;
}

/// @brief 按照规定的时间将规定时间端内的数据放到指定的路径下
/// @return
int CDealTask::StartBackup()
{

    return 0;
}

int CDealTask::DealTaskThread()
{
    while (!m_ptaskQueue->isDestroyed())
    {
        std::cout << "开始执行任务" << std::endl;
        dataInQueue data = m_ptaskQueue->pop();
        CPacket pack = data.pack;
        int sock_fd = data.recv_fd;
        // 取出了任务，将任务交给线程池来完成还是一个线程直接处理？我觉得一个就行了
        // 1. 解密数据
        // 解密后的数据
        std::string strDataDec;
        int ret = DecSm4Data(pack.m_strData, strDataDec);
        if (ret < 0)
        {
            PRINT_ERROR(ret, "DecSm4Data 失败");
        }
        // 2. 将解密的数据转换成类
        switch (pack.m_msgVersion)
        {
        case 0x4120: // 确定性删除系统
        {
            CDelData jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4121: // 删除异常数据拷贝
        {
            CDelCopyData jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4020: // 删除指令通知与确认系统
        {
            CNoticData jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4021: // 删除指令通知与确认系统
        {
            CNotifyAckData jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4221: // 未按删除意图对信息进行对照删除异常数据存证信息
        {
            CUnCpareDelData jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4222: // 未按删除触发条件对信息进行删除异常数据存证信息
        {
            CUnTrigerDelData jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4223: // 删除通知与确认完备性异常数据存证信息
        {
            UnNotificationCfirm jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4224: // 删除一致性无效异常数据存证信息
        {
            CConsistData jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4225: // 删除方法合规性异常数据存证信息
        {
            CAlgStandardDiff jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4226: // 删除副本不可恢复性异常数据存证信息
        {
            UnRecoverable jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4227: // 删除算法失效异常数据存证信息
        {
            UnEfficaAlg jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4228: // 副本删除完备性异常数据存证信息
        {
            DelDupComple jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4229: // 整体删除无效异常数据存证信息
        {
            CmpliDelFail jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            ret = SaveDataInDB(jsonData);
        }
        case 0x4300: // 定时备份数据
        {
            CDataQuery jsonData(strDataDec.c_str());
            ret = jsonData.TurnStr2Obj(strDataDec.c_str());
            if (ret != 0)
            {
                PRINT_ERROR(ret, "CDataQuery,转换失败");
                return ret;
            }
            auto it = m_mapTableName.find(jsonData.tableVersion);
            if (it == m_mapTableName.end())
            {
                PRINT_ERROR(ret, "没有你想要查询的数据");
                ret = -1;
            }
            else
            {
                SaveDataInFtp(it->second, jsonData.subtimeStart, jsonData.subtimeEnd);
            }
        }
        default:
            LOG(LOG_ERROR, "不清楚的包来源和指令");
            break;
        }
    }
    return 0;
}

/// @brief 设置通信模块
/// @param socket
/// @return
int CDealTask::SetSocket(CSeverSocket *socket)
{
    if (socket != NULL)
    {
        m_psocket = socket;
        return 0;
    }
    return -1;
}

/// @brief 设置数据库操作模块
/// @param pdbCtl
/// @return
int CDealTask::SetSqlCtl(CSqlGradOperate *pdbCtl)
{

    if (socket != NULL)
    {
        m_pdbCtl = pdbCtl;
        return 0;
    }
    return -1;
}

/// @brief 将数据解密传出
/// @param dataIn in 需要解密的数据
/// @param dataOut out 解密完成的数据
/// @return 0：成功
int CDealTask::DecSm4Data(const std::string &dataIn, std::string &dataOut)
{
    // todo :解密数据
    return 0;
}

/// @brief
/// @param data
/// @return
int CDealTask::SaveDataInDB(const DataInfoBase &data)
{
    int ret = 0;
    ret = m_pdbCtl->InitDMShemaAndTable();
    if (ret < 0)
    {
        PRINT_ERROR(ret, "数据库初始化出现问题 失败");
        return ret;
    }
    std::string sql;
    data.GetInserDataInOrder(sql);
    ret = m_pdbCtl->DataInsert(sql.c_str());
    if (ret < 0)
    {
        PRINT_ERROR(ret, "数据库插入语句执行错误 失败");
        return ret;
    }
    return 0;
}
/// @brief 将客户端要查询的数据放到ftp服务器指定目录上
/// @param tableName
/// @param startTime
/// @param EndTime
/// @return
int CDealTask::SaveDataInFtp(const std::string &tableName, const std::string &startTime, const std::string &EndTime)
{
    std::string fileName = tableName + ":" + startTime + "-" + EndTime;
    // 要保存文件的完整路径名
    std::string fullNanme = m_filepath + "/" + fileName;
    CDbData dataAll;
    m_pdbCtl->GetDataBySubmitTime(tableName.c_str(), startTime.c_str(), EndTime.c_str(), dataAll);
    m_pdbCtl->SavaDataAsCsv(dataAll, fullNanme);
    return 0;
}

/// @brief 初始化读配置文件的对象
/// @return
int CDealTask::InitConfigReader()
{
    // 定义ini文档对象
    // 参数为true时保存为UTF-8格式，否则为本地编码格式
    m_systemIni.SetUnicode(true);
    // 是否允许一个关键字对应多个值，默认为允许；若不允许，则将最后一个值作为此关键字关联的值
    m_systemIni.SetMultiKey(false);
    SI_Error ret = 0;
    ret = m_systemIni.LoadFile(SYSTEM_INI_FILE_PATH);
    if (ret < 0)
    {
        std::cout << "加载文件：" << SYSTEM_INI_FILE_PATH << "失败了" << std::endl;
        return ret;
    }
}

/// @brief 构建表名和version的映射关系
/// @return
int CDealTask::InitVerTableName()
{
    m_mapTableName[0x4120] = "delete_UnOrder";
    m_mapTableName[0x4121] = "delete_DupFail";
    m_mapTableName[0x4020] = "delete_NotificationConfirmationDiff";
    m_mapTableName[0x4021] = "delete_NotifyAckError";
    m_mapTableName[0x4221] = "delete_IntentiongRequestDiff";
    m_mapTableName[0x4222] = "delete_RequestTriggerDiff";
    m_mapTableName[0x4223] = "delete_NotificationConfirmationDiff";
    m_mapTableName[0x4224] = "delete_ConsistencyDiff";
    m_mapTableName[0x4225] = "delete_AlgStandardDiff";
    m_mapTableName[0x4226] = "delete_AlgStandardIrrecoverableDiff";
    m_mapTableName[0x4227] = "delete_AlgRetentionStatusDiff";
    m_mapTableName[0x4228] = "delete_DupCompletenessDiff";
    m_mapTableName[0x4229] = "delete_EffectEvaDomainSet";
    return 0;
}
