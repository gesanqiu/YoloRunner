/*
 * @Description: Thread pool task queue implement.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-07 20:23:08
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-19 03:02:28
 */
#pragma once

#include <list>
#include <mutex>
#include <condition_variable>
#include <iostream>

template<typename T>
class SafeQueue {
private:
    std::list<std::shared_ptr<T>> m_queue;
    std::mutex m_mutex;//全局互斥锁
    std::condition_variable m_notEmpty;//全局条件变量（不为空）
    std::condition_variable m_notFull;//全局条件变量（不为满）
    unsigned int m_maxSize;//队列最大容量
    std::atomic<bool> m_isExit;

private:
    //队列为空
    bool isEmpty() const {
        return m_queue.empty();
    }
    //队列已满
    bool isFull() const {
        return m_queue.size() == m_maxSize;
    }

public:
    SafeQueue(unsigned int maxSize = 25) {
        m_isExit.store(false);
        this->m_maxSize = maxSize;
    }

    void setMaxSize(unsigned int maxSize) {
        std::unique_lock<std::mutex> locker(m_mutex);
        this->m_maxSize = maxSize;
    }

    unsigned int getMaxSize() {
        std::unique_lock<std::mutex> locker(m_mutex);
        return this->m_maxSize;
    }

    unsigned int getCurrentSize() {
        std::unique_lock<std::mutex> locker(m_mutex);
        return m_queue.size();
    }

    ~SafeQueue(){}

    void exit() {
        m_isExit.store(true);
        m_notEmpty.notify_all();
        m_notFull.notify_all();
    }

    void product(const std::shared_ptr<T>& v) {
        std::unique_lock<std::mutex> locker(m_mutex);
        while(isFull()) {
            m_notFull.wait(locker);
        }

        m_queue.push_back(v);
        m_notEmpty.notify_one();
    }

    void consumption(std::shared_ptr<T>& v) {
        std::unique_lock<std::mutex> locker(m_mutex);
        while(isEmpty()) {
            if (this->m_isExit.load()) {
                v = nullptr;
                return ;
            }
            m_notEmpty.wait(locker);
        }

        v = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
    }
};