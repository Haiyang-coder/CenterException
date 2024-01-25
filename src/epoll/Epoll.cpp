#include "Epoll.h"

/// @brief 创建epoll
/// @param count
/// @return
int CEpoll::Create(unsigned count)
{
    if (m_epoll < 0)
    {
        return -1;
    }
    m_epoll = epoll_create(count);
    if (m_epoll < 0)
    {
        return -2;
    }
    return SUCCESS;
}

/// @brief 等待epoll提醒
/// @param events [in\out] epoll等待返回的事件合集
/// @param timeout[in] 默认是阻塞状态
/// @return
ssize_t CEpoll::WaitEvents(EPEvents &events, int timeout)
{
    if (m_epoll < 0)
        return -1;
    EPEvents evs(EVENTS_SIZE);
    int ret = epoll_wait(m_epoll, evs.data(), static_cast<int>(evs.size()), timeout);
    if (ret < 0)
    {
        if (errno == EINTR || errno == EAGAIN)
        {
            return 0;
        }
        return -2;
    }
    if (ret > static_cast<int>(events.size()))
    {
        events.resize(ret);
    }
    memcpy(events.data(), evs.data(), sizeof(epoll_event) * ret);
    return ret;
}

/// @brief 添加epoll要监听的文件描述符以及事件
/// @param fd [in]要监听的文件描述符
/// @param data [in] 给epoll存储的信息epoll_data_t
/// @param events [in] 要监听的事件类型
/// @return 0是成功
int CEpoll::Add(int fd, const epoll_data_t &data, uint32_t events)
{
    if (m_epoll < 0)
        return m_epoll;
    epoll_event ev = {events, data};
    int ret = epoll_ctl(m_epoll, EPOLL_CTL_ADD, fd, &ev);
    if (ret < 0)
        return -2;
    return 0;
}

/// @brief 修改epoll要监听的文件描述符以及事件
/// @param fd [in]要监听的文件描述符
/// @param data [in] 给epoll存储的信息epoll_data_t
/// @param events [in] 要监听的事件类型
/// @return 0是成功
int CEpoll::Modify(int fd, const epoll_data_t &data, uint32_t events)
{
    if (m_epoll < 0)
        return m_epoll;
    epoll_event ev = {events, data};
    int ret = epoll_ctl(m_epoll, EPOLL_CTL_MOD, fd, &ev);
    if (ret < 0)
        return -2;
    return 0;
}

/// @brief 删除epoll要监听的文件描述符
/// @param fd [in] 要删除的文件描述符
/// @return 0是成功
int CEpoll::Del(int fd)
{
    if (m_epoll < 0)
        return m_epoll;
    int ret = epoll_ctl(m_epoll, EPOLL_CTL_DEL, fd, NULL);
    if (ret < 0)
        return -2;
    return 0;
}

/// @brief 关闭epoll
void CEpoll::Close()
{
    if (m_epoll != -1)
    {
        int fd = m_epoll;
        m_epoll = -1;
        close(fd);
    }
}
