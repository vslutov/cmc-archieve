import numpy as np
from sklearn.metrics import roc_auc_score
import sys

y_true = np.genfromtxt(sys.argv[1], delimiter=',', skip_header=1)[:, -1]
y_pred = np.genfromtxt(sys.argv[2], delimiter=',')[:, 1]
print(roc_auc_score(y_true, y_pred))
