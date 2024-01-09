
#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include <thread>
#include <pthread.h>
#include "TaskQueue.h"
#include <iostream>
#include <chrono>
#include <cstring>
#include "SimpleIni.h"
#include "../cmdresult.h"

class CThreadPool
{
public:
    // 因为是static，所以声明周期和进程一样，所以是RAII原则的单例模式
    static CThreadPool &getInstance()
    {
        static CThreadPool instance; // 线程安全的局部静态变量
        return instance;
    }

    // 添加任务
    void addTask(TaskNode task);
    // 获取忙线程的个数
    int getBusyNumber();
    // 获取活着的线程个数/*
    int getAliveNumber();
    void destorypools();

private:
    CThreadPool();
    ~CThreadPool();
    CThreadPool(const CThreadPool &) = delete;
    CThreadPool &operator=(const CThreadPool &) = delete;

private:
    // 工作的线程的任务函数
    static void *worker(void *arg);
    // 管理者线程的任务函数
    static void *manager(void *arg);
    void threadExit();

private:
    pthread_mutex_t m_lock;
    pthread_cond_t m_notEmpty;
    pthread_t *m_threadIDs;
    pthread_t m_managerID;
    CThreadQueue *m_taskQ;
    int m_minNum;
    int m_maxNum;
    int m_busyNum;
    int m_aliveNum;
    int m_exitNum;
    bool m_shutdown = false;
};

#endif
