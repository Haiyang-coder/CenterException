#include "TaskDeal.h"
#include "muduo/base/Logging.h"
#include "data/DataRequest/DataRequest.h"
CTaskDeal::CTaskDeal(int threadNumer) : m_threadNum(threadNumer)
{
    InitVerTableName();
}

CTaskDeal::~CTaskDeal()
{
}

/// @brief 开启任务线程池
/// @return
int CTaskDeal::Start()
{
    LOG_INFO << "任务处理线程池已经启动,开启的任务处理线程数是:" << m_threadNum << "_threads";
    m_threadPool.start(m_threadNum);
    return 0;
}

int CTaskDeal::SolveTask(CPacket packet)
{
    m_threadPool.run(std::bind(&CTaskDeal::ExcuteTask, this, std::move(packet)));
    return 0;
}

void CTaskDeal::ExcuteTask(CPacket packet)
{
    // 解密数据
    int ret = 0;
    std::string strDataDec;
    ret = DecSm4Data(packet.m_strData, strDataDec);
    if (ret < 0)
    {
        LOG_ERROR << "DecSm4Data 失败";
    }

    //  将解密的数据转换成类
    switch (packet.m_msgVersion)
    {
    case 0x4300: // 定时备份数据
    {
        CDataRequest request;
        m_pftpCtl->RetSetTimeSpan(request.timeSpan);
        break;
    }
    default:
    {
        // 默认的就是将数据存入数据库
        auto it = m_mapTableName.find(packet.m_msgVersion);
        if (it == m_mapTableName.end())
        {
            break;
        }
        std::string strDataInsert;
        DataInfoBase jsonData(strDataDec.c_str());
        std::string modelName;
        m_pdbCtl->GetModelName(modelName);
        jsonData.GetInserDataInOrder(strDataInsert, modelName, m_mapTableName[0x4120]);
        m_pdbCtl->DataInsert(strDataInsert.c_str());
    }
    break;
    }
}

// /// @brief 将数据以csv的格式存到指定的文件夹
// /// @param dataAll 入参 数据
// /// @param fullName 入参 保存的路径（包括文件名）
// /// @return 0成功
// int CTaskDeal::SavaDataAsCsv(const CDbData &dataAll, std::string fullName)
// {
//     int lineSize = dataAll.iLineSize; // 行数
//     int cloSize = dataAll.iClomuSize; // 列数
//     // 获取输出路径的目录
//     std::string outputDir = m_filepath;

//     // 如果目录不存在，新建目录
//     if (!outputDir.empty() && access(outputDir.c_str(), F_OK) != 0)
//     {
//         if (mkdir(outputDir.c_str(), 0777) != 0)
//         {
//             std::cerr << "Error creating directory: " << outputDir << std::endl;
//             return -2;
//         }
//     }
//     // 打开文件操作流开始写入数据
//     std::ofstream csvFile(fullName);

//     // 先写入标题信息
//     for (size_t i = 0; i < cloSize; i++)
//     {
//         if (i == cloSize - 1)
//         {
//             csvFile << dataAll.vecCloName.at(i) << "\n";
//         }
//         csvFile << dataAll.vecCloName.at(i) << ",";
//     }
//     // 按照数据类型写入剩余的所有数据
//     for (size_t i = 0; i < lineSize; i++)
//     {
//         for (size_t j = 0; j < cloSize; j++)
//         {
//             if (dataAll.vecType.at(j) == TYPE_INT)
//             {
//                 if (j == cloSize - 1)
//                 {
//                     csvFile << dataAll.vecCloName.at(i * cloSize + j) << "\n";
//                 }
//                 csvFile << dataAll.vecDbRow.at(i * cloSize + j).piData << ",";
//             }
//             else if (dataAll.vecType.at(j) == TYPE_STRING)
//             {
//                 if (j == cloSize - 1)
//                 {
//                     csvFile << "\"" << dataAll.vecDbRow.at(i * cloSize + j).strData << "\""
//                             << "\n";
//                 }
//                 csvFile << "\"" << dataAll.vecDbRow.at(i * cloSize + j).strData << "\""
//                         << ",";
//             }
//         }
//     }
//     // 关闭文件流
//     csvFile.close();
//     return 0;
// }

// int CTaskDeal::DealTaskThread()
// {
// }

/// @brief 设置数据库操作模块
/// @param pdbCtl
/// @return
int CTaskDeal::SetSqlCtl(CSqlGradOperate *pdbCtl)
{

    if (m_pdbCtl == NULL)
    {
        m_pdbCtl = pdbCtl;
        return 0;
    }
    return -1;
}

/// @brief 设置ftp服务器的操作指针
/// @param pftpCtl
/// @return
int CTaskDeal::SetFtpCtl(CftpServerEx *pftpCtl)
{
    if (m_pftpCtl == NULL)
    {
        m_pftpCtl = pftpCtl;
        return 0;
    }
    return -1;
}

/// @brief 将数据解密传出
/// @param dataIn in 需要解密的数据
/// @param dataOut out 解密完成的数据
/// @return 0：成功
int CTaskDeal::DecSm4Data(const std::string &dataIn, std::string &dataOut)
{
    // todo :解密数据
    return 0;
}

// /// @brief
// /// @param data
// /// @return
// int CTaskDeal::SaveDataInDB(const DataInfoBase &data, const std::string &ModelName, const std::string &tableName)
// {
//     int ret = 0;
//     ret = m_pdbCtl->InitDMShemaAndTable();
//     if (ret < 0)
//     {
//         PRINT_ERROR(ret, "数据库初始化出现问题 失败");
//         return ret;
//     }
//     std::string sql;
//     data.GetInserDataInOrder(sql, ModelName, tableName);
//     ret = m_pdbCtl->DataInsert(sql.c_str());
//     if (ret < 0)
//     {
//         PRINT_ERROR(ret, "数据库插入语句执行错误 失败");
//         return ret;
//     }
//     return 0;
// }
// // /// @brief 将客户端要查询的数据放到ftp服务器指定目录上
// /// @param tableName
// /// @param startTime
// /// @param EndTime
// /// @return
// int CTaskDeal::SaveDataInFtp(const std::string &tableName, const std::string &startTime, const std::string &EndTime)
// {
//     std::string fileName = tableName + ":" + startTime + "-" + EndTime;
//     // 要保存文件的完整路径名
//     std::string fullNanme = m_filepath + "/" + fileName;
//     CDbData dataAll;
//     m_pdbCtl->GetDataBySubmitTime(tableName.c_str(), startTime.c_str(), EndTime.c_str(), dataAll);
//     m_pdbCtl->SavaDataAsCsv(dataAll, fullNanme);
//     return 0;
// }

// /// @brief 初始化读配置文件的对象
// /// @return
// int CTaskDeal::InitConfigReader()
// {
//     // 定义ini文档对象
//     // 参数为true时保存为UTF-8格式，否则为本地编码格式
//     m_systemIni.SetUnicode(true);
//     // 是否允许一个关键字对应多个值，默认为允许；若不允许，则将最后一个值作为此关键字关联的值
//     m_systemIni.SetMultiKey(false);
//     SI_Error ret = 0;
//     ret = m_systemIni.LoadFile(SYSTEM_INI_FILE_PATH);
//     if (ret < 0)
//     {
//         std::cout << "加载文件：" << SYSTEM_INI_FILE_PATH << "失败了" << std::endl;
//         return ret;
//     }
// }

/// @brief 构建表名和version的映射关系
/// @return
int CTaskDeal::InitVerTableName()
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
