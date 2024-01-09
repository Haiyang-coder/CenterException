#include"TaskQueue.h"

CThreadQueue::CThreadQueue()
{
    pthread_mutex_init(&m_mutex, NULL);
}

CThreadQueue::~CThreadQueue()
{
     while (!m_queue.empty())
    {
        TaskNode task = m_queue.front();
        m_queue.pop();
        // 清理任务
        delete task.arg;
        task.arg = nullptr;
    }
    pthread_mutex_destroy(&m_mutex);
}

void CThreadQueue::addTask(TaskNode &task)
{
    pthread_mutex_lock(&m_mutex);
    m_queue.push(task);
    pthread_mutex_unlock(&m_mutex);
}

void CThreadQueue::addTask(ThreadPollCallBack func, void *arg)
{
    pthread_mutex_lock(&m_mutex);
    TaskNode task;
    task.function = func;
    task.arg = arg;
    m_queue.push(task);
    pthread_mutex_unlock(&m_mutex);

}

TaskNode CThreadQueue::takeTask()
{
    TaskNode t;
    pthread_mutex_lock(&m_mutex);
    if (m_queue.size() > 0)
    {
        t = m_queue.front();
        m_queue.pop();
    }
    pthread_mutex_unlock(&m_mutex);
    return t;

}
