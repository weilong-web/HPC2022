#include <stdlib.h>
#include <stdio.h>
#include "different_file.h"
#include <time.h>




int main(
)
{

struct timespec bench_start_time;
struct timespec bench_stop_time;
double bench_diff_time;

const int size = 30000;
double *a_re = (double*)malloc(size*sizeof(double));
double *a_im = (double*)malloc(size*sizeof(double));
double *b_re = (double*)malloc(size*sizeof(double));
double *b_im = (double*)malloc(size*sizeof(double));
double *c_re = (double*)malloc(size*sizeof(double));
double *c_im = (double*)malloc(size*sizeof(double));


for (int ix = 0; ix < size; ++ix){
b_re[ix] = ix;
b_im[ix] = ix;
c_re[ix] = ix;
c_im[ix] = ix;
}
timespec_get(&bench_start_time, TIME_UTC);
for (int x = 0;x<200000;++x){

for (int ix = 0; ix < size; ++ix){
mul_cpx(&a_re[ix],&a_im[ix],&b_re[ix],&b_im[ix],&c_re[ix],&c_im[ix]);
}
}
timespec_get(&bench_stop_time, TIME_UTC);

printf("%f,%f\n",a_re[20],a_im[20]);
bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("benchmark time for 200,000 iterations: %fmus\n",
      bench_diff_time);



}
