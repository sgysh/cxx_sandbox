// Boost Software License 1.0
// Copyright (c) 2014 yohhoy

// two CVs + notify_one(signal)
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class mt_queue {
  static const int capacity = 10;
  std::queue<int> q_;
  std::mutex mtx_;
  std::condition_variable cv_nofull_;
  std::condition_variable cv_noempty_;
public:
  void push(int data)
  {
    std::unique_lock<std::mutex> lk(mtx_);
    cv_nofull_.wait(lk, [&]{
      return (q_.size() < capacity);
    });
    bool do_signal = q_.empty();
    q_.push(data);
    if (do_signal)
      cv_noempty_.notify_one();
  }
  int pop()
  {
    std::unique_lock<std::mutex> lk(mtx_);
    cv_noempty_.wait(lk, [&]{
      return !q_.empty();
    });
    bool do_signal = (q_.size() == capacity);
    int data = q_.front();
    q_.pop();
    if (do_signal)
      cv_nofull_.notify_one();
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
