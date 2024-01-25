#include "timer.h"

CTimer::CTimer() : timerfd(-1), m_bRunning(false)
{
}

CTimer::~CTimer()
{
    StopTimer();
}

/// @brief 开启计时器，也可以用作计时器的重启
/// @return
int CTimer::StartTimer()
{
    if (timerfd < 0)
    {
        return -1;
    }
    // 启动定时器(或者重新设置)
    if (timerfd_settime(timerfd, 0, &m_interval, NULL) == -1)
    {
        perror("timerfd_settime");
        return -2;
    }
}

/// @brief 设置定时器的间隔
/// @param seconds [in] 间隔时间
/// @return
int CTimer::SetTimer(const int &seconds)
{
    // 将之前的计时器设置清零
    memset(&m_interval, 0, sizeof(m_interval));
    //  定时器的重复间隔
    m_interval.it_value.tv_sec = seconds;
    m_interval.it_value.tv_nsec = 0;
    // 定时器的初始值
    m_interval.it_interval.tv_sec = 0;
    m_interval.it_interval.tv_nsec = 0;
}

/// @brief 到了时间就会返回
/// @return
void CTimer::Wait()
{
    // 调用这个就会阻塞，计时器好了之后就会放开
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock);
}

/// @brief 通过阻塞的方式等待计时器的结束
/// @return
int CTimer::WaitTimerOn()
{
    int ret = -1;
    ret = m_epoll.Create(1);
    if (ret < 0)
    {
        PRINT_ERROR(ret, " m_epoll.Create");
        return -1;
    }
    epoll_data_t fd;
    fd.fd = timerfd;
    uint32_t event = EPOLLIN;
    ret = m_epoll.Add(timerfd, fd, event);
    if (ret < 0)
    {
        PRINT_ERROR(ret, " m_epoll.Add");
        return -2;
    }
    // 开始等待计时器的唤醒

    workerThread = std::thread([this]()
                               {
                                   while (m_bRunning)
                                   {
                                       // 开始等待
                                       EPEvents events;
                                       auto numEvents = m_epoll.WaitEvents(events);
                                       if (numEvents == -1)
                                       {
                                           PRINT_ERROR(numEvents, " m_epoll.WaitEvents");
                                           return -1;
                                       }
                                       // 检测到epollin事件
                                       //  处理定时器事件
                                       if (events[0].data.fd == timerfd)
                                       {
                                           // 通知等待线程
                                           condition.notify_all();
                                       }
                                   } });

    return 0;
}

int CTimer::StopTimer()
{
    // 计时器都没有开启
    if (timerfd == -1)
    {
        m_bRunning = false;
        return 0;
    }
    // 唤醒现在等待的线程
    condition.notify_all();
    // 正在运行，关闭资源
    if (m_bRunning)
    {
        close(timerfd);
        m_bRunning = false;
        m_epoll.Close();
    }
    // 等待线程资源的释放
    if (workerThread.joinable())
    {
        workerThread.join();
    }
}

int CTimer::CreateTimer()
{
    // 已经有计时器被创建了
    if (timerfd > 0)
    {
        return 0;
    }
    // 创建一个计时器
    timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timerfd == -1)
    {
        return -1;
    }
    m_bRunning = true;
    return 0;
}
