#include <cassert>

template<typename T>
class TBasePtr {
  using pointer = T *;
  using const_pointer = const T *;

  pointer p;

public:
  TBasePtr(pointer  _p) :
    p(_p)
  {
  }

  pointer
  operator->()
  {
    return p;
  }

  const_pointer
  operator->() const
  {
    return p;
  }

  pointer
  operator *()
  {
    return p;
  }

  const_pointer
  operator *() const
  {
    return p;
  }

  friend bool
  operator==(const TBasePtr &lhs, const TBasePtr &rhs)

  friend bool
  operator!=(const TBasePtr &lhs, const TBasePtr &rhs)

  bool
  operator bool() const
  {
    return p != nullptr;
  }
};

template<typename T>
bool
operator==(const TBasePtr<T> &lhs, const TBasePtr<T> &rhs)
{
  return lhs.p == rhs.p;
}

template<typename T>
bool
operator!=(const TBasePtr<T> &lhs, const TBasePtr<T> &rhs)
{
  return lhs.p != rhs.p;
}

// template<typename T>
// class TRefCounter {
// };
// 
// class TDoc: public TRefCounter<TDoc> {
//   using TPtr = TIntrusivePtr<TDoc>;
//   //...
// }

int
main()
{
  auto a = new TBasePtr<int>
  // TDoc* p = new TDoc;
  // TIntrusivePtr<TDoc> ip1(p);
  // TIntrusivePtr<TDoc> ip2(p);
}
