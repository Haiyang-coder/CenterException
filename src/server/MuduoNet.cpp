#include "MuduoNet.h"
#include "muduo/base/Logging.h"

CMuduoServer::CMuduoServer(EventLoop *loop)
    : m_numIoThreads(m_param.ioThreadNum), m_taskDeal(m_param.workThreadNumber),
      m_server(loop, InetAddress(m_param.port), "ExceptionServer"),
      m_startTime(Timestamp::now()),
      m_loop(loop)
{
    LOG_INFO << " 服务端 ExceptionServer 开始初始化";
    // 绑定我们关注的事件
    m_server.setConnectionCallback(
        std::bind(&CMuduoServer::onConnection, this, _1));
    m_server.setMessageCallback(
        std::bind(&CMuduoServer::onMessage, this, _1, _2, _3));
    // 根据配置文件初始化我们的服务器数据
    LOG_INFO << "服务器开启:开始网络io线程的数量是: " << m_numIoThreads << " threads";
    m_server.setThreadNum(m_numIoThreads);
    LOG_INFO << " 服务端 ExceptionServer 初始化完成";
}

CMuduoServer::~CMuduoServer()
{
    if (m_loop != NULL)
    {
        m_loop->quit();
    }
}

int CMuduoServer::Start()
{
    LOG_INFO << "服务器开启:开始网络io线程的数量是: " << m_numIoThreads << " threads";
    m_server.start();
    m_taskDeal.Start();
    return 0;
}

void CMuduoServer::onConnection(const TcpConnectionPtr &conn)
{
    LOG_INFO << "CMuduoServer - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
}

/// @brief 消息到达回调函数
/// @param conn
/// @param buf
/// @param
void CMuduoServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
{
    // 这个函数要
    // 1.构造完整数包
    // 2.执行数据包中的操作

    // 找到数据头
    CPacket packet;
    size_t dataSize = buf->readableBytes();
    int ret = packet.FindHeadStart(buf->peek(), dataSize);
    if (ret != 0)
    {
        LOG_INFO << "没有找到数据包的起始标志位";
        buf->retrieveAll();
        return;
    }
    // 将数据流中不是数据包的部分都清理出去
    buf->retrieve(dataSize);
    // 重新赋值
    dataSize = buf->readableBytes();
    // 构造完整的数据包
    ret = packet.ConstructData(buf->peek(), dataSize);
    if (ret != 0)
    {
        LOG_INFO << "本次消息到来并不能构造一个完整的包";
        return;
    }

    // 数据流可以构成一个完整的数据包
    // 将数据包取出来给任务处理模块处理
    buf->retrieve(dataSize);
    dataSize = buf->readableBytes();
    m_taskDeal.SolveTask(std::move(packet));
}
