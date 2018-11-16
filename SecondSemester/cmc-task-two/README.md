#cmc-task-two

check two polynomials equivalence

## LICENSE

    cmc-task-one - equation solving and definite integrals
    Copyright (C) 2014  V. S. Lutov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Отчет

    МОСКОВСКИЙ ГОСУДАРСТВЕННЫЙ УНИВЕРСИТЕТ
    имени. М. В. ЛОМОНОСОВА
    ФАКУЛЬТЕТ ВЫЧИСЛИТЕЛЬНОЙ МАТЕМАТИКИ И КИБЕРНЕТИКИ

    Исполнитель:
    студент 104 группы
    Лютов Владимир Сергеевич

    Преподаватель:
    Гуляев Анатолий Викторович

    Вариант:
    14

### Тема

Проверка двух многочленов над полем Z на равенство

### Содержание

- [Постановка задачи](#Постановка-задачи)
- [Структура программы на Си](#Структура-программы-на-Си)
- [Тестирование](#Тестирование)
- [Исходный код](#Исходный-код)
- [Полученные результаты](#Полученные-результаты)
- [Анализ ошибок](#Анализ-ошибок)

### Постановка задачи

На этот раз у нас довольно простая задача - проверить, что мы не забыли
навыки написания процедур для работы со списками, таких как добавление
элементов, сравнение списков и их сортировка.

#### Строгая постановка задачи

Даны два символьных многочлена (последовательность одночленов особого вида).

Выяснить равны ли эти два многочлена.

Два многочлена считаются равными, если один можно получить из другого
перестановкой одночленов.

Одночлены могут выглядеть так:

    aX^k
    X^k
    aX
    X
    a

Где `a` и `k` - некие натуральные числа.

### Структура программы на Си

В заголовочном файле [src/polynomial.h](src/polynomial.h) описаны основные
типы и функции, которые понадобятся программисту для решения этой задачи.
Они скрыты *под капотом* благодаря типу **Polynomial** с которым научены
работать функции из этой библиотеки.

Непосредственно в файле исходного кода [src/polynomial.c](src/polynomial.c)
описаны дополнительные функции, такие как сортировка и прочая работа со
списками.

### Тестирование

Меньше слов.

<table><thead><tr><td>First polynomial</td><td>Second polynomial</td><td>Is equal?</td></tr></thead><tbody><tr><td>X-5</td><td>-5+X</td><td>YES</td></tr><tr><td>X</td><td>X</td><td>YES</td></tr><tr><td>-X</td><td>-X</td><td>YES</td></tr><tr><td>-X</td><td>X</td><td>NO</td></tr><tr><td>X-3X^2</td><td>X^48</td><td>NO</td></tr><tr><td>42</td><td>-5+X^11</td><td>NO</td></tr><tr><td>1+X^11</td><td>5+X^11</td><td>NO</td></tr><tr><td>5+3X^12+X^4</td><td>3X^12+5+X^4</td><td>YES</td></tr><tr><td>X-10+X^3-X^4</td><td>-10+X^3-X^4+X</td><td>YES</td></tr></tbody></table>

### Исходный код

Исходный код доступен по стандартной общественной лицензии GNU на этой странице
[https://github.com/vslutov/cmc-task-one](https://github.com/vslutov/cmc-task-one)

### Анализ ошибок

В процессе разработке были подтверждены следующие тезисы:

- С указателями на Си очень сложно работать
- Изменяемые состояния - зло
- Самописные контейнеры - зло

