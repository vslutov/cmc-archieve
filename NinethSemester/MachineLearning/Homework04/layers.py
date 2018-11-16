import numpy as np
from scipy.special import logsumexp

class Layer:
    """
    A building block. Each layer is capable of performing two things:

    - Process input to get output:           output = layer.forward(input)

    - Propagate gradients through itself:    grad_input = layer.backward(input, grad_output)

    Some layers also have learnable parameters which they update during layer.backward.
    """

    def __init__(self):
        """
        Here you can initialize layer parameters (if any) and auxiliary stuff.
        """

        pass

    def forward(self, input):
        """
        Takes input data of shape [batch, ...], returns output data [batch, ...]
        """

        return input

    def backward(self, input, grad_output):
        """
        Performs a backpropagation step through the layer, with respect to the given input. Updates layer parameters and returns gradient for next layer
        Let x be layer weights, output – output of the layer on the given input and grad_output – gradient of layer with respect to output

        To compute loss gradients w.r.t parameters, you need to apply chain rule (backprop):
        (d loss / d x)  = (d loss / d output) * (d output / d x)
        Luckily, you already receive (d loss / d output) as grad_output, so you only need to multiply it by (d output / d x)
        If your layer has parameters (e.g. dense layer), you need to update them here using d loss / d x. The resulting update is a sum of updates in a batch.

        returns (d loss / d input) = (d loss / d output) * (d output / d input)
        """

        return grad_output


class ReLU(Layer):
    def __init__(self):
        """
        ReLU layer simply applies elementwise rectified linear unit to all inputs
        This layer does not have any parameters.
        """

        self.pos = None

    def forward(self, input):
        """
        Perform ReLU transformation
        input shape: [batch, input_units]
        output shape: [batch, input_units]
        """

        self.pos = input > 0
        return input * self.pos

    def backward(self, input, grad_output):
        """
        Compute gradient of loss w.r.t. ReLU input
        """

        return self.pos * grad_output


class Dense(Layer):
    def __init__(self, input_units, output_units, learning_rate=0.1):
        """
        A dense layer is a layer which performs a learned affine transformation:
        f(x) = Wx + b

        W: matrix of shape [num_inputs, num_outputs]
        b: vector of shape [num_outputs]
        """

        self.learning_rate = learning_rate

        # initialize weights with small random numbers from normal distribution

        self.weights = np.random.normal(size=(input_units, output_units), scale=np.sqrt(2 / (input_units + output_units)))
        self.biases = np.zeros(shape=(output_units, ))

    def forward(self, input):
        """
        Perform an affine transformation:
        f(x) = <W*x> + b

        input shape: [batch, input_units]
        output shape: [batch, output units]
        """
        return np.dot(input, self.weights) + self.biases[np.newaxis, :]

    def backward(self, input, grad_output):
        """
        input shape: [batch, input_units]
        grad_output: [batch, output units]

        Returns: grad_input, gradient of output w.r.t input
        """
        grad_input = grad_output.dot(self.weights.T)

        self.biases -= self.learning_rate * grad_output.sum(axis=0)
        self.weights -= self.learning_rate * np.dot(input.T, grad_output)

        return grad_input


class Conv2d(Layer):
    def __init__(self, in_channels, out_channels, kernel_size, learning_rate=0.1):
        """
        A convolutional layer with out_channels kernels of kernel_size.

        in_channels — number of input channels
        out_channels — number of convolutional filters
        kernel_size — tuple of two numbers: k_1 and k_2

        Initialize required weights.
        """

        self.in_channels = in_channels
        self.out_channels = out_channels
        self.learning_rate = learning_rate
        self.kw, self.kh = kernel_size

        eta = 1 + self.kw * self.kh * in_channels
        self.weights = np.random.normal(scale=np.sqrt(2 / eta), size=(in_channels, out_channels, self.kw, self.kh))
        
    def forward(self, input):
        """
        Perform convolutional transformation:

        input shape: [batch, in_channels, h, w]
        output shape: [batch, out_channels, h_out, w_out]
        """
        b, _, h, w = input.shape
        h_out = h - self.kh + 1
        w_out = w - self.kw + 1

        td = np.tensordot(input, self.weights, [[1], [0]]).transpose((0, 3, 4, 5, 1, 2))
        output = np.zeros((b, self.out_channels, h_out, w_out))

        for i in range(0, self.kh):
            for j in range(0, self.kw):
                output += td[:, :, i, j, i:i + h_out, j:j + w_out]

        return output

    def backward(self, input, grad_output):
        """
        Compute gradients w.r.t input and weights and update weights
        """
        b, _, h, w = input.shape
        h_out = h - self.kh + 1
        w_out = w - self.kw + 1

        d_input = np.zeros(input.shape)
        td = np.tensordot(grad_output, self.weights, [[1], [1]]).transpose((0, 3, 4, 5, 1, 2))
        for i in range(0, self.kh):
            for j in range(0, self.kw):
                d_input[:, :, i:i + h_out, j:j + w_out] += td[:, :, i, j]

        d_weights = np.zeros(self.weights.shape)
        for i in range(0, self.kh):
            for j in range(0, self.kw):
                d_weights[:, :, i, j] = np.tensordot(input[:, :, i:i + h_out, j:j + w_out], grad_output, [[0, 2, 3], [0, 2, 3]])

        self.weights -= self.learning_rate * d_weights

        return d_input

class Maxpool2d(Layer):
    def __init__(self, kernel_size):
        """
        A maxpooling layer with kernel of kernel_size.
        This layer donwsamples [kernel_size, kernel_size] to
        1 number which represents maximum.

        Stride description is identical to the convolution
        layer. But default value we use is kernel_size to
        reduce dim by kernel_size times.

        This layer does not have any learnable parameters.
        """

        self.stride = kernel_size
        self.kernel_size = kernel_size

    def forward(self, input):
        """
        Perform maxpooling transformation:

        input shape: [batch, in_channels, h, w]
        output shape: [batch, out_channels, h_out, w_out]
        """
        kh = kw = self.kernel_size

        self.reshaped = np.stack([input[:, :, i::kh, j::kw] for i in range(kh) for j in range(kw)], axis=-1)
        return self.reshaped.max(axis=-1)

    def backward(self, input, grad_output):
        """
        Compute gradient of loss w.r.t. Maxpool2d input
        """
        argmax = np.argmax(self.reshaped, axis=-1)
        kh = kw = self.kernel_size

        d_input = np.zeros(input.shape)
        for i in range(kh):
            for j in range(kw):
                d_input[:, :, i::kh, j::kw] = (argmax == i * kw + j) * grad_output

        return d_input


class Flatten(Layer):
    def __init__(self):
        """
        This layer does not have any parameters
        """
        pass

    def forward(self, input):
        """
        input shape: [batch_size, channels, feature_nums_h, feature_nums_w]
        output shape: [batch_size, channels * feature_nums_h * feature_nums_w]
        """
        return input.reshape((input.shape[0], -1))

    def backward(self, input, grad_output):
        """
        Compute gradient of loss w.r.t. Flatten input
        """
        return grad_output.reshape(input.shape)


def softmax(logits):
    logits = logits - logits.max()
    s_max = np.exp(logits)
    s_max /= s_max.sum(axis=1, keepdims=True)
    return s_max

def one_hot_encoding(y_true, classes):
    ohe = np.eye(classes)[y_true]
    return ohe

def softmax_crossentropy_with_logits(logits, y_true):
    """
    Compute crossentropy from logits and ids of correct answers
    logits shape: [batch_size, num_classes]
    reference_answers: [batch_size]
    output is a number
    """
    return (-logits[np.arange(logits.shape[0]), y_true] + logsumexp(logits, axis=1)).mean()


def grad_softmax_crossentropy_with_logits(logits, y_true):
    """
    Compute crossentropy gradient from logits and ids of correct answers
    Output should be divided by batch_size, so any layer update can be simply computed as sum of object updates.
    logits shape: [batch_size, num_classes]
    reference_answers: [batch_size]
    """
    return (softmax(logits) - one_hot_encoding(y_true, logits.shape[1])) / y_true.shape[0]
