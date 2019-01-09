#include <string>
#include <utility>
#include <vector>
#include <algorithm>

template<typename TArg>
decltype(auto)
Compose(TArg FArg)
{
  return FArg;
}

template<typename TArg, typename... TArgs>
decltype(auto)
Compose(TArg f1, TArgs... FArgs)
{
  return [f1=f1,FArgs...](auto &&x) {
    return f1(
      Compose(FArgs...)(
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
