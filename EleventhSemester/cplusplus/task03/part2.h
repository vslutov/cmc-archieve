#ifndef PART2_HEADER
#define PART2_HEADER

#include <memory>
#include <string>
#include <optional>
#include <map>

struct IShop {
  virtual ~IShop() = default;

  virtual std::weak_ptr<IShop>
  GetWeakPtr() = 0;

  virtual std::optional<double>
  GetPrice(const std::string &) = 0;

  virtual void
  Notify(const std::string &, const std::optional<double> &) = 0;

  virtual std::string
  GetPriceList() = 0;
};

struct IProduct {
  virtual ~IProduct() = default;

  virtual void
  Attach(const std::weak_ptr<IShop> &) = 0;

  virtual void
  Detach(const std::weak_ptr<IShop> &) = 0;

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
  std::weak_ptr<IShop> self;
  std::map<std::string, double> products;

  Shop(const std::string &_name);

public:
  const std::string name;

  static std::shared_ptr<Shop>
  CreateShop(const std::string &name);

  virtual std::weak_ptr<IShop>
  GetWeakPtr() override;

  virtual std::optional<double>
  GetPrice(const std::string &) override;

  virtual void
  Notify(const std::string &, const std::optional<double> &) override;

  virtual std::string
  GetPriceList() override;
};

struct Product : IProduct {
  Product(const std::string &, double);

  virtual void
  Attach(const std::weak_ptr<IShop> &) override;

  virtual void
  Detach(const std::weak_ptr<IShop> &) override;

  virtual double
  GetPrice() override;

  virtual void
  StartSales() override;

  virtual void
  StopSales() override;

  virtual void
  ChangePrice(double) override;
};

#endif
