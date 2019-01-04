#include "part2.h"

#include <sstream>

Shop::Shop(const std::string &_name) :
  self(),
  products(),
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

std::weak_ptr<IShop>
Shop::GetWeakPtr()
{
  return this->self;
}

std::optional<double>
Shop::GetPrice(const std::string &product) {
  try {
    return this->products.at(product);
  } catch (const std::out_of_range &) {
    return std::nullopt;
  }
}

void
Shop::Notify(const std::string &product, const std::optional<double> &price)
{
  if (price.has_value()) {
    this->products[product] = price.value();
  } else {
    this->products.erase(product);
  }
}

std::string
Shop::GetPriceList() {
  std::stringstream ss;
  ss.setf(std::ios::fixed, std::ios::floatfield);
  ss.precision(2);

  ss << this->name << std::endl << std::endl;

  for (const auto &product : this->products) {
    ss << product.first << ": " << product.second << std::endl;
  }

  return ss.str();
}
