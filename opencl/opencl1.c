#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

void outMatrix(double*, int, int);
double calAvg(double * , int, int);
double calAvgDiff(double * , double, int, int);


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
    FILE *clfp = fopen("./open.cl", "r");
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
  cl_kernel kernel_reduction = clCreateKernel(program, "calAvg", &error);
  if (error != CL_SUCCESS)
  {
    fprintf(stderr, "cannot create kernel\n");
    return 1;

  }  
  if (argc > 1) {
    if (argv[1][0] == '-' && argv[1][1] == 'n') {
      iterations = atoi(argv[1] + 2);
    }
    if (argv[2][0] == '-' && argv[2][1] == 'n') {
      iterations = atoi(argv[2] + 2);
    }


    if (argv[2][0] == '-' && argv[2][1] == 'd') {
      sscanf(argv[2] + 2, "%f", &constant);
    }
    if (argv[1][0] == '-' && argv[1][1] == 'd') {
      sscanf(argv[1] + 2, "%f", &constant);
    }
    //printf("%i,%f\n",iterations, constant);
  }

  int rows;
  int cols;

  FILE *file = fopen("init_3_3", "r");
//don't forget to change the local size if wanna submit again


  fscanf(file, "%d %d\n", &rows, &cols);
  double *heatMatrix = (double*)malloc(sizeof(double) * (rows) * (cols));
  memset(heatMatrix, 0, sizeof(double) * (rows) * (cols));
  double *nextMatrix = (double*)malloc(sizeof(double) * (rows) * (cols));
  memset(nextMatrix, 0, sizeof(double) * (rows) * (cols));
  //int wholeRows = rows + 2;
  //int wholeCols = cols + 2;
  int index = 0;
  int x, y;
  double z;
  while (fscanf(file, "%i %i %lf", &x, &y, &z) == 3) {
    //fprintf(stderr, "%i (%lf) %i (%i)\n", x, z, y, cols);
    double q = z;
    heatMatrix[(x) * cols + y] = q;
    index++;
  }
  // init opencl matrix
  cl_mem input_buffer_now, output_buffer_next,output_buffer_c_sum;;
    input_buffer_now = clCreateBuffer(context, CL_MEM_READ_WRITE,
                                    rows * cols * sizeof(double), NULL, &error);
    if (error != CL_SUCCESS)
    {
      fprintf(stderr, "cannot create buffer a\n");
      return 1;
    }
    
    output_buffer_next = clCreateBuffer(context, CL_MEM_READ_WRITE,
                                    cols * rows * sizeof(double), NULL, &error);
    if (error != CL_SUCCESS)
    {
      fprintf(stderr, "cannot create buffer next\n");
      return 1;
    }
    const int global_redsz = 1024;
    const int local_redsz = 32;
    const int nmb_redgps = global_redsz / local_redsz;
    output_buffer_c_sum = clCreateBuffer(context, CL_MEM_WRITE_ONLY, nmb_redgps*sizeof(double), NULL, &error);
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


    const size_t global_sz[] = {rows,cols}; 
    const size_t local_work_size[2]= {1,1};


    if (clEnqueueWriteBuffer(command_queue,
                            input_buffer_now, CL_TRUE, 0, rows * cols * sizeof(double), heatMatrix, 0, NULL, NULL) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot enqueue write of buffer a\n");
      return 1;
    }


for(int it = 0; it < iterations/2; it++){
   

    if (clEnqueueNDRangeKernel(command_queue, kernel,
                              2, NULL, (const size_t *)&global_sz,
                                (const size_t *)&local_work_size, 0, NULL, NULL) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot enqueue kernel\n");
      return 1;
    }
    if (clEnqueueNDRangeKernel(command_queue, kernel1,
                              2, NULL, (const size_t *)&global_sz,
                                (const size_t *)&local_work_size, 0, NULL, NULL) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot enqueue kernel\n");
      return 1;
    }
}
    if (clEnqueueReadBuffer(command_queue,
                            output_buffer_next,
                            CL_TRUE, 0,
                              rows * cols * sizeof(double),
                              nextMatrix, 0, NULL, NULL) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot enqueue read of buffer res\n");
      return 1;
    }
    if ( clEnqueueBarrierWithWaitList(command_queue, 0, NULL, NULL) != CL_SUCCESS ) {
    printf("cannot enqueue barrier\n");
    return 1;
    }

    int szz = (rows)*(cols);
    const cl_int sz_clint = (cl_int)szz;
    clSetKernelArg(kernel_reduction, 0, sizeof(cl_mem), &output_buffer_next);
    clSetKernelArg(kernel_reduction, 1, local_redsz * sizeof(double), NULL);
    clSetKernelArg(kernel_reduction, 2, sizeof(cl_int), &sz_clint);
    clSetKernelArg(kernel_reduction, 3, sizeof(cl_mem), &output_buffer_c_sum);

    size_t global_redsz_szt = (size_t) global_redsz;
    size_t local_redsz_szt = (size_t) local_redsz;

    if ( clEnqueueNDRangeKernel(command_queue,
           kernel_reduction, 1, NULL, (const size_t *) &global_redsz_szt, (const size_t *) &local_redsz_szt,
           0, NULL, NULL)!= CL_SUCCESS) 
    {
    fprintf(stderr, "cannot enqueue kernel reduction\n");
    return 1;
    }
    double *c_sum = malloc(nmb_redgps*sizeof(double));
    if ( clEnqueueReadBuffer(command_queue,
           output_buffer_c_sum, CL_TRUE, 0, nmb_redgps*sizeof(double), c_sum, 0, NULL, NULL)
      != CL_SUCCESS) {
    fprintf(stderr, "cannot enqueue read of buffer c\n");
    return 1;
    }

    if (clFinish(command_queue) != CL_SUCCESS)
    {
      fprintf(stderr, "cannot finish queue\n");
      return 1;
    }
    //memcpy(heatMatrix, nextMatrix, sizeof(double) * (rows + 2) * (cols + 2));
    double c_sum_total = 0.f;
    for (size_t ix = 0; ix < nmb_redgps; ++ix){
    c_sum_total += c_sum[ix];}

    
  // for (int i = 1; i < rows + 1; ++i) {
  //     for (int j = 1; j < cols + 1; ++j) {
  //         printf("%f \t", heatMatrix[i * (cols + 2) + j]);
  //     }
  //     printf("\n");
  // }
  //printf("%lf\n",nextMatrix[999]);
  //outMatrix(nextMatrix, rows, cols);
  double avg = calAvg(nextMatrix,rows,cols);
  calAvgDiff(nextMatrix,avg,rows,cols);
  //printf("Average equals %f\n", c_sum_total/rows/cols);
  free(heatMatrix);
  free(nextMatrix);
  // free(res);
  free(c_sum);
  clReleaseMemObject(input_buffer_now);
  clReleaseMemObject(output_buffer_next);
  clReleaseMemObject(output_buffer_c_sum);
  
  
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseKernel(kernel1);
  clReleaseKernel(kernel_reduction);

  clReleaseCommandQueue(command_queue);
  clReleaseContext(context);

  return 0;
}

 void outMatrix(double *matrix, int rows, int cols) {
  // printf("%.1f\n",matrix[99][14]);

   for (int i = 0; i < rows; ++i) {
     for (int j = 0; j < cols; ++j) {
       printf("%.1f \t ", matrix[i*(cols)+j]);
     }
     printf("\n");
   }
 }
double calAvg(double * matrix, int rows, int cols){
    double sum = 0;
    for (int i = 0; i < rows ; ++i) {
        for (int j = 0; j < cols ; ++j) {
            sum += matrix[i * (cols) + j];
        }
    }

    double matrixAvg = sum / rows / cols;
    printf("%.1f\n",matrixAvg);
    return matrixAvg;
}
double calAvgDiff(double * matrix, double avg, int rows, int cols){
    double avgDiff = 0;
    //printf("ffff%lf\n",avg);
    for (int i = 0; i < rows ; ++i) {
        for (int j = 0; j < cols ; ++j) {
         // printf("%lf\n",matrix[i * (cols+2) + j]);
            avgDiff += fabs(avg - matrix[i * (cols) + j]);
            //printf("average difference: %.1f\n",avgDiff);
        }
    }
   // printf("average difference: %.1f\n",avgDiff);
    double AVGDIFF = avgDiff/rows/cols;
    printf("%.1f\n",AVGDIFF);
}
