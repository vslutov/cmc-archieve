#include <fstream>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include <ctime>
#include <functional>

#include <CL/cl.h>
#include <IL/devil_cpp_wrapper.hpp>

struct OpenCLContext {
  cl_platform_id cpPlatform;
  cl_device_id cdDevice;
  cl_context GPUContext;
  cl_command_queue cqCommandQueue;
  cl_mem GPUInputVector;
  cl_mem GPUOutputVector;
  cl_program OpenCLProgram;
  cl_kernel ApplyFilter;
  size_t global_work_size[2];
  size_t local_work_size[2];
};

void
calcCpu(
  const float *input,
  float *output,
  ssize_t width,
  ssize_t height
)
{
  for(ssize_t i = 0; i < height; ++ i) {
    for(ssize_t j = 0; j < width; ++ j) {
      auto coord = (i * width + j) * 3;

      output[coord] = 1 - input[coord];
      output[coord + 1] = 1 - input[coord + 1];
      output[coord + 2] = 1 - input[coord + 2];
    }
  }
}

OpenCLContext
initGpu(size_t width, size_t height)
{
  auto bufferSize = width * height * 3;

  OpenCLContext context;

  //Get an OpenCL platform
  clGetPlatformIDs(1, &context.cpPlatform, NULL);

  // Get a GPU device
  clGetDeviceIDs(context.cpPlatform, CL_DEVICE_TYPE_GPU, 1, &context.cdDevice, NULL);

  // Create a context to run OpenCL enabled GPU
  context.GPUContext = clCreateContextFromType(0, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);

  // Create a command-queue on the GPU device
  context.cqCommandQueue = clCreateCommandQueue(context.GPUContext, context.cdDevice, 0, NULL);

  // Allocate GPU memory for source vectors AND initialize from CPU memory
  context.GPUInputVector = clCreateBuffer(
    context.GPUContext,
    CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
    sizeof(float) * bufferSize,
    NULL,
    NULL
  );

  // Allocate output memory on GPU
  context.GPUOutputVector = clCreateBuffer(
    context.GPUContext,
    CL_MEM_WRITE_ONLY,
    sizeof(float) * bufferSize,
    NULL,
    NULL
  );

  std::string kernelSource;

  // Load ApplyFilter source
  std::ifstream in("ApplyFilter.cl");
  if (in) {
    in.seekg(0, std::ios::end);
    size_t len = in.tellg();
    in.seekg(0);
    kernelSource.resize(len + 1, '\0');
    in.read(&kernelSource[0], len);
  }

  auto kernelCSource = kernelSource.c_str();

  // Create OpenCL program with source code
  context.OpenCLProgram = clCreateProgramWithSource(
    context.GPUContext,
    1,
    &kernelCSource,
    NULL,
    NULL
  );

  // Build the program (OpenCL JIT compilation)
  clBuildProgram(context.OpenCLProgram, 0, NULL, NULL, NULL, NULL);

  // Create a handle to the compiled OpenCL function (Kernel)
  context.ApplyFilter = clCreateKernel(context.OpenCLProgram, "ApplyFilter", NULL);

  // In the next step we associate the GPU memory with the Kernel arguments
  clSetKernelArg(context.ApplyFilter, 0, sizeof(cl_mem), (void*)&context.GPUOutputVector);
  clSetKernelArg(context.ApplyFilter, 1, sizeof(cl_mem), (void*)&context.GPUInputVector);

  // Launch the Kernel on the GPU

  // This ApplyFilter only uses global data
  context.global_work_size[0] = width / 4;
  context.global_work_size[1] = height / 4;
  context.local_work_size[0] = 4;
  context.local_work_size[1] = 4;

  return context;
}

void
calcTime(const std::string &task, const std::function<void(void)> &func) {
  volatile std::clock_t start, time;

  start = std::clock();
  func();
  time = std::clock() - start;

  std::cout << task <<" time: " << time * 1000.0 / CLOCKS_PER_SEC << " ms" << std::endl;
}

int
main(int argc, char **argv)
{
  auto inputImage = ilImage("bliss.png");

  auto width = inputImage.Width();
  auto height = inputImage.Height();
  auto bufferSize = width * height * 3;

  // read image to buffer
  auto input = std::make_unique<float[]>(3 * width * height);

  std::transform(
    inputImage.GetData(),
    inputImage.GetData() + bufferSize,
    input.get(),
    [](float x) -> auto { return x / 255; }
  );


  // output buffer
  auto output = std::make_unique<float[]>(3 * width * height);

  // calc result
  volatile std::clock_t start, time;

  calcTime("CPU", [&input, &output, width, height]() {
    calcCpu(input.get(), output.get(), width, height);
  });

  calcTime("GPU", [&input, &output, width, height]() {
    OpenCLContext context = initGpu(width, height);
  });

  // write result to disk
  auto outputImage = ilImage(inputImage);

  std::transform(
    output.get(),
    output.get() + bufferSize,
    outputImage.GetData(),
    [](float x) -> ILubyte {
      if (x < 0) {
        return 0;
      }

      if (x > 1) {
        return 255;
      }

      return x * 255;
    }
  );

  outputImage.Save("result.png");

  return 0;


  // clEnqueueNDRangeKernel(
  //   cqCommandQueue,
  //   ApplyFilter,
  //   1,
  //   2,
  //   context.global_work_size,
  //   context.local_work_size,
  //   0,
  //   NULL,
  //   NULL
  // );

  // // Copy the output in GPU memory back to CPU memory
  // clEnqueueReadBuffer(
  //   cqCommandQueue,
  //   GPUOutputVector,
  //   CL_TRUE,
  //   0,
  //   SIZE * sizeof(int),
  //   HostOutputVector,
  //   0,
  //   NULL,
  //   NULL
  // );

  // clFinish(cqCommandQueue);

  // // Cleanup
  // clReleaseKernel(ApplyFilter);
  // clReleaseProgram(OpenCLProgram);
  // clReleaseCommandQueue(cqCommandQueue);
  // clReleaseContext(GPUContext);
  // clReleaseMemObject(GPUInputVector);
  // clReleaseMemObject(GPUOutputVector);

  // for( int i =0 ; i < SIZE; i++) {
  //   std::cout << HostInputVector[i] << " * 2 = " << HostOutputVector[i] << std::endl;
  // }

  // return 0;
}
