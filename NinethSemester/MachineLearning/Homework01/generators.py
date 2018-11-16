from random import random, randrange

def prod_non_zero_diag(max_size):
    for n in range(1, max_size):
        yield ([[random() * 2 for j in range(n)] for i in range(n)], )

def are_multisets_equal(max_size):
    for n in range(1, max_size):
        yield tuple(zip(*((randrange(n), randrange(n)) for i in range(n))))

def max_after_zero(max_size):
    for n in range(1, max_size):
        yield ([0] + list(randrange(n) for i in range(n)), )

def convert_image(max_size):
    for n in range(1, max_size):
        yield ([[[randrange(256) for c in range(3)] for j in range(n)] for i in range(n)], [0.299, 0.587, 0.114])

def run_length_encoding(max_size):
    for n in range(1, max_size):
        yield (list(randrange(3) for i in range(n)), )

def pairwise_distance(max_size):
    for n in range(1, max_size):
        yield ([[randrange(n) for c in range(20)] for i in range(n)], [[randrange(n) for c in range(20)] for i in range(n)])