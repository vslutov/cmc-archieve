#include "part2.h"

#include <algorithm>
#include <sstream>

Shop::Shop(const std::string &_name) :
  self(),
  products(),
  mutex(),
  name(_name)
{
}

std::shared_ptr<Shop>
Shop::CreateShop(const std::string &name)
{
  auto shop = std::shared_ptr<Shop>(new Shop(name));
  shop->self = shop;
  return shop;
}

std::shared_ptr<IShop>
Shop::GetSharedPtr() const
{
  return this->self.lock();
}

std::optional<double>
Shop::GetPrice(const std::string &product) const
{
  std::lock_guard<std::mutex> lock(this->mutex);

  try {
    return this->products.at(product);
  } catch (const std::out_of_range &) {
    return std::nullopt;
  }
}

void
Shop::Notify(const std::string &product, const std::optional<double> &price)
{
  std::lock_guard<std::mutex> lock(this->mutex);

  if (price.has_value()) {
    this->products[product] = price.value();
  } else {
    this->products.erase(product);
  }
}

std::string
Shop::GetPriceList() const
{
  std::lock_guard<std::mutex> lock(this->mutex);

  std::stringstream ss;
  ss.setf(std::ios::fixed, std::ios::floatfield);
  ss.precision(2);

  ss << this->name << std::endl << std::endl;

  for (const auto &product : this->products) {
    ss << product.first << ": " << product.second << std::endl;
  }

  return ss.str();
}

Product::Product(const std::string &_name, double _price) :
  observers(),
  mutex(),
  price(_price),
  isSelling(false),
  name(_name)
{
}

void
Product::Attach(const std::shared_ptr<IShop> &shop)
{
  std::lock_guard<std::mutex> lock(this->mutex);

  if (bool(shop)) {
    if (this->isSelling) {
      shop->Notify(this->name, this->price);
    }

    this->observers.push_back(shop);
  }
}

void
Product::Detach(const std::shared_ptr<IShop> &shop)
{
  std::lock_guard<std::mutex> lock(this->mutex);

  if (bool(shop)) {
    shop->Notify(this->name, std::nullopt);

    this->observers.erase(
      std::remove_if(
        this->observers.begin(),
        this->observers.end(),
        [&](const std::weak_ptr<IShop>& wptr) {
          return wptr.expired() || wptr.lock() == shop;
        }
      ),
      this->observers.end()
    );
  }
}

double
Product::GetPrice() const
{
  std::lock_guard<std::mutex> lock(this->mutex);

  return this->price;
}

void
Product::StartSales()
{
  std::lock_guard<std::mutex> lock(this->mutex);

  this->isSelling = true;
  this->SendPriceToShop(this->price);
}

void
Product::StopSales()
{
  std::lock_guard<std::mutex> lock(this->mutex);

  this->isSelling = false;
  this->SendPriceToShop(std::nullopt);
}

void
Product::ChangePrice(double _price)
{
  std::lock_guard<std::mutex> lock(this->mutex);

  this->price = _price;
  this->SendPriceToShop(_price);
}

void
Product::SendPriceToShop(const std::optional<double> &_price) const {
  for (const auto &wptr: observers) {
    auto observer = wptr.lock();
    if (bool(observer)) {
      observer->Notify(this->name, _price);
    }
  }
}
