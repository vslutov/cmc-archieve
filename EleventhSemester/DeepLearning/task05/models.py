import torch
from torch import nn
from torch.nn import functional as F
import math

"""
Все тензоры в задании имеют тип данных float32.
"""

class AE(nn.Module):
    def __init__(self, d, D):
        """
        Инициализирует веса модели.
        Вход: d, int - размерность латентного пространства.
        Вход: D, int - размерность пространства объектов.
        """
        super(type(self), self).__init__()
        self.d = d
        self.D = D
        self.encoder = nn.Sequential(
            nn.Linear(self.D, 200),
            nn.LeakyReLU(),
            nn.Linear(200, self.d)
        )
        self.decoder = nn.Sequential(
            nn.Linear(self.d, 200),
            nn.LeakyReLU(),
            nn.Linear(200, self.D),
            nn.Sigmoid()
        )

    def encode(self, x):
        """
        Генерирует код по объектам.
        Вход: x, Tensor - матрица размера n x D.
        Возвращаемое значение: Tensor - матрица размера n x d.
        """
        return self.encoder(x)

    def decode(self, z):
        """
        По матрице латентных представлений z возвращает матрицу объектов x.
        Вход: z, Tensor - матрица n x d латентных представлений.
        Возвращаемое значение: Tensor, матрица объектов n x D.
        """
        return self.decoder(z)

    def batch_loss(self, batch):
        """
        Вычисляет функцию потерь по батчу - усреднение функции потерь
        по объектам батча.
        Функция потерь по объекту- сумма L2-ошибки восстановления по батчу и
        L2 регуляризации скрытых представлений с весом 1.
        Возвращаемое значение должно быть дифференцируемо по параметрам модели (!).
        Вход: batch, Tensor - матрица объектов размера n x D.
        Возвращаемое значение: Tensor, скаляр - функция потерь по батчу.
        """
        representation = self.encode(batch)
        restored = self.decode(representation)
        restored_loss = ((restored - batch) ** 2).sum(1).mean()
        l2_reg = (representation ** 2).sum(1).mean()
        return restored_loss + l2_reg

    def generate_samples(self, num_samples):
        """
        Генерирует сэмплы объектов x. Использует стандартное нормальное
        распределение в пространстве представлений.
        Вход: num_samples, int - число сэмплов, которые надо сгененрировать.
        Возвращаемое значение: Tensor, матрица размера num_samples x D.
        """
        device = next(iter(self.parameters())).device
        representation = torch.normal(torch.zeros(num_samples, self.d), torch.ones(num_samples, self.d)).to(device)
        return self.decode(representation).cpu()

EPS = 1e-6

def log_mean_exp(data):
    """
    Возвращает логарифм среднего по последнему измерению от экспоненты данной матрицы.
    Подсказка: не забывайте про вычислительную стабильность!
    Вход: mtx, Tensor - тензор размера n_1 x n_2 x ... x n_K.
    Возвращаемое значение: Tensor, тензор размера n_1 x n_2 x ,,, x n_{K - 1}.
    """
    return torch.logsumexp(data, -1) - math.log(data.shape[-1])

def log_likelihood(x_true, x_distr):
    """
    Вычисляет логарфм правдоподобия объектов x_true для индуцированного
    моделью покомпонентного распределения Бернулли.
    Каждому объекту из x_true соответствуют K сэмплированных распределений
    на x из x_distr.
    Требуется вычислить оценку логарифма правдоподобия для каждого объекта.
    Подсказка: не забывайте про вычислительную стабильность!
    Подсказка: делить логарифм правдоподобия на число компонент объекта не надо.

    Вход: x_true, Tensor - матрица объектов размера n x D.
    Вход: x_distr, Tensor - тензор параметров распределений Бернулли
    размера n x K x D.
    Выход: Tensor, матрица размера n x K - оценки логарифма правдоподобия
    каждого сэмпла.
    """
    n, K, D = x_distr.shape
    x_distr = x_distr.clamp(EPS, 1 - EPS)
    x_log_dist = x_distr.log()
    m_x_log_dist = (1 - x_distr).log()
    return ((x_log_dist * x_true.view(n, 1, D)) + (m_x_log_dist * (1 - x_true).view(n, 1, D))).sum(2)

def kl(p_distr, q_distr):
    """
    Вычисляется KL-дивергенция KL(q || p) между n парами гауссиан.
    Вход: q_distr, tuple(Tensor, Tensor). Каждый Tensor - матрица размера n x d.
    Первый - mu, второй - sigma.
    Вход: p_distr, tuple(Tensor, Tensor). Аналогично.
    Возвращаемое значение: Tensor, вектор размерности n, каждое значение которого - 
    - KL-дивергенция между соответствующей парой распределений.
    """
    p_mu, p_sigma = p_distr
    q_mu, q_sigma = q_distr
    return (q_sigma.log()
            - p_sigma.log()
            + (p_sigma ** 2 + (p_mu - q_mu) ** 2) / (2 * q_sigma ** 2)
            - 0.5
           ).sum(1)

class VAE(nn.Module):
    def __init__(self, d, D):
        """
        Инициализирует веса модели.
        Вход: d, int - размерность латентного пространства.
        Вход: D, int - размерность пространства объектов.
        """
        super(type(self), self).__init__()
        self.d = d
        self.D = D
        self.proposal_network = nn.Sequential(
            nn.Linear(self.D, 200),
            nn.LeakyReLU(),
        )
        self.proposal_mu_head = nn.Linear(200, self.d)
        self.proposal_sigma_head = nn.Linear(200, self.d)
        self.generative_network = nn.Sequential(
            nn.Linear(self.d, 200),
            nn.LeakyReLU(),
            nn.Linear(200, self.D),
            nn.Sigmoid()
        )

    def proposal_distr(self, x):
        """
        Генерирует предложное распределение на z.
        Подсказка: областью значений sigma должны быть положительные числа.
        Для этого при генерации sigma следует использовать softplus (!) в качестве
        последнего преобразования.
        Вход: x, Tensor - матрица размера n x D.
        Возвращаемое значение: tuple(Tensor, Tensor),
        Каждый Tensor - матрица размера n x d.
        Первый - mu, второй - sigma.
        """
        device = next(iter(self.parameters())).device
        proposal = self.proposal_network(x.to(device))
        mu = self.proposal_mu_head(proposal)
        sigma = F.softplus(self.proposal_sigma_head(proposal))
        return mu, sigma

    def prior_distr(self, n):
        """
        Генерирует априорное распределение на z.
        Вход: n, int - число распределений.
        Возвращаемое значение: tuple(Tensor, Tensor),
        Каждый Tensor - матрица размера n x d.
        Первый - mu, второй - sigma.
        """
        device = next(iter(self.parameters())).device
        mu = torch.zeros(n, self.d, device=device)
        sigma = torch.ones(n, self.d, device=device)
        return mu, sigma

    def sample_latent(self, distr, K=1):
        """
        Генерирует сэмплы из гауссовского распределения на z.
        Сэмплы должны быть дифференцируемы по параметрам распределения!
        Вход: distr, tuple(Tensor, Tensor). Каждое Tensor - матрица размера n x d.
        Первое - mu, второе - sigma.
        Вход: K, int - число сэмплов для каждого объекта.
        Возвращаемое значение: Tensor, матрица размера n x K x d.
        """
        mu, sigma = distr
        device = mu.device
        n, d = mu.shape
        distr = torch.normal(torch.zeros(n, K, d, device=device), torch.ones(n, K, d, device=device)) 
        return distr * sigma.view(n, 1, d) + mu.view(n, 1, d)

    def generative_distr(self, z):
        """
        По матрице латентных представлений z возвращает матрицу параметров
        распределения Бернулли для сэмплирования объектов x.
        Вход: z, Tensor - тензор n x K x d латентных представлений.
        Возвращаемое значение: Tensor, тензор параметров распределения
        Бернулли размера n x K x D.
        """
        device = next(iter(self.parameters())).device
        n, K, d = z.shape
        result = self.generative_network(z.to(device).view(-1, d)).view(n, K, -1)
        return result

    def batch_loss(self, x_true):
        """
        Вычисляет вариационную нижнюю оценку логарифма правдоподобия по батчу.
        Вариационная нижняя оценка должна быть дифференцируема по параметрам модели (!),
        т. е. надо использовать репараметризацию.
        Требуется вернуть усреднение вариационных нижних оценок объектов батча.
        Вход: batch, FloatTensor - матрица объектов размера n x D.
        Возвращаемое значение: Tensor, скаляр - вариационная нижняя оценка логарифма
        правдоподобия по батчу.
        """
        mu, sigma = self.proposal_distr(x_true)
        z = self.sample_latent((mu, sigma))
        x_distr = self.generative_distr(z)
        reconstructed_loss = log_likelihood(x_true, x_distr).sum(1).mean(0)
        kl_div = kl((mu, sigma), self.prior_distr(x_true.shape[0])).mean()
        return reconstructed_loss - kl_div
        

    def generate_samples(self, num_samples):
        """
        Генерирует сэмплы из индуцируемого моделью распределения на объекты x.
        Вход: num_samples, int - число сэмплов, которые надо сгененрировать.
        Возвращаемое значение: Tensor, матрица размера num_samples x D.
        """
        mu, sigma = self.prior_distr(num_samples)
        z = self.sample_latent((mu, sigma))
        sample = self.generative_distr(z)
        return torch.bernoulli(sample.view(num_samples, self.D)).cpu()


def gaussian_log_pdf(distr, samples):
    """
    Функция вычисляет логарифм плотности вероятности в точке относительно соответствующего
    нормального распределения, заданного покомпонентно своими средним и среднеквадратичным отклонением.
    Вход: distr, tuple(Tensor, Tensor). Каждый Tensor - матрица размера n x d.
    Первый - mu, второй - sigma.
    Вход: samples, Tensor - тензор размера n x K x d сэмплов в скрытом пространстве.
    Возвращаемое значение: Tensor, матрица размера n x K, каждый элемент которой - логарифм
    плотности вероятности точки относительно соответствующего распределения.
    """
    mu, sigma = distr
    n, K, d = samples.shape
    sqrt_2_pi = math.log(math.sqrt(2 * math.pi))
    log_pdf = - sqrt_2_pi - sigma.log().view(n, 1, d) - (samples - mu.view(n, 1, d)) ** 2 / (2 * sigma ** 2).view(n, 1, d)
    return log_pdf.sum(-1)


def compute_log_likelihood_monte_carlo(x_true, model, K):
    """
    Функция, оценку логарифма правдоподобия вероятностной модели по батчу методом Монте-Карло.
    Оценка логарифма правдоподобия модели должна быть усреднена по всем объектам батча.
    Подсказка: не забудьте привести возращаемый ответ к типу float, иначе при вычислении
    суммы таких оценок будет строится вычислительный граф на них, что быстро приведет к заполнению
    всей доступной памяти.
    Вход: batch, FloatTensor - матрица размера n x D
    Вход: model, Module - объект, имеющий методы prior_distr, sample_latent и generative_distr,
    описанные в VAE.
    Вход: K, int - количество сэмплов.
    Возвращаемое значение: float - оценка логарифма правдоподобия.
    """
    priori = model.prior_distr(x_true.shape[0])
    z = model.sample_latent(priori, K)
    x_distr = model.generative_distr(z)
    return log_mean_exp(log_likelihood(x_true, x_distr)).mean().item()


def compute_log_likelihood_iwae(x_true, model, K):
    """
    Функция, оценку IWAE логарифма правдоподобия вероятностной модели по батчу.
    Оценка логарифма правдоподобия модели должна быть усреднена по всем объектам батча.
    Подсказка: не забудьте привести возращаемый ответ к типу float, иначе при вычислении
    суммы таких оценок будет строится вычислительный граф на них, что быстро приведет к заполнению
    всей доступной памяти.
    Вход: batch, FloatTensor - матрица размера n x D
    Вход: model, Module - объект, имеющий методы prior_distr, proposal_distr, sample_latent и generative_distr,
    описанные в VAE.
    Вход: K, int - количество сэмплов.
    Возвращаемое значение: float - оценка логарифма правдоподобия.
    """
    n, D = x_true.shape
    mu, sigma = model.proposal_distr(x_true)
    priori = model.prior_distr(x_true.shape[0])

    z = model.sample_latent((mu, sigma), K)
    x_distr = model.generative_distr(z)
    
    p_x_z = log_likelihood(x_true, x_distr)
    p_z = gaussian_log_pdf(priori, z)
    q_z_x = gaussian_log_pdf((mu, sigma), z)
    
    p = (p_x_z + p_z - q_z_x)
    return log_mean_exp(p).mean().item()
