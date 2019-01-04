# cpp_task3

Решение задания 3 курса C++

Первая часть решения содержится в файле [part1.cpp](part1.cpp).
В ней добавлены примитивы синхронизации: mutex, condition variable.

Вторая часть задания разбита на 4 файла:
- [part2.h](part2.h) и [part2.cpp](part2.cpp) - интерфейс и реализация абстрактных и конкретных магазинов и товаров.
- [part2-test.cpp](part2-test.cpp) - тесты к коду на GTest.
- [part2-main.cpp](part2-main.cpp) - многопоточный код, демонстрирующий работу программы.

Собираются программы с помощью cmake.

GTest настраивал по [этой инструкции](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/).
Тестировал программы на Ubuntu 16.04 с gcc 7.4.0.
