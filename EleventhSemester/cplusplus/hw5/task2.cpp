#include <utility>
#include <type_traits>
#include <cinttypes>

template<typename... TArgs>
struct are_same;

template<typename T1, typename T2>
struct are_same<T1, T2> : std::is_same<T1, T2> {
};

template<typename T1, typename T2, typename... TArgs>
struct are_same<T1, T2, TArgs...> :
  std::integral_constant<bool, std::is_same<std::decay<T1>, std::decay<T2>>::value && are_same<T2, TArgs...>::value> {
};

template<typename... TArgs>
static constexpr bool
are_same_v = are_same<TArgs...>::value;

int
main()
{
  static_assert(are_same_v<int, int32_t, signed int>, "compile assert");
  static_assert(are_same_v<int, int32_t, long>, "compile assert");
  static_assert(are_same_v<long, int32_t, signed int>, "compile assert");
}
