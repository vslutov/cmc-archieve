# Varmethods task02

Active contours

## Prepared by Vladimir Liutov

Run with parameters:

```bash
  ./start.sh
```

Install requirements:

```bash
  pip install -r requirements.txt
```

## Задание 4: Активные контуры

### Обязательная часть задания

Требуется написать программу, реализующую метод сегментации изображения с помощью активных контуров.

Выбор внешней энергии на усмотрение студента, однако он обязательно должен содержать line energy (E_line) и edge energy (E_edge). Наличие balloon force на усмотрение студента.

Рекомендуемое начальное приближение для активного контура предоставляется в виде текстового файла. При желании, его можно менять. Также предоставляется набор утилит (utils.py) для конвертации активного контура, представленного массивом точек numpy.array, в бинарную маску.
Требования к программе

Язык реализации — Python 3. Программа либо должна состоять из одного файла, либо иметь главный файл с именем main.py. Задача реализовать метод самостоятельно. Использовать готовые реализации из skimage, opencv и пр. — нельзя. Допустимо использовать библиотеки для базовых вещей (интерполяция, обращение матриц, градиенты, свертки и пр.), общепринятые для обработки изображений (scipy, numpy).

Один и тот же код должен работать для всех входных изображений. Для получения результата можно изменять только параметры. Результатом работы программы является бинарная маска объекта, сохраненная в png файл, где белое (255) — это объект, а черное (0) — это фон. Готовое решение должно быть либо загружено на сайт, либо отправлено на почту khvostikov@cs.msu.ru.

### Критерии оценки

Уже хорошо, если программа выдаст результат (маску сегментации), и он будет близок по метрике (IoU > 0.7) к ground true для 3 из 5 картинок.

### Рекомендации

К входным картинкам можно (и иногда нужно) применять простейшие фильтры, такие как медиана или Гаусс. Для чтения начального приближения активного контура можно использовать numpy.loadtxt.
Формат параметров командной строки

Программа должна поддерживать запуск из командной строки со строго определённым форматом команд:

    %programname% (input_image) (initial_snake) (output_image) (alpha) (beta) (tau) (w_line) (w_edge) (kappa)

Аргументы:
```
    input_image    Имя файла — входное изображение
    initial_snake  Имя файла с начальным приближением для активного контура
    output_image   Имя файла — выходное изображение
    alpha          Параметр alpha внутренней энергии, отвечающий за растяжимость контура
    beta           Параметр beta внутренней энергии, отвечающий за жесткость контура
    tau            Шаг градиентного спуска
    w_line         Вес слагаемого интенсивности во внешней энергии
    w_edge         Вес слагаемого границ во внешней энергии
    kappa          Вес balloon force (опционально)
```