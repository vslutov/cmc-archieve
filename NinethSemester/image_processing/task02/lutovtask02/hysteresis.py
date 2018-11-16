import numpy as np

def hysteresis(result,
               mask):
    global used, coords
    rows = result.shape[0]
    cols = result.shape[1]
    used = np.zeros((rows, cols), dtype=np.uint8)
    coords = np.zeros((rows * cols, 2), dtype=np.int32)

    for i in range(rows):
        for j in range(cols):
            if result[i, j] and not used[i, j]:
                used[i, j] = 1
                head = 0
                tail = 1
                coords[head, 0] = i
                coords[head, 1] = j
                while head < tail:
                    h_i = coords[head, 0]
                    h_j = coords[head, 1]
                    head += 1
                    for step_i in range(-1, 2):
                        for step_j in range(-1, 2):
                            n_i = h_i + step_i
                            n_j = h_j + step_j
                            if 0 <= n_i < rows and 0 <= n_j < cols and mask[n_i, n_j] and not used[n_i, n_j]:
                                used[n_i, n_j] = 1
                                result[n_i, n_j] = 1
                                coords[tail, 0] = n_i
                                coords[tail, 1] = n_j
                                tail += 1
    return result
