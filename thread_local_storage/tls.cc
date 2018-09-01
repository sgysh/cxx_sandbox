#include <iostream>
#include <thread>

int countup()
{
  thread_local int thread_local_counter = 0; /* implicitly static */
  return thread_local_counter++;
}

int main(void) {
  std::thread thread1([]{
    for (auto i = 0; i < 4; ++i) {
      auto count = countup();
      std::cout << "thread1 " << count << std::endl;
    }
  });

  std::thread thread2([]{
    for (auto i = 0; i < 4; ++i) {
      auto count = countup();
      std::cout << "thread2 " << count << std::endl;
    }
  });

  thread1.join();
  thread2.join();
}
