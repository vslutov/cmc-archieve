#ifndef PART2_HEADER
#define PART2_HEADER

#include <memory>
#include <string>

struct IShop {
  virtual ~IShop() = default;

  virtual std::weak_ptr<IShop>
  GetShared() = 0;
};

struct IProduct {
  virtual ~IProduct() = default;

  virtual void
  Attach(IShop *) = 0;

  virtual void
  Detach(IShop *) = 0;

  virtual double
  GetPrice() = 0;

  virtual void
  StartSales() = 0;

  virtual void
  StopSales() = 0;

  virtual void
  ChangePrice(double) = 0;
};

struct Shop : public IShop {
private:
  std::string name;
  std::weak_ptr<IShop> self;

  Shop(const std::string &_name);

public:
  static std::shared_ptr<Shop>
  createShop(const std::string &name);

  std::weak_ptr<IShop>
  GetShared();
};

#endif
