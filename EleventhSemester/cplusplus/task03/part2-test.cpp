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

TEST_F(TestShop, create)
{
  ASSERT_EQ(shop->name, "first shop");
}

TEST_F(TestShop, GetSharedPtr)
{
  ASSERT_EQ(shop, shop->GetSharedPtr());
}

TEST_F(TestShop, NotifyAndGetPrice)
{
  shop->Notify("A", 10);
  ASSERT_EQ(shop->GetPrice("A"), 10);

  shop->Notify("A", std::nullopt);
  ASSERT_EQ(shop->GetPrice("A"), std::nullopt);
  ASSERT_EQ(shop->GetPrice("B"), std::nullopt);
}

TEST_F(TestShop, GetPriceList)
{
  shop->Notify("product 1", 10);
  shop->Notify("product 2", 20);
  ASSERT_EQ(shop->GetPriceList(), "first shop\n\nproduct 1: 10.00\nproduct 2: 20.00\n");
}

class TestProduct : public testing::Test
{
protected:
  TestProduct() :
    shop(),
    product()
  {
  }

  void
  SetUp()
  {
    this->shop = Shop::CreateShop("first shop");
    this->product = std::make_shared<Product>("A", 10);
  }

  void
  TearDown()
  {
    this->shop.reset();
    this->product.reset();
  }

  std::shared_ptr<Shop> shop;
  std::shared_ptr<Product> product;
};

TEST_F(TestProduct, GetPrice)
{
  ASSERT_EQ(product->GetPrice(), 10);
}

TEST_F(TestProduct, AttachBeforeSales)
{
  product->Attach(shop);
  ASSERT_EQ(shop->GetPrice("A"), std::nullopt);
  product->StartSales();
  ASSERT_EQ(shop->GetPrice("A"), 10);
}

TEST_F(TestProduct, AttachAfterSales)
{
  product->StartSales();
  product->Attach(shop);
  ASSERT_EQ(shop->GetPrice("A"), 10);
}

TEST_F(TestProduct, AttachToNull)
{
  product->Attach(nullptr);
}

TEST_F(TestProduct, Detach)
{
  product->StartSales();
  product->Attach(shop);
  product->Detach(shop);
  ASSERT_EQ(shop->GetPrice("A"), std::nullopt);
}

TEST_F(TestProduct, ChangePrice)
{
  product->StartSales();
  product->Attach(shop);
  product->ChangePrice(15);
  ASSERT_EQ(shop->GetPrice("A"), 15);
}

TEST_F(TestProduct, MultipleShops)
{
  auto shopB = Shop::CreateShop("second shop");
  product->StartSales();
  product->Attach(shop);
  product->Attach(shopB);
  product->Detach(shop);
  ASSERT_EQ(shop->GetPrice("A"), std::nullopt);
  ASSERT_EQ(shopB->GetPrice("A"), 10);
  product->ChangePrice(15);
  ASSERT_EQ(shop->GetPrice("A"), std::nullopt);
  ASSERT_EQ(shopB->GetPrice("A"), 15);
}

TEST_F(TestProduct, WorkWithFalseShops)
{
  auto shopB = std::shared_ptr<Shop>();
  product->Attach(shopB);

  ASSERT_TRUE(shop.unique());
  shop.reset();
  product->ChangePrice(15);
}

int
main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
