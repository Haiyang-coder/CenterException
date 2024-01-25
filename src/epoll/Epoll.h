#pragma once
#include <unistd.h>
#include <sys/epoll.h>
#include <vector>
#include <errno.h>
#include <sys/signal.h>
#include <memory.h>
#include "../tools/LogPrint.h"

#define SUCCESS 0
#define EVENTS_SIZE 1024

// 因为一般传给epollwait的是一个数组，这里我用vector进行封装
using EPEvents = std::vector<epoll_event>;

/// @brief 这个类就是对epoll常用操作的封装
class CEpoll
{
public:
    CEpoll() { m_epoll = 0; }
    ~CEpoll() { Close(); }
    CEpoll(const CEpoll &) = delete;
    CEpoll &operator=(const CEpoll &) = delete;

public:
    operator int() const { return m_epoll; }

public:
    // 创建epoll
    int Create(unsigned count);
    // 设置epoll等待
    ssize_t WaitEvents(EPEvents &events, int timeout = -1);
    // 添加eopoll事件
    int Add(int fd, const epoll_data_t &data, uint32_t events = EPOLLIN);
    // 修改epoll事件
    int Modify(int fd, const epoll_data_t &data, uint32_t events = EPOLLIN);
    // 删除epoll事件
    int Del(int fd);
    // 关闭epoll
    void Close();

private:
    // epoll的标识符
    int m_epoll;
};
