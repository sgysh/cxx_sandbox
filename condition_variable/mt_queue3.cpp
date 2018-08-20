// Boost Software License 1.0
// Copyright (c) 2014 yohhoy

// push/pop + close + abort
#include <queue>
#include <exception>
#include <thread>
#include <mutex>
#include <condition_variable>

struct closed_queue : std::exception {};
struct abort_exception : std::exception {};

class mt_queue {
  static const int capacity = 10;
  std::queue<int> q_;
  std::mutex mtx_;
  std::condition_variable cv_nofull_;
  std::condition_variable cv_noempty_;
  bool closed_ = false;
  bool aborted_ = false;
public:
  void push(int data)
  {
    std::unique_lock<std::mutex> lk(mtx_);
    cv_nofull_.wait(lk, [&]{
      return (q_.size() < capacity) || closed_ || aborted_;
    });
    if (closed_)
      throw closed_queue();
    if (aborted_)
      throw abort_exception();
    bool do_signal = q_.empty();
    q_.push(data);
    if (do_signal)
      cv_noempty_.notify_one();
  }
  bool pop(int& data)
  {
    std::unique_lock<std::mutex> lk(mtx_);
    cv_noempty_.wait(lk, [&]{
      return !q_.empty() || (q_.empty() && closed_) || aborted_;
    });
    if (q_.empty() && closed_)
      return false;  // closed queue
    if (aborted_)
      throw abort_exception();
    bool do_signal = (q_.size() == capacity);
    data = q_.front();
    q_.pop();
    if (do_signal)
      cv_nofull_.notify_one();
    return true;
  }
  void close()
  {
    std::lock_guard<std::mutex> lk(mtx_);
    closed_ = true;
    cv_nofull_.notify_all();
    cv_noempty_.notify_all();
  }
  void abort()
  {
    std::lock_guard<std::mutex> lk(mtx_);
    aborted_ = true;
    cv_nofull_.notify_all();
    cv_noempty_.notify_all();
  }
};


//----------------------------------------------------------
#include <iostream>
const int N = 100;

int main()
{
  mt_queue q;
  std::thread th1([&]{
    try {
      for (int i = 1; i <= N; ++i) {
        q.push(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      q.close();  // end of data
    } catch (closed_queue&) {
      std::cout << "closed queue" << std::endl;
    } catch (abort_exception&) {
      std::cout << "abort producer" << std::endl;
    }
  });
  std::thread th2([&]{
    try {
      int v;
      while (q.pop(v)) {
        std::cout << v << std::endl;
      }
      std::cout << "(EOD)" << std::endl;
    } catch (abort_exception&) {
      std::cout << "abort consumer" << std::endl;
    }
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  // q.close();
  q.abort();
  th1.join();
  th2.join();
}
