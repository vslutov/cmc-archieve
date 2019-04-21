#include <iostream>
#include <cmath>
#include <stdexcept>
#include <cstring>
#include <memory>
#include <ctime>

#include <omp.h>
#include <cuda.h>
#include <cuda_runtime.h>

#include <SOIL/SOIL.h>

const float EPS = 0.15 * 0.15;
const ssize_t BOX_SIZE = 5;
const ssize_t PAD_SIZE = BOX_SIZE / 2;
const ssize_t BLOCK_SIZE_X = 16;
const ssize_t BLOCK_SIZE_Y = 16;
const dim3 BLOCK_SIZE = dim3(BLOCK_SIZE_X, BLOCK_SIZE_Y);
const ssize_t SHARED_BUFFER_SIZE_X = BLOCK_SIZE_X + BOX_SIZE - 1;
const ssize_t SHARED_BUFFER_SIZE_Y = BLOCK_SIZE_Y + BOX_SIZE - 1;

// Access to array
#define ACCESS(arr, x, y, c, w, h, channels) (arr[((y) * w + (x)) * (channels) + (c)])

// Check that point is outside of array
__host__ __device__ int
validAddress(int x, int y, int w, int h)
{
  return x >= 0 && x < w && y >= 0 && y < h;
}

inline void
calcTmp (
  uint8_t *inImg,
  float *tmpImg,
  ssize_t x,
  ssize_t y,
  ssize_t w,
  ssize_t h,
  float eps
)
{
  for (ssize_t c = 0; c < 3; ++c) {
    float mean = 0;
    float corr = 0;
    ssize_t count = 0;

    for (ssize_t i = x - PAD_SIZE; i < x + BOX_SIZE - PAD_SIZE; ++ i) {
      for (ssize_t j = y - PAD_SIZE; j < y + BOX_SIZE - PAD_SIZE; ++ j) {
        if (validAddress(i, j, w, h)) {
          float color = ACCESS(inImg, i, j, c, w, h, 3) / 255.0;
          mean += color;
          corr += color * color;
          ++ count;
        }
      }
    }

    mean /= count;
    corr /= count;
    float var = corr - mean * mean;

    float a = var / (var + eps);
    float b = (1 - a) * mean;

    ACCESS(tmpImg, x, y, 2 * c, w, h, 6) = a;
    ACCESS(tmpImg, x, y, 2 * c + 1, w, h, 6) = b;
  }
}

__device__ void
prepareSharedBufferFromIn(
  uint8_t *inImg,
  float sharedBuffer[SHARED_BUFFER_SIZE_X][SHARED_BUFFER_SIZE_Y][3],
  ssize_t x,
  ssize_t y,
  ssize_t w,
  ssize_t h
)
{
  ssize_t diffX = -PAD_SIZE;
  ssize_t diffY = -PAD_SIZE;
  if (validAddress(x + diffX, y + diffY, w, h)) {
    for (ssize_t c = 0; c < 3; ++ c) {
      sharedBuffer[threadIdx.x + diffX + PAD_SIZE][threadIdx.y + diffY + PAD_SIZE][c] = ACCESS(inImg, x + diffX, y + diffY, c, w, h, 3) / 255.0;
    }
  }
  diffY = BOX_SIZE - PAD_SIZE - 1;
  if (validAddress(x + diffX, y + diffY, w, h)) {
    for (ssize_t c = 0; c < 3; ++ c) {
      sharedBuffer[threadIdx.x + diffX + PAD_SIZE][threadIdx.y + diffY + PAD_SIZE][c] = ACCESS(inImg, x + diffX, y + diffY, c, w, h, 3) / 255.0;
    }
  }
  diffX = BOX_SIZE - PAD_SIZE - 1;
  if (validAddress(x + diffX, y + diffY, w, h)) {
    for (ssize_t c = 0; c < 3; ++ c) {
      sharedBuffer[threadIdx.x + diffX + PAD_SIZE][threadIdx.y + diffY + PAD_SIZE][c] = ACCESS(inImg, x + diffX, y + diffY, c, w, h, 3) / 255.0;
    }
  }
  diffY = -PAD_SIZE;
  if (validAddress(x + diffX, y + diffY, w, h)) {
    for (ssize_t c = 0; c < 3; ++ c) {
      sharedBuffer[threadIdx.x + diffX + PAD_SIZE][threadIdx.y + diffY + PAD_SIZE][c] = ACCESS(inImg, x + diffX, y + diffY, c, w, h, 3) / 255.0;
    }
  }
  __syncthreads();
}

__device__  void
calcTmpShared(
  uint8_t *inImg,
  float *tmpImg,
  ssize_t x,
  ssize_t y,
  ssize_t w,
  ssize_t h,
  float eps
)
{
  __shared__ float sharedBuffer[SHARED_BUFFER_SIZE_X][SHARED_BUFFER_SIZE_Y][3];
  prepareSharedBufferFromIn(inImg, sharedBuffer, x, y, w, h);

  for (ssize_t c = 0; c < 3; ++c) {
    float mean = 0;
    float corr = 0;
    ssize_t count = 0;

    for (ssize_t i = x - PAD_SIZE; i < x + BOX_SIZE - PAD_SIZE; ++ i) {
      for (ssize_t j = y - PAD_SIZE; j < y + BOX_SIZE - PAD_SIZE; ++ j) {
        if (validAddress(i, j, w, h)) {
          float color = sharedBuffer[i - blockIdx.x * blockDim.x + PAD_SIZE][j - blockIdx.y * blockDim.y + PAD_SIZE][c];
          mean += color;
          corr += color * color;
          ++ count;
        }
      }
    }

    mean /= count;
    corr /= count;
    float var = corr - mean * mean;

    float a = var / (var + eps);
    float b = (1 - a) * mean;

    ACCESS(tmpImg, x, y, 2 * c, w, h, 6) = a;
    ACCESS(tmpImg, x, y, 2 * c + 1, w, h, 6) = b;
  }
}

__device__ void
prepareSharedBufferFromTmp(
  float *tmpImg,
  float sharedBuffer[SHARED_BUFFER_SIZE_X][SHARED_BUFFER_SIZE_Y][6],
  ssize_t x,
  ssize_t y,
  ssize_t w,
  ssize_t h
)
{
  ssize_t diffX = -PAD_SIZE;
  ssize_t diffY = -PAD_SIZE;
  if (validAddress(x + diffX, y + diffY, w, h)) {
    for (ssize_t c = 0; c < 6; ++ c) {
      sharedBuffer[threadIdx.x + diffX + PAD_SIZE][threadIdx.y + diffY + PAD_SIZE][c] = ACCESS(tmpImg, x + diffX, y + diffY, c, w, h, 6);
    }
  }
  diffY = BOX_SIZE - PAD_SIZE - 1;
  if (validAddress(x + diffX, y + diffY, w, h)) {
    for (ssize_t c = 0; c < 6; ++ c) {
      sharedBuffer[threadIdx.x + diffX + PAD_SIZE][threadIdx.y + diffY + PAD_SIZE][c] = ACCESS(tmpImg, x + diffX, y + diffY, c, w, h, 6);
    }
  }
  diffX = BOX_SIZE - PAD_SIZE - 1;
  if (validAddress(x + diffX, y + diffY, w, h)) {
    for (ssize_t c = 0; c < 6; ++ c) {
      sharedBuffer[threadIdx.x + diffX + PAD_SIZE][threadIdx.y + diffY + PAD_SIZE][c] = ACCESS(tmpImg, x + diffX, y + diffY, c, w, h, 6);
    }
  }
  diffY = -PAD_SIZE;
  if (validAddress(x + diffX, y + diffY, w, h)) {
    for (ssize_t c = 0; c < 6; ++ c) {
      sharedBuffer[threadIdx.x + diffX + PAD_SIZE][threadIdx.y + diffY + PAD_SIZE][c] = ACCESS(tmpImg, x + diffX, y + diffY, c, w, h, 6);
    }
  }
  __syncthreads();
}

inline  void
calcOut(
  uint8_t *inImg,
  float *tmpImg,
  uint8_t *outImg,
  ssize_t x,
  ssize_t y,
  ssize_t w,
  ssize_t h
)
{
  for (ssize_t c = 0; c < 3; ++c) {
    float meanA = 0;
    float meanB = 0;
    ssize_t count = 0;

    for (ssize_t i = x - PAD_SIZE; i < x + BOX_SIZE - PAD_SIZE; ++ i) {
      for (ssize_t j = y - PAD_SIZE; j < y + BOX_SIZE - PAD_SIZE; ++ j) {
        if (validAddress(i, j, w, h)) {
          meanA += ACCESS(tmpImg, i, j, 2 * c, w, h, 6);
          meanB += ACCESS(tmpImg, i, j, 2 * c + 1, w, h, 6);
          ++ count;
        }
      }
    }

    meanA /= count;
    meanB /= count;

    ACCESS(outImg, x, y, c, w, h, 3) = ACCESS(inImg, x, y, c, w, h, 3) * meanA + 255 * meanB;
  }
}

__device__  void
calcOutShared(
  uint8_t *inImg,
  float *tmpImg,
  uint8_t *outImg,
  ssize_t x,
  ssize_t y,
  ssize_t w,
  ssize_t h
)
{
  __shared__ float sharedBuffer[SHARED_BUFFER_SIZE_X][SHARED_BUFFER_SIZE_Y][6];
  prepareSharedBufferFromTmp(tmpImg, sharedBuffer, x, y, w, h);

  for (ssize_t c = 0; c < 3; ++c) {
    float meanA = 0;
    float meanB = 0;
    ssize_t count = 0;

    for (ssize_t i = x - PAD_SIZE; i < x + BOX_SIZE - PAD_SIZE; ++ i) {
      for (ssize_t j = y - PAD_SIZE; j < y + BOX_SIZE - PAD_SIZE; ++ j) {
        if (validAddress(i, j, w, h)) {
          meanA += sharedBuffer[i - blockIdx.x * blockDim.x + PAD_SIZE][j - blockIdx.y * blockDim.y + PAD_SIZE][2 * c];
          meanB += sharedBuffer[i - blockIdx.x * blockDim.x + PAD_SIZE][j - blockIdx.y * blockDim.y + PAD_SIZE][2 * c + 1];
          ++ count;
        }
      }
    }

    meanA /= count;
    meanB /= count;

    ACCESS(outImg, x, y, c, w, h, 3) = ACCESS(inImg, x, y, c, w, h, 3) * meanA + 255 * meanB;
  }
}

void
guidedCpu(
  uint8_t *inImg,
  float *tmpImg,
  uint8_t *outImg,
  ssize_t w,
  ssize_t h,
  float eps
)
{
  for(ssize_t x = 0; x < w; ++x) {
    for(ssize_t y = 0; y < h; ++y) {
      calcTmp(inImg, tmpImg, x, y, w, h, eps);
    }
  }
  for(ssize_t x = 0; x < w; ++x) {
    for(ssize_t y = 0; y < h; ++y) {
      calcOut(inImg, tmpImg, outImg, x, y, w, h);
    }
  }
}

void
guidedOpenMP(
  uint8_t *inImg,
  float *tmpImg,
  uint8_t *outImg,
  ssize_t w,
  ssize_t h,
  float eps
)
{
  #pragma omp parallel for
  for(ssize_t x = 0; x < w; x++) {
    for(ssize_t y = 0; y < h; y++) {
      calcTmp(inImg, tmpImg, x, y, w, h, eps);
    }
  }
  #pragma omp parallel for
  for(ssize_t x = 0; x < w; x++) {
    for(ssize_t y = 0; y < h; y++) {
      calcOut(inImg, tmpImg, outImg, x, y, w, h);
    }
  }
}

__global__ void
guidedGpuTmp(
  uint8_t *inImg,
  float *tmpImg,
  ssize_t w,
  ssize_t h,
  float eps
)
{
  // compute idx
  int x = threadIdx.x + blockIdx.x * blockDim.x;
  int y = threadIdx.y + blockIdx.y * blockDim.y;

  if (!validAddress(x, y, w, h)) {
    return;
  }

  calcTmpShared(inImg, tmpImg, x, y, w, h, eps);
}

__global__ void
guidedGpuOut(
  uint8_t *inImg,
  float *tmpImg,
  uint8_t *outImg,
  ssize_t w,
  ssize_t h,
  float eps
)
{
  // compute idx
  int x = threadIdx.x + blockIdx.x * blockDim.x;
  int y = threadIdx.y + blockIdx.y * blockDim.y;

  if (!validAddress(x, y, w, h)) {
    return;
  }

  calcOutShared(inImg, tmpImg, outImg, x, y, w, h);
}

void
checkErrors()
{
  cudaError_t code = cudaGetLastError();
  if (code != cudaSuccess){
    throw std::runtime_error(std::string("Cuda error : ") + cudaGetErrorString(code));
  }
}

void
guidedGpu(
  uint8_t *inImg,
  float *tmpImg,
  uint8_t *outImg,
  ssize_t w,
  ssize_t h,
  float eps
)
{
  dim3 gridSize(std::ceil((float) w / BLOCK_SIZE_X),
                std::ceil((float) h / BLOCK_SIZE_Y));

  guidedGpuTmp<<<gridSize, BLOCK_SIZE>>>(inImg, tmpImg, w, h, eps); checkErrors();
  guidedGpuOut<<<gridSize, BLOCK_SIZE>>>(inImg, tmpImg, outImg, w, h, eps); checkErrors();
}

float
measureTime(const std::string &task, const std::function<void(void)> &func) {
  volatile std::clock_t start, time;

  start = std::clock();
  func();
  time = std::clock() - start;

  float result = time * 1000.0 / CLOCKS_PER_SEC;

  std::cout << task <<" time: " << result << " ms" << std::endl;

  return result;
}

int
main(int argc, char **argv)
{
  // input image
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " image" << std::endl;
    return 1;
  }

  int width, height;

  uint8_t* inImg = SOIL_load_image(
    argv[1],
    &width,
    &height,
    NULL,
    SOIL_LOAD_RGB
  );

  int image_size = height * width * 3;

  // output image
  auto outImgContainer = std::unique_ptr<uint8_t[]>(new uint8_t[image_size]);
  auto outImg = outImgContainer.get();
  auto tmpImgContainer = std::unique_ptr<float[]>(new float[2 * image_size]);
  auto tmpImg = tmpImgContainer.get();

  // guided arguments
  // cpu
  measureTime("CPU", [inImg, tmpImg, outImg, width, height]() {
    guidedCpu(inImg, tmpImg, outImg, width, height, EPS);
  });

  SOIL_save_image("cpu.tga", SOIL_SAVE_TYPE_TGA, width, height, 3, outImg);

  // OpenMP
  measureTime("OpenMP", [inImg, tmpImg, outImg, width, height]() {
    guidedOpenMP(inImg, tmpImg, outImg, width, height, EPS);
  });

  SOIL_save_image("openmp.tga", SOIL_SAVE_TYPE_TGA, width, height, 3, outImg);

  // allocate memory
  uint8_t *inImgDev, *outImgDev;
  float *tmpImgDev;
  cudaMalloc((void **) &inImgDev, image_size); checkErrors();
  cudaMalloc((void **) &outImgDev, image_size); checkErrors();
  cudaMalloc((void **) &tmpImgDev, sizeof(float) * 2 * image_size); checkErrors();
  cudaMemcpy(inImgDev, inImg, image_size, cudaMemcpyHostToDevice); checkErrors();

  // cuda
  const auto timeWithCopy = measureTime("CUDA with copy", [inImgDev, tmpImgDev, outImgDev, width, height, inImg, outImg, image_size]() {
    cudaMemcpy(inImgDev, inImg, image_size, cudaMemcpyHostToDevice); checkErrors();
    guidedGpu(inImgDev, tmpImgDev, outImgDev, width, height, EPS);
    cudaMemcpy(outImg, outImgDev, image_size, cudaMemcpyDeviceToHost); checkErrors();
  });

  const auto timeWithoutCopy = measureTime("CUDA without copy", [inImgDev, tmpImgDev, outImgDev, width, height]() {
    guidedGpu(inImgDev, tmpImgDev, outImgDev, width, height, EPS);
    cudaDeviceSynchronize(); checkErrors();
  });

  std::cout << "Copy time: " << (timeWithCopy - timeWithoutCopy) << " ms" << std::endl;

  SOIL_save_image(
    "gpu.tga",
    SOIL_SAVE_TYPE_TGA,
    width,
    height,
    3,
    outImg
  );

  cudaFree(inImgDev); checkErrors();
  cudaFree(outImgDev); checkErrors();

  return 0;
}
