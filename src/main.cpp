#include "./socket/SeverSocket.h"
#include <vector>
#include <system_error>
#include "json.h"
#include <fstream>
#include <iostream>
#include "./simpleIni/SimpleIni.h"
#include <functional>
#include <thread>
#include "./dm/sqlGradoperate.h"
#include "DealTask.h"
#include "./ftp/ftpServer.h"

int main()
{
    // 开启ftp服务
    Cftp ftpServer;
    if (ftpServer.StartFtp() < 0)
    {
        perror("StartFtp function error!");
        exit(-1);
    }
    pause();
    // 初始化线程安全的任务队列
    // 所有的请求都会放到这个队列中排队执行
    CThreadSafeQueue<dataInQueue> taskQueue;

    //  初始化服务端的通信套接字
    CSeverSocket socket;
    bool ret = socket.InitSockEnv();
    if (!ret)
    {
        perror("InitSockEnv function error!");
        exit(-1);
    }

    // 创建接收连接线程
    std::vector<std::thread> vectThread;
    try
    {
        vectThread.emplace_back(&CSeverSocket::EpollAceppt_thead, &socket);
        LOG(LOG_DEBUG, " 监听线程开启");
    }
    catch (const std::system_error &e)
    {
        LOG(LOG_ERROR, " cuowu");
        PRINT_ERROR(-1, std::string("Failed to create thread: ") + e.what());
        perror("create establishlink function error!");
        socket.CloseSocket();
        socket.CloseEpollfd(D_EPOLL_THREADS_NUM);
        exit(-1);
    }

    // 创建接收数据线程
    for (int i = 0; i < D_EPOLL_THREADS_NUM - 1; i++)
    {
        try
        {
            vectThread.emplace_back(&CSeverSocket::EpollRecv_thead, &socket, i, &taskQueue);
            std::cout << " 接受线程" << std::to_string(i) << "开启" << std::endl;
        }
        catch (const std::system_error &e)
        {
            LOG(LOG_ERROR, " cuowu");
            PRINT_ERROR(-1, "create establishlink function error!");
            socket.CloseSocket();
            socket.CloseEpollfd(i);
            exit(-1);
        }
    }

    // 初始化数据库
    CSqlGradOperate sqlOpera;
    if (sqlOpera.InitDMShemaAndTable() < 0)
    {
        PRINT_ERROR(-1, "初始化数据库失败，退出程序");
        exit(-1);
    }

    // 设置数据的全备份和增量备份
    if (sqlOpera.SetDbBackup() < 0)
    {
        PRINT_ERROR(-1, "设置数据库备份出错，退出");
        exit(-1);
    }

    // 初始化任务处理模块
    CDealTask dealTask(&taskQueue);
    if (dealTask.SetSocket(&socket) != 0)
    {
        PRINT_ERROR(-1, "设置通信模块出错，退出");
        exit(-1);
    }
    if (dealTask.SetSqlCtl(&sqlOpera) != 0)
    {
        PRINT_ERROR(-1, "设置数据库出错，退出");
        exit(-1);
    }
    // 创建任务处理线程
    dealTask.StartDealTask();

    // 安全退出线程
    for (size_t i = 0; i < vectThread.size(); ++i)
    {
        if (vectThread[i].joinable())
        {
            vectThread[i].join();
        }
    }
    // 套接字的销毁
    socket.CloseSocket();
    LOG(LOG_DEBUG, "程序结束退出");
    return 0;
}
