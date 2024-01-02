#include "DealTask.h"
CDealTask::CDealTask(CThreadSafeQueue<CPacket> *taskQueue) : m_ptaskQueue(taskQueue)
{
    m_pthreadPool = new CThreadPool(D_WORK_THREADS_MIN_NUM, D_WORK_THREADS_MAX_NUM);
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
    m_thread = std::thread(&CDealTask::DealTaskThread,this);
    return 0;
}

int CDealTask::DealTaskThread()
{
    while (!m_ptaskQueue->isDestroyed())
    {
        std::cout << "开始执行任务" << std::endl;
        CPacket pack = m_ptaskQueue->pop();
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
        RequstJsonData jsonData;
        ret = jsonData.TurnStr2Obj(strDataDec.c_str());
        if (ret != SUCCESS)
        {
            PRINT_ERROR(ret, "TurnStr2Obj 失败");
        }
        // 3. 根据数据中的密令码来执行
        //  switch (jsonData.mainCMD)
        //  {
        //  case 0:

        //     break;

        // default:
        //     break;
        // }
        // 这里就默认是存入数据库了
        ret = SaveDataInDB(jsonData);
        if (ret < 0)
        {
            PRINT_ERROR(ret, "SaveDataInDB 失败");
        }

    }
    return 0;
}

void CDealTask::DealTask()
{
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

int CDealTask::SaveDataInDB(const DataInfoBase &data)
{
    CSqlGradOperate sqlOpera;
    int ret = 0;
    ret = sqlOpera.InitDMShemaAndTable();
    if (ret < 0)
    {
        PRINT_ERROR(ret, "数据库初始化出现问题 失败");
        return ret;
    }
    std::string sql;
    data.GetInserDataInOrder(sql);
    ret = sqlOpera.DataInsert(sql.c_str());
    if (ret < 0)
    {
        PRINT_ERROR(ret, "数据库插入语句执行错误 失败");
        return ret;
    }
    return 0;
}
