#include <string>
#include <utility>
#include <vector>
#include <algorithm>

template<typename TArg>
decltype(auto)
Compose(TArg &&FArg)
{
  return std::forward<TArg>(FArg);
}

template<typename TArg, typename... TArgs>
decltype(auto)
Compose(TArg &&f1, TArgs &&... FArgs)
{
  return [&](auto &&x) {
    return std::forward<decltype(f1)>(f1)(
      Compose(std::forward<TArgs>(FArgs)...)(
        std::forward<decltype(x)>(x)
      )
    );
  };
}

const char* f2(const std::string& str) {
  return str.c_str();
}

int main() {
  std::string s[] = {"1.2", "2.343", "3.2"};
  std::vector<double> d(3);
  auto f1 = atof;
  std::transform(s,
    s + 3,
    d.begin(),
    Compose(f1, f2)
  );
  //...
}
