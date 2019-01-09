#include <string>
#include <utility>

template <typename T>
struct Any {
  T val;
public:
  Any(T t) :
    val(std::move(t))
  {
  }

  template<typename O,
           typename = std::enable_if_t<std::is_same<T, O>::value>
          >
  T
  get()
  {
    return val;
  }
};

int
main()
{
  Any<int> a(5);
  a.get<int>();
  a.get<std::string>();
}
