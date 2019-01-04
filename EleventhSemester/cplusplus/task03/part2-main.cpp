#include "part2.h"

#include <thread>
#include <iostream>

int
main()
{
  auto shopPtr1 = Shop::CreateShop("Shop 1");
  auto shopPtr2 = Shop::CreateShop("Shop 2");
  auto shopPtr3 = Shop::CreateShop("Shop 3");

  std::thread producer([&]() {
    Product prod1("A", 15);
    prod1.StartSales();
    prod1.Attach(shopPtr1);
    prod1.Attach(shopPtr2);
    Product prod2("B", 13);
    prod2.StartSales();
    prod2.Attach(shopPtr3);
    prod2.Attach(shopPtr1);
    prod1.Detach(shopPtr2);
    prod2.Detach(shopPtr3);
    prod2.ChangePrice(12.99);
    prod1.ChangePrice(16);
    Product prod3("C", 45);
    prod3.Attach(shopPtr1);
  });

  std::thread consumer([&]() {
    shopPtr2.reset();
    std::this_thread::sleep_for (std::chrono::seconds(1));
    shopPtr3.reset();
    std::cout << shopPtr1->GetPriceList();
  });

  producer.join();
  consumer.join();
  return 0;
}
