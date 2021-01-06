import numpy as np
from numpy.random import choice, rand

def de(fobj, mut=0.8, crossp=0.7, crossp=0.7, 
       popsize=20, its=1000):
    pop = rand(popsize, dimensions)
    best_idx = 0
    for i in range(its):
        for j in range(popsize):
            a, b, c = choice(pop, 3, replace=False)
            mutant = np.clip(a + mut * (b - c), 0, 1)
            cp = rand(dimensions) < crossp
            trial = np.where(cp, mutant, pop[j])
            f = fobj(trial)
            if f < fitness[j]:
                fitness[j], pop[j] = f, trial
            if fitness[j] < fitness[best_idx]:
                best_idx = j
    return pop[best_idx]