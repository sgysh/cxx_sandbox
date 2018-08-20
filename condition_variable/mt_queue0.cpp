// Boost Software License 1.0
// Copyright (c) 2014 yohhoy

// mutex + busy-loop
#include <queue>
#include <thread>
#include <mutex>

class mt_queue {
  static const int capacity = 10;
  std::queue<int> q_;
  std::mutex mtx_;
public:
  void push(int data)
  {
    std::unique_lock<std::mutex> lk(mtx_);
    while (q_.size() == capacity) {
      lk.unlock();
      std::this_thread::yield();
      lk.lock();
    }
    q_.push(data);
  }
  int pop()
  {
    std::unique_lock<std::mutex> lk(mtx_);
    while (q_.empty()) {
      lk.unlock();
      std::this_thread::yield();
      lk.lock();
    }
    int data = q_.front();
    q_.pop();
    return data;
  }
};


//----------------------------------------------------------
#include <iostream>
const int N = 100;

int main()
{
  mt_queue q;
  std::thread th1([&]{
    for (int i = 1; i <= N; ++i)
      q.push(i);
    q.push(-1);  // end of data
  });
  std::thread th2([&]{
    int v;
    while ((v = q.pop()) > 0)
      std::cout << v << std::endl;
    std::cout << "(EOD)" << std::endl;
  });
  th1.join();
  th2.join();
}
