#pragma onceMuduoServerduoServer
#include <unistd.h>
#include "muduo/net/TcpServer.h"
#include "muduo/base/ThreadPool.h"、
#include "dealTask/TaskDeal.h"
#include "data/Packet/Packet.h"

using namespace muduo;
using namespace muduo::net;

class CNetparam
{
public:
    // 这个是服务器配置类，主要包括，服务器的网络io线程数，计算线程数，port地址
    CNetparam()
    {
        port = std::stoi(m_systemIni.GetValue("server", "port", "7824"));
        ioThreadNum = std::stoi(m_systemIni.GetValue("server", "IOthreadNum", "5"));
        workThreadNumber = std::stoi(m_systemIni.GetValue("server", "WorkthreadNum", "10"));
    }
    ~CNetparam(){};

private:
    // 用来读取配置文件
    CSimpleIniEx m_systemIni;

public:
    int port;
    int ioThreadNum;
    int workThreadNumber;
};

class CMuduoServer
{
public:
    CMuduoServer(EventLoop *loop);
    ~CMuduoServer();

    // 服务端开启服务
    int Start();

private:
    // 连接建立的回调函数
    void onConnection(const TcpConnectionPtr &conn);
    // 消息到来的回调函数
    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time);

private:
    // CNetparam m_param;
    CNetparam m_param;
    // EventLoop
    EventLoop *m_loop;
    // 服务器
    TcpServer m_server;
    // io线程数
    int m_numIoThreads;
    // loop开始计时器
    Timestamp m_startTime;
    // 用来处理任务的模块
    CTaskDeal m_taskDeal;
    // 服务器的配置参数
};