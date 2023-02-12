#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(
)
{
const long int size = 30000;
int iterations = 200000;
struct timespec bench_start_time;
struct timespec bench_stop_time;
double bench_diff_time;
double *a_re = (double*)malloc(size*sizeof(double));
double *a_im = (double*)malloc(size*sizeof(double));
double *b_re = (double*)malloc(size*sizeof(double));
double *b_im = (double*)malloc(size*sizeof(double));
double *c_re = (double*)malloc(size*sizeof(double));
double *c_im = (double*)malloc(size*sizeof(double));
for(long int ix = 0;ix<size;++ix){
b_re[ix] = ix;
b_im[ix] = ix;
c_re[ix] = ix;
c_im[ix] = ix;
}
timespec_get(&bench_start_time,TIME_UTC);
for (int x = 0;x<200000;++x){
for(int ix = 0;ix<size;++ix){
a_re[ix] = (b_re[ix])*(c_re[ix])-(b_im[ix])*(c_im[ix]);
a_im[ix] = (b_im[ix])*(c_re[ix])+(b_re[ix])*(c_im[ix]);
}
}
timespec_get(&bench_stop_time,TIME_UTC);
printf("%f,%f\n",a_re[20],a_im[20]);
bench_diff_time = difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000 + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
printf("benchmark time for 200,000 iteration: %fmus\n",bench_diff_time);



}

