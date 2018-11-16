import numpy as np
from collections import Counter


def compute_bias_variance(regressor, dependence_fun, x_generator=np.random.uniform, noise_generator=np.random.uniform,
                          sample_size=300, samples_num=100, objects_num=200, seed=1234):
    """
    После генерации всех необходимых объектов, должна вызываться функция compute_bias_variance_fixed_samples.

    Рекомендации:
    * Создайте вектор объектов для оценивания интеграла по $x$, затем вектор зашумленных правильных ответов.
      Оцените мат. ожидание шума с помощью генерации отдельной шумовой выборки длины objects_num.
    * Проверить правильность реализации можно на примерах, которые разбирались на семинаре и в домашней работе.

    :param regressor: объект sklearn-класса, реализующего регрессионный алгоритм (например, DecisionTreeRegressor,
     LinearRegression, Lasso, RandomForestRegressor ...)
    :param dependence_fun: функция, задающая истинную зависимость в данных. Принимает на вход вектор и возвращает вектор
     такой же длины. Примеры: np.sin, lambda x: x**2
    :param x_generator: функция, генерирующая одномерную выборку объектов и имеющая параметр size (число объектов в
     выборке). По умолчанию np.random.uniform
    :param noise_generator: функция, генерирующая одномерную выборку шумовых компонент (по одной на каждый объект) и
     имеющая параметр size (число объектов в выборке). По умолчанию np.random.uniform
    :param sample_size: число объектов в выборке
    :param samples_num: число выборок, которые нужно сгенерировать, чтобы оценить интеграл по X
    :param objects_num: число объектов, которые нужно сгенерировать, чтобы оценить интеграл по x
    :param seed: seed для функции np.random.seed

    :return bias: смещение алгоритма regressor (число)
    :return variance: разброс алгоритма regressor (число)
    """
    np.random.seed(seed)

    samples = x_generator(size=(samples_num, sample_size))
    noise = noise_generator(size=(samples_num, sample_size))
    objects = x_generator(size=objects_num)
    mean_noise = noise_generator(size=objects_num).mean()

    return compute_bias_variance_fixed_samples(regressor=regressor, dependence_fun=dependence_fun,
                                               samples=samples, objects=objects, noise=noise,
                                               mean_noise=mean_noise)


def compute_bias_variance_fixed_samples(regressor, dependence_fun, samples, objects, noise, mean_noise):
    """
    В качестве допущения, будем оценивать $E_X\left[\mu(X)\right](x)$ как средний ответ на $x$ из samples_num
    алгоритмов, обученных на своих подвыборках $X$

    Рекомендации:
    * $\mathbb{E}[y|x]$ оценивается как сумма правильного ответа на объекте и мат. ожидания шума
      $\mathbb{E}_X [\mu(X)]$ оценивается как в предыдущей задаче: нужно обучить regressor на samples_num выборках длины
       sample_size и усреднить предсказания на сгенерированных ранее объектах.

    :param regressor: объект sklearn-класса, реализующего регрессионный алгоритм (например, DecisionTreeRegressor,
     LinearRegression, Lasso, RandomForestRegressor ...)
    :param dependence_fun: функция, задающая истинную зависимость в данных. Принимает на вход вектор и возвращает вектор
     такой же длины. Примеры: np.sin, lambda x: x**2
    :param samples: samples_num выборк длины sample_size для оценки интеграла по X
    :param objects: objects_num объектов для оценки интеграла по x
    :param noise: шумовая компонента размерности (samples_num, sample_size)
    :param mean_noise: среднее шумовой компоненты

    :return bias: смещение алгоритма regressor (число)
    :return variance: разброс алгоритма regressor (число)
    """
    y_train = dependence_fun(samples) + noise
    y_test = dependence_fun(objects) + mean_noise
    y_pred = []

    for x_sample, y_sample in zip(samples, y_train):
        regressor.fit(x_sample[:, np.newaxis], y_sample)
        y_pred.append(regressor.predict(objects[:, np.newaxis]))

    y_pred = np.array(y_pred)
    mean_y_pred = y_pred.mean(axis=0)
    bias = ((y_test - mean_y_pred) ** 2).mean()
    variance = ((y_pred - mean_y_pred) ** 2).mean()
    return bias, variance


def find_best_split(feature_vector, target_vector):
    """
    Под критерием Джини здесь подразумевается следующая функция:
    $$Q(R) = -\frac {|R_l|}{|R|}H(R_l) -\frac {|R_r|}{|R|}H(R_r)$$,
    $R$ — множество объектов, $R_l$ и $R_r$ — объекты, попавшие в левое и правое поддерево,
     $H(R) = 1-p_1^2-p_0^2$, $p_1$, $p_0$ — доля объектов класса 1 и 0 соответственно.

    Указания:
    * Пороги, приводящие к попаданию в одно из поддеревьев пустого множества объектов, не рассматриваются.
    * В качестве порогов, нужно брать среднее двух сосдених (при сортировке) значений признака
    * Поведение функции в случае константного признака может быть любым.
    * При одинаковых приростах Джини нужно выбирать минимальный сплит.
    * За наличие в функции циклов балл будет снижен. Векторизуйте! :)

    :param feature_vector: вещественнозначный вектор значений признака
    :param target_vector: вектор классов объектов,  len(feature_vector) == len(target_vector)

    :return thresholds: отсортированный по возрастанию вектор со всеми возможными порогами, по которым объекты можно
     разделить на две различные подвыборки, или поддерева
    :return ginis: вектор со значениями критерия Джини для каждого из порогов в thresholds len(ginis) == len(thresholds)
    :return threshold_best: оптимальный порог (число)
    :return gini_best: оптимальное значение критерия Джини (число)
    """
    size = feature_vector.shape[0]
    sort_indeces = np.argsort(feature_vector)
    feature_vector = feature_vector[sort_indeces]
    target_vector = target_vector[sort_indeces]

    class_number = target_vector.max() + 1
    r_border_ids = np.where(feature_vector[:-1] != feature_vector[1:])[0]
    thresholds = (feature_vector[r_border_ids] + feature_vector[r_border_ids + 1]) / 2

    if len(r_border_ids) == 0:
        return None, None, None, None

    # Готовим данные для подсчета границ
    class_count = np.zeros((size, class_number))
    for i in range(class_number):
        class_count[:, i] = np.cumsum(target_vector == i)

    # Для каждой валидной границы находим число классов и размер
    l_class_count = class_count[r_border_ids]
    l_p_class = l_class_count / l_class_count.sum(axis=1)[:, np.newaxis]

    r_class_count = class_count[-1] - l_class_count
    r_p_class = r_class_count / r_class_count.sum(axis=1)[:, np.newaxis]

    h_l = 1 - (l_p_class ** 2).sum(axis=1)
    h_r = 1 - (r_p_class ** 2).sum(axis=1)

    # Считаем gain и находим минимальный
    ginis = - (r_border_ids + 1) / size * h_l - (size - r_border_ids - 1) / size * h_r
    idx = np.argmax(ginis)

    # Возвращаем минимальный gain и соответствующий threshold
    return thresholds, ginis, thresholds[idx], ginis[idx]


class DecisionTree:
    def __init__(self, feature_types, max_depth=None, min_samples_split=None, min_samples_leaf=None):
        if np.any(list(map(lambda x: x != "real" and x != "categorical", feature_types))):
            raise ValueError("There is unknown feature type")

        self._tree = {}
        self._feature_types = feature_types
        self._max_depth = max_depth
        self._min_samples_split = min_samples_split
        self._min_samples_leaf = min_samples_leaf

    def _fit_node(self, sub_X, sub_y, node, depth):
        if np.all(sub_y == sub_y[0]) or \
           ((self._min_samples_split is not None) and (sub_y.shape[0] < self._min_samples_split)) or \
           ((self._max_depth is not None) and (depth > self._max_depth)):
            node["type"] = "terminal"
            node["class"] = Counter(sub_y).most_common(1)[0][0]
            return

        feature_best, threshold_best, gini_best, split = None, None, None, None

        for feature in range(sub_X.shape[1]):
            feature_type = self._feature_types[feature]
            categories_map = {}

            if feature_type == "real":
                feature_vector = sub_X[:, feature]
            elif faeture_type == "categorical":
                counts = Counter(sub_X[:, feature])
                clicks = Counter(sub_X[sub_y == 1, feature])
                ratio = {}

                for key, current_count in counts.items():
                    if key in clicks:
                        current_click = clicks[key]
                    else:
                        current_click = 0
                    ratio[key] = float(current_click) / current_count
                sorted_categories = list(map(lambda x: x[0], sorted(ratio.items(), key=lambda x: x[1])))
                categories_map = dict(zip(sorted_categories, list(range(len(sorted_categories)))))

                feature_vector = np.array(list(map(lambda x: categories_map[x], sub_X[:, feature])))
            else:
				raise ValueError()

            _, _, threshold, gini = find_best_split(feature_vector, sub_y)

            if (gini is not None) and (gini_best is None or gini > gini_best):
                split = feature_vector < threshold

                if ((self._min_samples_leaf is not None) and
                    (split.sum() < self._min_samples_leaf or
                     np.logical_not(split).sum() < self._min_samples_leaf)):
                    continue

                feature_best = feature
                gini_best = gini
                split_best = split

                if feature_type == "real":
                    threshold_best = threshold
                elif feature_type == "categorical":
                    threshold_best = [key for key, value in categories_map.items() if value < threshold]
                else:
					raise ValueError()

        if feature_best is None:
            node["type"] = "terminal"
            node["class"] = Counter(sub_y).most_common(1)[0][0]
            return

        node["type"] = "nonterminal"
        node["feature"] = feature_best
        node["threshold"] = threshold_best

        node["left_child"], node["right_child"] = {}, {}
        self._fit_node(sub_X[split_best], sub_y[split_best], node["left_child"], depth + 1)
        self._fit_node(sub_X[np.logical_not(split_best)], sub_y[np.logical_not(split_best)],
                       node["right_child"], depth + 1)

    def _predict_node(self, x, node):
        if node['type'] == 'terminal':
            return node['class']

        feature = node['feature']
        feature_type = self._feature_types[feature]
        if (feature_type == 'real' and x[feature] < node['threshold']) or \
           (feature_type == 'categorical' and x[feature] in node['threshold']):
            return self._predict_node(x, node['left_child'])
        else:
            return self._predict_node(x, node['right_child'])

    def fit(self, X, y):
        y = y.astype(np.int32)
        self._fit_node(X, y, self._tree, 1)

    def predict(self, X):
        predicted = []
        for x in X:
            predicted.append(self._predict_node(x, self._tree))
        return np.array(predicted)

    def get_params(self, *args, **kwargs):
        return {
            'min_samples_split': self._min_samples_split,
            'min_samples_leaf': self._min_samples_leaf,
            'feature_types': self._feature_types,
            'max_depth': self._max_depth,
        }
