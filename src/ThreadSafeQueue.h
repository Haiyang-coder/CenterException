#pragma once
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

template <typename T>
class CThreadSafeQueue
{
public:
    CThreadSafeQueue(size_t capacity) : m_capacity(capacity), m_isDestroyed(false) {}

    // 生产者放入数据
    void push(const T &value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(value);
        lock.unlock();
        // 通知消费者队列非空
        m_notEmpty.notify_one();
    }

    // 消费者取出数据
    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        // 等待队列非空
        while (m_queue.empty() && !m_isDestroyed)
        {
            m_notEmpty.wait(lock);
        }
        T value = m_queue.front();
        m_queue.pop();
        lock.unlock();
        return value;
    }
    // 销毁队列
    void destroy()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_isDestroyed = true;
        m_notEmpty.notify_all(); // 唤醒所有等待的消费者线程
    }

    // 查看队列是否销毁
    bool isDestroyed()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_isDestroyed;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
    size_t m_capacity;
    bool m_isDestroyed;
};