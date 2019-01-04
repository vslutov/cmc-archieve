#include <iostream>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>

int main() {
  std::atomic<size_t> count = 0;
  std::atomic<bool> done = false;
  std::queue<int> items;
  std::mutex m;
  std::thread producer([&]() {
    for (int i = 0; i < 10000; ++i) {
      // ... some code may be here ...
      {
        std::lock_guard<std::mutex> lock(m);
        items.push(i);
      }
      ++count;
    }
    done = true;
  });
  std::thread consumer([&]() {
    while (!done.load()) {
      while (!items.empty()) {
        {
          std::lock_guard<std::mutex> lock(m);
          items.pop();
        }
        // ...
        --count;
      }
    }
  });
  producer.join();
  consumer.join();
  std::cout << count << std::endl;
}
