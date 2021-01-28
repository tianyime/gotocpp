#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>

class WaitGroup {
public:
  WaitGroup() : count{0} { std::cout << "WaigGroup()" << std::endl; };
  ~WaitGroup() { std::cout << "~WaigGroup()" << std::endl; };
  WaitGroup(const WaitGroup &) = delete;
  WaitGroup &operator=(const WaitGroup &) = delete;

  void Add() {
    std::cout << "Add()" << std::endl;
    count++;
    return;
  }

  void Done() {
    std::cout << "Done()" << std::endl;
    count--;
    if (count <= 0) {
      cv.notify_all();
    }
    return;
  }

  void Wait() {
    std::unique_lock<std::mutex> lck(mx);
    cv.wait(lck);
    std::cout << "Wait finish" << std::endl;
    return;
  }

private:
  std::atomic<int> count;
  std::mutex mx;
  std::condition_variable cv;
};

void f(std::shared_ptr<WaitGroup> wg) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(2000ms);
  wg->Done();
  return;
}

int main() {
  std::cout << "Hello, WaitGroup!" << std::endl;
  std::shared_ptr<WaitGroup> wg = std::make_shared<WaitGroup>();

  for (int i = 0; i < 5; i++) {
    wg->Add();
    std::thread t(f, wg);
    t.detach();
  }

  wg->Wait();
  return 0;
}