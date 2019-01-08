#include <cstddef>

template <typename T, size_t N>
constexpr auto
Size(const T (&)[N]) noexcept
{
  return N;
}

int
main()
{
  int a[5];
  double b[Size(a)];
}
