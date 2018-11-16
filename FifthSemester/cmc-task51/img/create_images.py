# -*- coding: utf-8 -*-

import numpy as np
from matplotlib import pyplot as plt

optim = ('-O0', '-O1', '-O2', '-O3')
y_pos = np.arange(len(optim))
fast_time = np.array([0.52, 0.16, 0.16, 0.16])
fast_error = np.array([0.01, 0.0, 0.01, 0.00])
slow_time = np.array([2.10, 2.12, 2.15, 2.16])
slow_error = np.array([0.57, 0.07, 0.19, 0.26])

bar_width = 0.35
alpha = 0.4
error_kw = {'ecolor': '0.3'}

plt.bar(y_pos, slow_time, bar_width, 
        yerr=slow_error, error_kw=error_kw,
        align='center', color='blue', alpha=alpha,
        label='slow_sum')
plt.bar(y_pos + bar_width, fast_time, bar_width, 
        yerr=fast_error, error_kw=error_kw,
        align='center', color='red', alpha=alpha,
        label='sum')
        
plt.xticks(y_pos + bar_width / 2, optim)
plt.xlabel('Optimization level')
plt.ylabel('Time (sec) [less is better]')
plt.title('Execution time')
plt.legend()

plt.savefig('execution_time.png')