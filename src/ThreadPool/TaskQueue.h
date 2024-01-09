/*
 * @Author: 孙恒康
 * @Date: 2023-11-17 09:38:35
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-11-17 10:21:17
 * @FilePath: /ExceptionnalOperaAggregteSystem/src/epoll_thread.h
 * @Description: 线程池
 */
#ifndef _EPOLL_THREAD_QUEUE_
#define _EPOLL_THREAD_QUEUE_

#include<queue>
#include<thread>

#define INT_WORKTHREAD_NUM 5

//定义任务结构体
using  ThreadPollCallBack = void*(* )(void *arg);

struct TaskNode
{
   TaskNode()
    {
        function = nullptr;
        arg = nullptr;
    }
    TaskNode(ThreadPollCallBack func, void* arg)
    {
        function = func;
        this->arg = arg;
    }
    ThreadPollCallBack function;
    void* arg;
};

class CThreadQueue
{
public:
    CThreadQueue();
    ~CThreadQueue();
// 添加任务
    void addTask(TaskNode& task);
    void addTask(ThreadPollCallBack func, void* arg);

    // 取出一个任务
    TaskNode takeTask();

    // 获取当前队列中任务个数
    inline int taskNumber()
    {
        return m_queue.size();
    }

private:
    pthread_mutex_t m_mutex;    // 互斥锁
    std::queue<TaskNode> m_queue;   // 任务队列


};

#endif