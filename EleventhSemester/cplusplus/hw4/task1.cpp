#include <memory>
#include <vector>
#include <iostream>

template <typename T>
struct MyAlloc {
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  
  pointer
  allocate(size_t n)
  {
    std::cout << "Get " << n << " elems" << std::endl;
    return static_cast<pointer>(::operator new (sizeof(value_type) * n));
  }

  
  void
  deallocate(pointer p, size_t n)
  {
    std::cout << "Release " << n << " elems" << std::endl;
    return ::operator delete (static_cast<void *>(p));
  }
};

int
main() {
  std::vector<int, MyAlloc<int>> vec;
  std::cout << "Init vec" << std::endl;
  for (int i = 1; i < 10; ++ i) {
    std::cout << "Push " << i << std::endl;
    vec.push_back(i);
    std::cout << "Pushed " << i << std::endl;
  }
  vec.clear();
  std::cout << "Cleared vec" << std::endl;
}
