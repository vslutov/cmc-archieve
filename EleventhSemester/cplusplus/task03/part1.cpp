#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

int main() {
  size_t count = 0;
  bool done = false;
  std::queue<int> items;
  std::mutex m;
  std::condition_variable cv;

  std::thread producer([&]() {
    for (int i = 0; i < 10000; ++i) {
      // ... some code may be here ...
      {
        std::lock_guard<std::mutex> lock(m);
        items.push(i);
        ++count;
        cv.notify_one();
      }
    }

    {
      std::lock_guard<std::mutex> lock(m);
      done = true;
      cv.notify_one();
    }
  });

  std::thread consumer([&]() {
    std::unique_lock<std::mutex> lock(m);

    while (true) {
      cv.wait(lock, [&]() {
        return !items.empty() || done;
      });

      while (!items.empty()) {
        items.pop();
        lock.unlock();
        // ...
        lock.lock();
        --count;
      }

      if (done) {
        break;
      }
    }
  });

  producer.join();
  consumer.join();
  std::cout << count << std::endl;
}
