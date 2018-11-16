from sklearn.preprocessing import OneHotEncoder
from time import time
from sklearn.base import BaseEstimator
import numpy as np
from scipy.special import expit


class LogReg(BaseEstimator):
    def __init__(self, lambda_1=0.0, lambda_2=1.0, gd_type='stochastic',
                 tolerance=1e-6, max_iter=1000, w0=None, alpha=1e-3,
                 random_state=None
                ):
        """
        lambda_1: L1 regularization param
        lambda_2: L2 regularization param
        gd_type: 'full' or 'stochastic'
        tolerance: for stopping gradient descent
        max_iter: maximum number of steps in gradient descent
        w0: np.array of shape (d) - init weights
        alpha: learning rate
        """
        self.lambda_1 = lambda_1
        self.lambda_2 = lambda_2
        self.gd_type = gd_type
        self.tolerance = tolerance
        self.max_iter = max_iter
        self.w0 = w0
        self.alpha = alpha
        self.w = None
        self.loss_history = None
        self.random_state = random_state

    def fit(self, X, y, X_val=None, y_val=None, calc_loss=False):
        """
        X: np.array of shape (l, d)
        y: np.array of shape (l)
        ---
        output: self
        """
        self.loss_history = []
        l = X.shape[0]

        y = y.astype(np.float32)
        y -= y.min()
        y /= y.max() / 2.0
        y -= 1

        np.random.seed(self.random_state)
        self.w = self.w0 if self.w0 is not None else np.random.rand(X.shape[1]) * 2 - 1

        start = time()

        for i in range(self.max_iter):
            old_w = self.w.copy()

            if self.gd_type == "stochastic":
                i_k = np.random.randint(0, l, size=(1, ))
                g = self.calc_gradient(X[i_k, :], y[i_k])
            else:
                g = self.calc_gradient(X, y)

            self.w -= g * self.alpha

            self.loss_history.append([time() - start])
            
            if calc_loss:
                self.loss_history[-1].append(self.calc_loss(X, y))
                if X_val is not None and y_val is not None:
                    self.loss_history[-1].append(self.calc_loss(X_val, y_val))

            if ((self.w - old_w) ** 2).sum() <= self.tolerance:
                break

        return self

    def predict_proba(self, X):
        """
        X: np.array of shape (l, d)
        ---
        output: np.array of shape (l, 2) where
        first column has probabilities of -1
        second column has probabilities of +1
        """
        if self.w is None:
            raise Exception('Not trained yet')

        result = 1 / (1 + np.exp(-X.dot(self.w)))
        return np.hstack([1 - result.reshape([-1, 1]), result.reshape([-1, 1])])

    def calc_gradient(self, X, y):
        """
        X: np.array of shape (l, d) (l can be equal to 1 if stochastic)
        y: np.array of shape (l)
        ---
        output: np.array of shape (d)
        """
        g = -y[:, np.newaxis] * X * expit(-y * X.dot(self.w))[:, np.newaxis]
        return g.mean(axis=0) + self.lambda_2 * self.w

    def calc_loss(self, X, y):
        """
        X: np.array of shape (l, d)
        y: np.array of shape (l)
        ---
        output: float
        """
        return (np.logaddexp(0, -y * X.dot(self.w)).mean() +
                self.lambda_2 / 2.0 * np.sum(self.w ** 2))
