#include "part2.h"

Shop::Shop(const std::string &_name) :
  name(_name),
  self()
{
}

std::shared_ptr<Shop>
Shop::createShop(const std::string &name)
{
  auto shop = std::shared_ptr<Shop>(new Shop(name));
  shop->self = shop;
  return shop;
}

std::weak_ptr<IShop>
Shop::GetShared()
{
  return this->self;
}
