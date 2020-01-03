#ifndef UTIL_CONCURRENT_QUEUE_H
#define UTIL_CONCURRENT_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include <spdlog/spdlog.h>


template<typename T>
class Queue {
public:
  T pop();
  void push(const T&);
  void push(T&&);
  
private:
  std::queue<T> queue;
  std::mutex mutex;
  std::condition_variable condition;
};


template<typename T>
T Queue<T>::pop() {
  std::unique_lock<std::mutex> mlock(mutex);
  condition.wait(mlock, [this]{ return !queue.empty(); });

  auto item = queue.front();
  queue.pop();

  return item;
}

template<typename T>
void Queue<T>::push(const T& item) {
  std::unique_lock<std::mutex> mlock(mutex);
  queue.push(item);
  mlock.unlock();
  condition.notify_one();
}

template<typename T>
void Queue<T>::push(T&& item) {
  std::unique_lock<std::mutex> mlock(mutex);
  queue.push(std::move(item));
  mlock.unlock();
  condition.notify_one();
}

#endif
