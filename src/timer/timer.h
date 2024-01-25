#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/timerfd.h>
#include "../epoll/Epoll.h"
#include "../tools/LogPrint.h"
#include <condition_variable>
#include <mutex>
#include <thread>

class CTimer

{

public:
    CTimer(/* args */);
    ~CTimer();

public:
    // 开启计时器
    int StartTimer();
    // 设置计时器
    int SetTimer(const int &seconds);
    // 计时器到时间了就会返回
    void Wait();
    //  停止计时器
    int StopTimer();

private:
    // 创建计时器
    int CreateTimer();
    // 等待计时器的通知
    int WaitTimerOn();

private:
    // 定时器的参数。包含了初始延迟和重复间隔
    struct itimerspec m_interval;
    // 计时器的描述符
    int timerfd;
    // epoll的操作类
    CEpoll m_epoll;
    // 是否正在运行
    bool m_bRunning;
    // 计时器在这个线程中等待
    std::thread workerThread;
    // 锁配合条件变量使用
    std::mutex mutex;
    // 条件变量用来唤醒wait函数
    std::condition_variable condition;
};
