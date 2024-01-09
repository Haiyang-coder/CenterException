#include "SeverSocket.h"
#include <vector>
#include <system_error>
#include "json.h"
#include <fstream>
#include <iostream>
#include "SimpleIni.h"
#include <functional>
#include <thread>
#include <fineftp/server.h>

#define D_WORK_THREADS_NUM 5
#define TASK_QUEUE_MAX_SIZE 2048
#ifdef WIN32
const std::string local_root = "C:\\"; // The backslash at the end is necessary!
#else                                  // WIN32
const std::string local_root = "/dm8";
#endif                                 // WIN32
int main()
{

    // 初始化ftp服务器
    fineftp::FtpServer server(212);
    server.addUserAnonymous(local_root, fineftp::Permission::All);
    server.addUser("MyUser", "MyPassword", local_root, fineftp::Permission::ReadOnly);
    server.addUser("Uploader", "12345", local_root, fineftp::Permission::DirList | fineftp::Permission::DirCreate | fineftp::Permission::FileWrite | fineftp::Permission::FileAppend);
    server.start(4);

    //  初始化服务端的通信套接字
    CSeverSocket socket;
    bool ret = socket.InitSockEnv();
    if (!ret)
    {
        perror("InitSockEnv function error!");
        exit(-1);
    }
    // 初始化线程安全的任务队列
    CThreadSafeQueue<CPacket> taskQueue(TASK_QUEUE_MAX_SIZE);
    // 初始化任务处理模块
    CDealTask dealTask(&taskQueue);
    // 创建任务处理线程
    dealTask.StartDealTask();

    // 创建accept connect线程
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

    // 创建处理EPOLLIN线程
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

    for (size_t i = 0; i < vectThread.size(); ++i)
    {
        if (vectThread[i].joinable())
        {
            vectThread[i].join();
        }
    }
    // 套接字的销毁
    socket.CloseSocket();
    std::cout << "jieshu" << std::endl;
    LOG(LOG_DEBUG, "程序结束退出");
    return 0;
}
