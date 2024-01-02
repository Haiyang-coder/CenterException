#include "SeverSocket.h"

CSeverSocket::CSeverSocket()
    : m_serv_fd(0)
{
    m_pthreadPool = new CThreadPool(D_WORK_THREADS_MIN_NUM, D_WORK_THREADS_MAX_NUM);
}

/// @brief 建立tcp的监听
/// @return 成功true 失败：false
bool CSeverSocket::InitSockEnv()
{
    struct sockaddr_in serv_addr;
    // 创建套接字
    m_serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_serv_fd)
    {
        perror("socket create error!");
        return false;
    }

    // 设置端口复用
    int opt = 1;
    if (setsockopt(m_serv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
    {
        perror("setsockopt reuseaddr error!");
        close(m_serv_fd);
        return false;
    }

    // serv_addr 服务器套接字结构初始化
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(D_SERVER_PORT);

    // 绑定
    if (-1 == (bind(m_serv_fd, (sockaddr *)&serv_addr, sizeof(serv_addr))))
    {
        perror("bind error!");
        close(m_serv_fd);
        return false;
    }

    // 监听
    if (-1 == (listen(m_serv_fd, 1024)))
    {
        perror("listen error!");
        close(m_serv_fd);
        return false;
    }

    for (int i = 0; i < D_EPOLL_THREADS_NUM; i++)
    {
        m_epoll_fds[i] = epoll_create(1000);
        if (-1 == m_epoll_fds[i])
        {
            perror("create epoll fd error!");
            close(m_serv_fd);
            exit(-1);
        }
    }
    struct epoll_event event;
    event.data.fd = m_serv_fd;
    event.events = EPOLLIN | EPOLLET;

    if (-1 == (EpollSafeCtrl(m_epoll_fds[D_EPOLL_THREADS_NUM - 1], EPOLL_CTL_ADD, m_serv_fd, &event)))
    {
        perror("epoll control error!");
        close(m_serv_fd);
        close(m_epoll_fds[D_EPOLL_THREADS_NUM - 1]);
        exit(-1);
    }
    std::cout << "InitSockEnv success" << std::endl;
    return true;
}

bool CSeverSocket::initClientEnv()
{
    if (m_client_sock != 0)
    {
        close(m_client_sock);
        m_client_sock = 0;
    }
    m_client_sock = socket(PF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(sockaddr_in));
    serv_adr.sin_addr.s_addr = htonl(D_CLIENT_IP);
    serv_adr.sin_family = AF_INET;
    if (serv_adr.sin_addr.s_addr == INADDR_NONE)
    {
        perror("ip error");
        return false;
    }
    serv_adr.sin_port = htons(D_CLIENT_PORT);
    int ret = connect(m_client_sock, (sockaddr *)&serv_adr, sizeof(serv_adr));
    if (ret < 0)
    {
        perror("connect");
        return false;
    }
    std::cout << "initClientEnv success" << std::endl;
    return true;
}

CSeverSocket::~CSeverSocket()
{
    CloseSocket();
    delete m_pthreadPool;
}

/// @brief 关闭服务端的连接socket
void CSeverSocket::CloseSocket()
{
    if (m_serv_fd != 0)
    {
        close(m_serv_fd);
        m_serv_fd = 0;
    }
}

/// @brief 监控有连接将进入，将连接建立工作交给线程池处理
void CSeverSocket::EpollAceppt_thead()
{
    struct epoll_event events[D_MAXEVENTS_NUM];

    while (1)
    {
        int nfds = EpollSafeWait(m_epoll_fds[D_EPOLL_THREADS_NUM - 1], events, D_MAXEVENTS_NUM, -1);
        if (-1 == nfds)
        {
            if (errno == EINTR)
            {
                // 被信号中断，继续等待
                continue;
            }
            else
            {
                // 其他错误
                perror("epoll_wait make mistake");
                break;
            }
        }
        LOG(LOG_DEBUG, "accept init success");
        for (int i = 0; i < nfds; i++)
        {
            if (m_serv_fd == events[i].data.fd)
            {
                // 这里有连接进入，把连接的任务交给线程池处理
                dataEpollAccept *new_node = new dataEpollAccept(0, this);
                TaskNode taskNode(&CSeverSocket::AcceptClient_thread, new_node);
                m_pthreadPool->addTask(taskNode);
            }
        }
    }
    return;
}

/// @brief 建立连接的线程入口
/// @param arg 必要参数
/// @return
void *CSeverSocket::AcceptClient_thread(void *arg)
{
    dataEpollAccept *data = (dataEpollAccept *)arg;
    CSeverSocket *server = (CSeverSocket *)data->m_arg;
    server->AcceptClien();
    return nullptr;
}

/// @brief 建立连接的具体实现
/// @return 0成功
int CSeverSocket::AcceptClien()
{
    int accept_fd;
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len;
    struct epoll_event events[D_MAXEVENTS_NUM];
    accept_fd = accept(m_serv_fd, (sockaddr *)&cli_addr, &cli_addr_len);
    if (-1 == accept_fd)
    {
        perror("accept error!");
        return -1;
    }
    std::cout << "有客户端连接成功" << std::endl;
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = accept_fd;
    EpollSafeCtrl(m_epoll_fds[accept_fd % (D_EPOLL_THREADS_NUM - 1)], EPOLL_CTL_ADD, accept_fd, &event);

    // 输出对端连接信息
    std::cout << "new connect come ip = " << inet_ntoa(cli_addr.sin_addr) << "  port : " << ntohs(cli_addr.sin_port) << std::endl;
}

/// @brief epoll的接收数据提醒线程，提醒你有数据来了
/// @param epoll_fd
/// @param arg 任务队列（需要包数据统一放入任务队列中）
void CSeverSocket::EpollRecv_thead(int epoll_fd, void *arg)
{
    unsigned char recv_buf[D_MAX_PACKET] = {0};
    struct epoll_event events[D_MAXEVENTS_NUM];
    while (1)
    {

        int nfds = EpollSafeWait(m_epoll_fds[epoll_fd], events, D_MAXEVENTS_NUM, -1);
        for (int i = 0; i < nfds; i++)
        {
            // 检查events[i].event中的EPOLLIN位是否被设置，是否有数据可读
            if (events[i].events & EPOLLIN)
            {
                // 这里有数据来，把数据的接收任务交给线程池处理
                dataEpollRecv *new_node = new dataEpollRecv(epoll_fd, events[i].data.fd, this, arg);
                TaskNode taskNode(&CSeverSocket::RecvData_thead, new_node);
                m_pthreadPool->addTask(taskNode);
            }
            // 检查发生的挂起、错误事件，进行关闭连接和清理资源
            else if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP || events[i].events & EPOLLRDHUP)
            {
                EpollSafeCtrl(m_epoll_fds[epoll_fd], EPOLL_CTL_DEL, events[i].data.fd, NULL);
                close(events[i].data.fd);
            }
        }
    }
    return;
}

/// @brief 接收数据的线程入口
/// @param arg 必要参数
/// @return
void *CSeverSocket::RecvData_thead(void *arg)
{
    dataEpollRecv *data = (dataEpollRecv *)arg;
    CSeverSocket *server = (CSeverSocket *)data->arg;
    CThreadSafeQueue<CPacket> *taskQueue = (CThreadSafeQueue<CPacket> *)data->taskqueue;
    server->RecvData(data->epoll_fd, data->recv_fd, taskQueue);
    return nullptr;
}

/// @brief 接收数据
/// @param epoll_fd epoll的fd
/// @param recv_fd 数据传输fd
/// @return
int CSeverSocket::RecvData(int epoll_fd, int recv_fd, CThreadSafeQueue<CPacket> *taskQueue)
{
    unsigned char recv_buf[D_MAX_PACKET] = {0};
    memset(recv_buf, 0, sizeof(recv_buf));
    size_t index = 0;
    int recv_len = recv(recv_fd, recv_buf + index, D_MAX_PACKET - index, 0);
    if (recv_len == 0)
    {
        // 断开了连接
        perror("client close socket!");
        EpollSafeCtrl(epoll_fd, EPOLL_CTL_DEL, recv_fd, NULL);
        close(recv_fd);
        return -1;
    }
    else if (recv_len < 0)
    {
        // 接收失败了
        perror("recv error!");
        return -1;
    }
    std::cout << "接受到了信息：长度为： " << std::to_string(recv_len) << std::endl;
    for (int i = 0; i < 18; ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(recv_buf[i])) << " ";

        std::cout << "  "; // Add extra space every 4 bytes

        if ((i + 1) % 16 == 0)
        {
            std::cout << std::endl; // Add newline every 18 bytes
        }
    }
    std::cout << std::endl;
    index = recv_len;
    std::cout << "接收index是：" << std::to_string(index) << std::endl;

    while (index > 0) // 除非缓冲区被完全清空否则一致循环
    {
        // 接收到了数据
        // 考虑粘包问题
        size_t lengthAll = index;
        CPacket packet((const char *)recv_buf, lengthAll);
        std::cout << "构造的数据长度为：" << std::to_string(lengthAll) << std::endl;
        std::cout << "接收到的数据是：" << std::endl;
        std::cout << packet.m_strData << std::endl;
        if (lengthAll > 0)
        {
            // 构造完一个完整的包后：
            // 1。将缓冲中用过的数据清除
            // 2.更新此时缓冲中剩余的数据
            // 继续循环
            memmove(recv_buf, recv_buf + lengthAll, D_MAX_PACKET - lengthAll);
            index -= lengthAll;
            // 这里要把构造好的包，放到一个缓冲队列里面
            taskQueue->push(packet);

            continue;
        }
        else
        {
            // 无法构造有效包
            index = 0;
            break;
        }
    }
    return SUCCESS;
}

/// @brief 关epoll文件描述符
/// @param number
/// @return
int CSeverSocket::CloseEpollfd(int number)
{
    return close(m_epoll_fds[number - 1]);
}

/// @brief 线程安全的epollCtl操作
/// @param epfd epollfd
/// @param opera 操作
/// @param fd socket的fd
/// @param event epollevent结构体指针
/// @return 0成功
int CSeverSocket::EpollSafeCtrl(int epfd, int opera, int fd, epoll_event *event)
{
    int ret = epoll_ctl(epfd, opera, fd, event);
    return ret;
}

/// @brief 线程安全的epollwait
/// @param __epfd epollfd
/// @param __events __events
/// @param __maxevents __maxevents
/// @param __timeout 超时时间
/// @return
int CSeverSocket::EpollSafeWait(int __epfd, epoll_event *__events, int __maxevents, int __timeout)
{
    int ret = epoll_wait(__epfd, __events, __maxevents, __timeout);
    return ret;
}
