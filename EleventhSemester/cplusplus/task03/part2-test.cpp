#include <gtest/gtest.h>

#include "part2.h"

class TestShop : public testing::Test
{
protected:
  TestShop() :
    shop()
  {
  }

  void
  SetUp()
  {
    this->shop = Shop::CreateShop("first shop");
  }

  void
  TearDown()
  {
    this->shop.reset();
  }

  std::shared_ptr<Shop> shop;
};

TEST_F(TestShop, create) {
  ASSERT_EQ(shop->name, "first shop");
}

TEST_F(TestShop, GetWeakPtr) {
  ASSERT_EQ(shop, shop->GetWeakPtr().lock());
}

TEST_F(TestShop, NotifyAndGetPrice) {
  shop->Notify("A", 10);
  ASSERT_EQ(shop->GetPrice("A"), 10);

  shop->Notify("A", std::nullopt);
  ASSERT_EQ(shop->GetPrice("A"), std::nullopt);
  ASSERT_EQ(shop->GetPrice("B"), std::nullopt);
}

TEST_F(TestShop, GetPriceList) {
  shop->Notify("product 1", 10);
  shop->Notify("product 2", 20);
  ASSERT_EQ(shop->GetPriceList(), "first shop\n\nproduct 1: 10.00\nproduct 2: 20.00\n");
}

int
main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
