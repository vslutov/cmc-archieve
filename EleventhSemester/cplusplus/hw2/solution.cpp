#include <ctime>
#include <vector>
#include <cassert>
#include <algorithm>

struct TItem {
  int value;
  time_t timestamp;

  TItem(int v)
  : value(v)
  , timestamp(std::time(0)) {}
  // ...
};

using Items = std::vector<TItem>;

template<int... args>
auto
MakeItemsSimple()
{
  return Items{args...};
}

auto
MakePredicate(const Items &items)
{
  return [&items](int v) {
    return std::any_of(items.cbegin(), items.cend(), [v](const TItem &i) {
      return i.value == v;
    });
  };
}

int
main()
{
  Items items = MakeItemsSimple<0, 1, 4, 5, 6>();
  Items newItems = MakeItemsSimple<7, 15, 1>();

  auto isFound = MakePredicate(items);
  auto isFoundNew = MakePredicate(newItems);
  assert(isFound(0) == true);
  assert(isFound(7) == false);
  assert(isFoundNew(7) == true);
  assert(isFoundNew(6) == false);
}
