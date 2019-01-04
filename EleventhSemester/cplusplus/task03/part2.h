#ifndef PART2_HEADER
#define PART2_HEADER

#include <memory>
#include <string>
#include <optional>
#include <map>
#include <mutex>
#include <vector>

struct IShop {
  virtual ~IShop() = default;

  virtual std::shared_ptr<IShop>
  GetSharedPtr() const = 0;

  virtual std::optional<double>
  GetPrice(const std::string &) const = 0;

  virtual void
  Notify(const std::string &, const std::optional<double> &) = 0;

  virtual std::string
  GetPriceList() const = 0;
};

struct IProduct {
  virtual ~IProduct() = default;

  virtual void
  Attach(const std::shared_ptr<IShop> &) = 0;

  virtual void
  Detach(const std::shared_ptr<IShop> &) = 0;

  virtual double
  GetPrice() const = 0;

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
  mutable std::mutex mutex;

  Shop(const std::string &);

public:
  const std::string name;

  static std::shared_ptr<Shop>
  CreateShop(const std::string &name);

  virtual std::shared_ptr<IShop>
  GetSharedPtr() const override;

  virtual std::optional<double>
  GetPrice(const std::string &) const override;

  virtual void
  Notify(const std::string &, const std::optional<double> &) override;

  virtual std::string
  GetPriceList() const override;
};

struct Product : IProduct {
private:
  std::vector<std::weak_ptr<IShop>> observers;
  mutable std::mutex mutex;
  double price;
  bool isSelling;

  void
  SendPriceToShop(const std::optional<double> &) const;

public:
  const std::string name;

  Product(const std::string &, double);

  virtual void
  Attach(const std::shared_ptr<IShop> &) override;

  virtual void
  Detach(const std::shared_ptr<IShop> &) override;

  virtual double
  GetPrice() const override;

  virtual void
  StartSales() override;

  virtual void
  StopSales() override;

  virtual void
  ChangePrice(double) override;
};

#endif
