import theano, idx2numpy
import sys, os, gzip
from theano import tensor as T
from matplotlib import pyplot as plt
import numpy as np

N_DIGITS = 10
N_HIDDEN = 500

class NeuralNetwork:
    """
    Neural network.
    
    y = x * A + b
    out = y * C + d
    """ 
    
    L1_COST = 0.00001
    L2_COST = 0.0001
    LR = 0.21
    VALIDATE_SIZE = 5000
    VAL_FREQ = 1000
    MINIBATCH_SIZE = 20
    
    MAX_ITERATION = 50000
    PATIENCE = 20000  # minimum of  iteration
    PATIENCE_INCREASE = 2  # wait this much longer when a new best is
                           # found
    IMPROVEMENT_THRESHOLD = 0.005  # an absolute improvement of this much is
                                   # considered significant
    
    def __init__(self, n_in, n_hidden, n_out):
        self.params = []
        self.x = T.matrix('x')
        self.res = T.ivector('res')
        self.lr = T.scalar('lr')
        self.hidden = self.add_full_connected_layer(self.x, n_in, n_hidden, T.tanh)
        self.out = self.add_full_connected_layer(self.hidden, n_hidden, n_out, T.nnet.softmax)
        self.y = T.argmax(self.out, axis=1)
       

        param_cost = 0
        for param in self.params:
            param_cost += self.L1_COST * T.sum(abs(param)) + self.L2_COST * T.sum(param ** 2)
        self.cost = param_cost - T.mean(T.log(self.out)[T.arange(self.res.shape[0]), self.res])
        
        grads = T.grad(self.cost, self.params)
        self.errors = T.mean(T.neq(self.y, self.res))
        
        self.updates = [(self.params[i], self.params[i] - self.lr * grads[i])
                        for i in range(len(self.params))]
        self.train = theano.function([self.x, self.res, self.lr], self.errors, 
                                      updates=self.updates)
        self.loss = theano.function([self.x, self.res], self.errors)
        
        self.predict = theano.function([self.x], self.y)
    
    def add_full_connected_layer(self, inp, n_in, n_out, activation=T.tanh, scale_mul=1):
        scale = scale_mul * np.sqrt(6. / (n_in + n_out))
        A = theano.shared(np.random.uniform(-scale, +scale, size=(n_in, n_out)), borrow=True)
        b = theano.shared(np.zeros(n_out), borrow=True)
        self.params.append(A)
        self.params.append(b)
        out = T.dot(inp, A) + b
        
        if activation is not None:
            return activation(out)
        else:
            return out
    
    def save_params(self):
        return [param.get_value(borrow=False) for param in self.params]
    
    def load_params(self, params):
        for i in range(len(params)):
            self.params[i].set_value(params[i], borrow=False)
    
    def sgd(self, train_images, train_labels):
        train_set_size = train_images.shape[0] - self.VALIDATE_SIZE
        
        def shared(array):
            array = np.split(array, range(self.MINIBATCH_SIZE,
                                          train_set_size,
                                          self.MINIBATCH_SIZE))
            array = array[:-1]
            size = len(array)            
            array = np.asarray(array)
            array = theano.shared(array, borrow=True)
            return array, size
        
        validate_images = theano.shared(train_images[train_set_size:], borrow=True)
        validate_labels = theano.shared(train_labels[train_set_size:], borrow=True)
        train_images, batch_count = shared(train_images)
        train_labels, batch_count = shared(train_labels)

        index = T.iscalar('index')
        lr = theano.shared(np.asarray(0, dtype=theano.config.floatX))
        
        train = theano.function([index], self.errors, updates=self.updates,
                                givens={self.x: train_images[index],
                                        self.res: train_labels[index],
                                        self.lr: lr})
        loss = theano.function([], self.errors,
                               givens={self.x: validate_images,
                                       self.res: validate_labels})
    
        patience = self.PATIENCE        
        best_validation_loss = np.inf
        epoch = 0
        done_looping = False
        
        while not done_looping:
            epoch = epoch + 1
            lr.set_value(self.LR * 7 / (epoch + 6))
            
            for index in range(0, batch_count):
                train(index)
                iteration = (epoch - 1) * batch_count + index + 1

                if  (index == batch_count - 1 or
                     iteration % self.VAL_FREQ == 0):

                    this_validation_loss = loss()
            
                    if this_validation_loss < best_validation_loss:
                        if this_validation_loss - best_validation_loss > self.IMPROVEMENT_THRESHOLD:
                            patience = max(patience, iteration * self.PATIENCE_INCREASE)
            
                        best_validation_loss = this_validation_loss
                        best_params = self.save_params()
                        print('epoch {epoch}, iteration {iteration}/{patience}, best loss: {best_validation_loss} %'
                              .format(epoch=epoch, iteration=iteration, patience=patience, best_validation_loss=100 * best_validation_loss))
    
                if iteration >= min(patience, self.MAX_ITERATION):
                    done_looping = True
                    break
            
            print('epoch {epoch}, {iteration}/{patience} validation error {error} %'
                  .format(epoch=epoch, iteration=iteration, patience=patience, error=100 * best_validation_loss))
                
        self.load_params(best_params)
        return loss()

USAGE = """usage:
    {name} MNIST_DIR
    
    MNIST_DIR should contain files: 
        * train_images.idx
        * train_labels.idx
        * test_images.idx
        * test_labels.idx
"""

def labels_to_array(labels):
    labels = np.array(labels)
    y = np.zeros((labels.shape[0], N_DIGITS))
    y[np.arange(labels.shape[0]), labels] = 1
    return y

def load_data(path):
    filenames = [("train-images-idx3-ubyte.gz", "train_images.npy"),
                 ("train-labels-idx1-ubyte.gz", "train_labels.npy"),
                 ("t10k-images-idx3-ubyte.gz", "test_images.npy"),
                 ("t10k-labels-idx1-ubyte.gz", "test_labels.npy")]  
    
    result = []
    for pathpair in filenames:
        if os.path.isfile(os.path.join(path, pathpair[1])):
            array = np.load(os.path.join(path, pathpair[1]))
        else:
            with gzip.open(os.path.join(path, pathpair[0])) as input_file:
                array = idx2numpy.convert_from_file(input_file)             
                if len(array.shape) > 1:
                    array = array.astype(theano.config.floatX)
                    array = array.reshape((array.shape[0], -1))
                    array /= 255
                else:
                    array = array.astype(np.int32)
                np.save(os.path.join(path, pathpair[1]), array)
        
        result.append(array)
            
    return result

def run_sgd(train_images, train_labels, lr, l1, l2):
    size = train_images.size // train_images.shape[0]
    nn = NeuralNetwork(size, N_HIDDEN, N_DIGITS)
    nn.LR = lr
    nn.L1_COST = l1
    nn.L2_COST = l2
    return nn.sgd(train_images, train_labels)

def search_params(train_images, train_labels):
    grid = dict()    
    for lr in np.arange(0.03, 0.3, 0.03):
        for l1 in [0.05, 0.01, 0.005, 0.001, 0.0005, 0.0001, 0.00005, 0.00001]:
            for l2 in [10 * l1, l1, 0.1 * l1]:
                grid[(lr, l1, l2)] = run_sgd(train_images, train_labels, lr, l1, l2)
                print(lr, l1, l2, grid[(lr, l1, l2)])
    print(grid)
    min_index = min(grid, key=lambda x: grid[x])
    print('min: grid[', min_index, '] = ', grid[min_index], sep='')

def main(argv):
    if len(argv) != 2 or not os.path.isdir(argv[1]):
        print(USAGE.format(name=argv[0]))
        sys.exit(1)
    else:
        path = argv[1]
        path = os.path.abspath(path)
    
    train_images, train_labels, test_images, test_labels = load_data(path)
    # search_params(train_images, train_labels)    
    
    
    size = train_images.size // train_images.shape[0]
    nn = NeuralNetwork(size, N_HIDDEN, N_DIGITS)
    nn.sgd(train_images, train_labels)
    
    print('Test error', nn.loss(test_images, test_labels) * 100, '%')

if __name__ == "__main__":
    main(sys.argv)