#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

void outMatrix(double*, int, int);
double calAvg(double * , int, int);

int main(int argc, char *argv[]) {
  int iterations = 100;
  float constant = 0.01;
  

  cl_int error;

  cl_platform_id platform_id;
  cl_uint nmb_platforms;
  if (clGetPlatformIDs(1, &platform_id, &nmb_platforms) != CL_SUCCESS)
  {
    fprintf(stderr, "cannot get platform\n");
    return 1;
  }

  cl_device_id device_id;
  cl_uint nmb_devices;
  if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &nmb_devices) != CL_SUCCESS)
  {
    fprintf(stderr, "cannot get device\n");
    return 1;
  }

  cl_context context;
  cl_context_properties properties[] =
      {
          CL_CONTEXT_PLATFORM,
          (cl_context_properties)platform_id,
          0};
  context = clCreateContext(properties, 1, &device_id, NULL, NULL, &error);

  if (error != CL_SUCCESS)
  {
    fprintf(stderr, "cannot create context\n");
    return 1;
  }

  cl_command_queue command_queue;
  command_queue = clCreateCommandQueueWithProperties(context, device_id, NULL, &error);

  if (error != CL_SUCCESS)
  {
    fprintf(stderr, "cannot create command queue\n");
    return 1;
  }

  char *opencl_program_src;
  {
    FILE *clfp = fopen("./openwithout.cl", "r");
    if (clfp == NULL)
    {
      fprintf(stderr, "could not load cl source code\n");
      return 1;
    }
    fseek(clfp, 0, SEEK_END);
    int clfsz = ftell(clfp);
    fseek(clfp, 0, SEEK_SET);
    opencl_program_src = (char *)malloc((clfsz + 1) * sizeof(char));
    fread(opencl_program_src, sizeof(char), clfsz, clfp);
    opencl_program_src[clfsz] = 0;
    fclose(clfp);
  }

  cl_program program;
  size_t src_len = strlen(opencl_program_src);
  program = clCreateProgramWithSource(
      context, 1, (const char **)&opencl_program_src, (const size_t *)&src_len, &error);
  if (error != CL_SUCCESS)
  {
    fprintf(stderr, "cannot create program\n");
    return 1;
  }

  free(opencl_program_src);

  error = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (error != CL_SUCCESS)
  {
    fprintf(stderr, "cannot build program. log:\n");

    size_t log_size = 0;
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

    char *log = malloc(log_size * sizeof(char));
    if (log == NULL)
    {
      fprintf(stderr, "could not allocate memory\n");
      return 1;
    }

    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

    fprintf(stderr, "%s\n", log);

    free(log);

    return 1;
  }

  cl_kernel kernel = clCreateKernel(program, "diffuse", &error);
  if (error != CL_SUCCESS)
  {
    fprintf(stderr, "cannot create kernel\n");
    return 1;

  }
  cl_kernel kernel1 = clCreateKernel(program, "diffuse", &error);
  if (error != CL_SUCCESS)
  {
    fprintf(stderr, "cannot create kernel\n");
    return 1;

  }
  
  if (argc > 1) {
    if (argv[1][0] == '-' && argv[1][1] == 'n') {
      iterations = atoi(argv[1] + 2);
    }
    if (argv[2][0] == '-' && argv[2][1] == 'd') {
      sscanf(argv[2] + 2, "%f", &constant);
    }
    printf("%i,%f\n",iterations, constant);
  }

  int rows;
  int cols;

  FILE *file = fopen("init_100000_100", "r");
  fscanf(file, "%d %d\n", &rows, &cols);
  double *heatMatrix = (double*)malloc(sizeof(double) * (rows + 2) * (cols + 2));
  memset(heatMatrix, 0, sizeof(double) * (rows + 2) * (cols + 2));
  double *nextMatrix = (double*)malloc(sizeof(double) * (rows + 2) * (cols + 2));
  memset(nextMatrix, 0, sizeof(double) * (rows + 2) * (cols + 2));
  int wholeRows = rows + 2;
  int wholeCols = cols + 2;
  int index = 0;
  int x, y;
  double z;
  while (fscanf(file, "%i %i %lf", &x, &y, &z) == 3) {
    //fprintf(stderr, "%i (%lf) %i (%i)\n", x, z, y, cols);
    double q = z;
    heatMatrix[(x + 1) * wholeCols + y + 1] = q;
    index++;
  }
  // init opencl matrix
    cl_mem input_buffer_now, output_buffer_next;
    input_buffer_now = clCreateBuffer(context, CL_MEM_READ_WRITE,
                                    wholeCols * wholeRows * sizeof(double), NULL, &error);
    if (error != CL_SUCCESS)
    {
      fprintf(stderr, "cannot create buffer a\n");
      return 1;
    }
    
    output_buffer_next = clCreateBuffer(context, CL_MEM_READ_WRITE,
                                    wholeCols * wholeRows * sizeof(double), NULL, &error);
    if (error != CL_SUCCESS)
    {
      fprintf(stderr, "cannot create buffer next\n");
      return 1;
    }
    
    if (error != CL_SUCCESS) {
    fprintf(stderr, "cannot create buffer c_sum\n");
    return 1;
    }
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer_now);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer_next);
    clSetKernelArg(kernel, 2, sizeof(float), &constant);
    clSetKernelArg(kernel, 3, sizeof(int), &rows);
    clSetKernelArg(kernel, 4, sizeof(int), &cols);
    clSetKernelArg(kernel1, 0, sizeof(cl_mem), &output_buffer_next);
    clSetKernelArg(kernel1, 1, sizeof(cl_mem), &input_buffer_now);
    clSetKernelArg(kernel1, 2, sizeof(float), &constant);
    clSetKernelArg(kernel1, 3, sizeof(int), &rows);
    clSetKernelArg(kernel1, 4, sizeof(int), &cols);


    const size_t global_sz[] = {wholeRows,wholeCols}; 



    if (clEnqueueWriteBuffer(command_queue,
                            input_buffer_now, CL_TRUE, 0, wholeCols * wholeRows * sizeof(double), heatMatrix, 0, NULL, NULL) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot enqueue write of buffer a\n");
      return 1;
    }


for(int it = 0; it < iterations/2; it++){
   

    if (clEnqueueNDRangeKernel(command_queue, kernel,
                              2, NULL, (const size_t *)&global_sz,
                                NULL, 0, NULL, NULL) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot enqueue kernel\n");
      return 1;
    }
    if (clEnqueueNDRangeKernel(command_queue, kernel1,
                              2, NULL, (const size_t *)&global_sz,
                                NULL, 0, NULL, NULL) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot enqueue kernel\n");
      return 1;
    }
}
    if (clEnqueueReadBuffer(command_queue,
                            output_buffer_next,
                            CL_TRUE, 0,
                              wholeCols * wholeRows * sizeof(double),
                              nextMatrix, 0, NULL, NULL) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot enqueue read of buffer res\n");
      return 1;
    }
    if ( clEnqueueBarrierWithWaitList(command_queue, 0, NULL, NULL) != CL_SUCCESS ) {
    printf("cannot enqueue barrier\n");
    return 1;
    }

    if (clFinish(command_queue) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot finish queue\n");
      return 1;
    }
    //memcpy(heatMatrix, nextMatrix, sizeof(double) * (rows + 2) * (cols + 2));
   
  // for (int i = 1; i < rows + 1; ++i) {
  //     for (int j = 1; j < cols + 1; ++j) {
  //         printf("%f \t", heatMatrix[i * (cols + 2) + j]);
  //     }
  //     printf("\n");
  // }
  printf("%lf\n",nextMatrix[999999]);
  //outMatrix(nextMatrix, rows, cols);
  calAvg(nextMatrix,rows,cols);
  //printf("Average equals %f\n", c_sum_total/rows/cols);
  free(heatMatrix);
  free(nextMatrix);
  // free(res);
 
  clReleaseMemObject(input_buffer_now);
  clReleaseMemObject(output_buffer_next);

  
  
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseKernel(kernel1);


  clReleaseCommandQueue(command_queue);
  clReleaseContext(context);

  return 0;
}

 void outMatrix(double *matrix, int rows, int cols) {
  // printf("%.1f\n",matrix[99][14]);q

   for (int i = 1; i < rows + 1; ++i) {
     for (int j = 1; j < cols + 1; ++j) {
       printf("%.1f \t ", matrix[i*(cols+2)+j]);
     }
     printf("\n");
   }
 }
double calAvg(double * matrix, int rows, int cols){
    long double sum = 0;
    for (int i = 1; i < rows + 1; ++i) {
        for (int j = 1; j < cols + 1; ++j) {
            sum += matrix[i * (cols+2) + j];
        }
    }

    double matrixAvg = sum / rows / cols;
    printf("avg formula: %.1f\n ",matrixAvg);
}
// read and convert problem here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// read the first index from start of the file and store it into

// memory allocation function
// double **mallocArray(int rows, int cols) {

//   double *arr_asentries = (double *)malloc(rows * cols * sizeof(double));
//   memset(arr_asentries, 0, sizeof(double) * rows * cols);
//   double **arr = (double **)malloc(sizeof(double *) * rows);
//   for (size_t i = 0; i < rows; ++i) {
//     arr[i] = arr_asentries + rows * i;
//   }

//   return arr;
// }

// void diffuse(int posX, int posY, double **nowMatrix, double **nextMatrix,
//              float constant) {
//   // h(i, j) = h(i, j) + c * ((h(i - 1, j) + h(i + 1, j) + h(i, j - 1) + h(i, j
//   // + 1)) / 4 - h(i,j))
//   float tmpVal = nowMatrix[posX][posY];
//   float surrounding = (nowMatrix[posX - 1][posY] + nowMatrix[posX + 1][posY] +
//                        nowMatrix[posX][posY - 1] + nowMatrix[posX][posY + 1]) /
//                           4 -
//                       tmpVal;
//   float nextValue = tmpVal + constant * surrounding;
//   nextMatrix[posX][posY] = nextValue;
// }



// float calAvgDiff(double **matrix, float avg, int rows, int cols) {
//   long double avgDiff = 0;
//   for (int i = 1; i < rows + 1; ++i) {
//     for (int j = 1; j < cols + 1; ++j) {
//       avgDiff += abs(avg - matrix[i][j]);
//     }
//   }

//   float matrixAvgDiff = avgDiff / rows / cols;
//   return matrixAvgDiff;
// }


