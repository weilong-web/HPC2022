#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 
void row_sums(double *,double **,size_t,size_t);
void col_sums(double *,double **,size_t,size_t);
void row_suns_unrolled2(double *,double **,size_t,size_t);
void row_suns_unrolled4(double *,double **,size_t,size_t);
void row_suns_unrolled8(double *,double **,size_t,size_t);
void row_suns_unrolled4_array(double *,double **,size_t,size_t);

int main(
int argc,
char *argv[]
)
{
const long int bench_iter = 5000;
struct timespec bench_start_time;
struct timespec bench_stop_time;
struct timespec another_start;
struct timespec another_end;


double bench_diff_time;
double bench_another;

size_t nrs = 1000;
size_t ncs = 1000;
double **as =(double**)malloc(1000*sizeof(double));

for (int ix = 0; ix < 1000;++ix){
as[ix] = (double*)malloc(1000*sizeof(double));
}

for (int ix = 0; ix < 1000;++ix)
  for (int jx = 0; jx < 1000;++jx){
    as[ix][jx] = ix + jx;
}

double *sums_row = (double*)malloc(1000*sizeof(double));
double *sums_col = (double*)malloc(1000*sizeof(double));
timespec_get(&bench_start_time, TIME_UTC);
for (int bx=0;bx<bench_iter;++bx ){
row_sums(sums_row,as,nrs,ncs);
}
timespec_get(&bench_stop_time, TIME_UTC);
bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("benchmark time for 5000 iterations: %fmus\n",
      bench_diff_time);


timespec_get(&bench_start_time, TIME_UTC);
for (int bx=0;bx<bench_iter;++bx ){
col_sums(sums_col,as,nrs,ncs);
}
timespec_get(&bench_stop_time, TIME_UTC);
bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("another benchmark time for 5000 iterations: %fmus\n",
      bench_diff_time);
//rowa2

timespec_get(&bench_start_time, TIME_UTC);
for (int bx=0;bx<bench_iter;++bx ){
row_suns_unrolled2(sums_row,as,nrs,ncs);
}
timespec_get(&bench_stop_time, TIME_UTC);
bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("another benchmark time for 5000 iterations: %fmus\n",
      bench_diff_time);
//rowa4
timespec_get(&bench_start_time, TIME_UTC);
for (int bx=0;bx<bench_iter;++bx ){
row_suns_unrolled4(sums_row,as,nrs,ncs);
}
timespec_get(&bench_stop_time, TIME_UTC);
bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("another benchmark time for 5000 iterations: %fmus\n",
      bench_diff_time);
//rowa8
timespec_get(&bench_start_time, TIME_UTC);
for (int bx=0;bx<bench_iter;++bx ){
row_suns_unrolled8(sums_row,as,nrs,ncs);
}
timespec_get(&bench_stop_time, TIME_UTC);
bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("another benchmark time for 5000 iterations: %fmus\n",
      bench_diff_time);
//row4array
timespec_get(&bench_start_time, TIME_UTC);
for (int bx=0;bx<bench_iter;++bx ){
row_suns_unrolled4_array(sums_row,as,nrs,ncs);
}
timespec_get(&bench_stop_time, TIME_UTC);
bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("another benchmark time for 5000 iterations: %fmus\n",
      bench_diff_time);



printf("%f\n",sums_row[0]);
printf("%f\n",sums_col[0]);
}

void row_sums(double *sums,double ** matrix,size_t nrs,size_t ncs)
{
for (size_t ix = 0; ix<nrs;++ix){
    double sum = 0.;
    for(size_t jx = 0;jx < ncs;++jx){
      sum += matrix[ix][jx];
}
    sums[ix] = sum;
}
}
void col_sums(double *sums,double **matrix,size_t nrs,size_t ncs)
{
for ( size_t jx = 0; jx < ncs; ++jx ) {
    double sum = 0.;
    for ( size_t ix = 0; ix < nrs; ++ix )
      sum += matrix[ix][jx];
    sums[jx] = sum;
  }

}
void row_suns_unrolled2(double *sums,double **matrix,size_t nrs,size_t ncs)
{
for (size_t ix = 0;ix<nrs;++ix){
double sum0 = 0.;
double sum1 = 0.;
for(size_t jx = 0;jx<ncs;jx +=2 ){
sum0 += matrix[ix][jx];
sum1 += matrix[ix][jx+1];
}
sums[ix] = sum0+sum1;
}
}



void row_suns_unrolled4(double *sums,double **matrix,size_t nrs,size_t ncs)
{
for (size_t ix = 0;ix<nrs;++ix){
double sum0 = 0.;
double sum1 = 0.;
double sum2 = 0.;
double sum3 = 0.;
for(size_t jx = 0;jx<ncs;jx +=4 ){
sum0 += matrix[ix][jx];
sum1 += matrix[ix][jx+1];
sum2 += matrix[ix][jx+2];
sum3 += matrix[ix][jx+3];

}
sums[ix] = sum0+sum1+sum2+sum3;
}
}



void row_suns_unrolled8(double *sums,double **matrix,size_t nrs,size_t ncs)
{
for (size_t ix = 0;ix<nrs;++ix){
double sum0 = 0.;
double sum1 = 0.;
double sum2 = 0.;
double sum3 = 0.;
double sum4 = 0.;
double sum5 = 0.;
double sum6 = 0.;
double sum7 = 0.;
for(size_t jx = 0;jx<ncs;jx +=8 ){
sum0 += matrix[ix][jx];
sum1 += matrix[ix][jx+1];
sum2 += matrix[ix][jx+2];
sum3 += matrix[ix][jx+3];
sum4 += matrix[ix][jx+4];
sum5 += matrix[ix][jx+5];
sum6 += matrix[ix][jx+6];
sum7 += matrix[ix][jx+7];
}
sums[ix] = sum0+sum1+sum2+sum3+sum4+sum5+sum6+sum7;
}
}

void row_suns_unrolled4_array(double *sums,double **matrix,size_t nrs,size_t ncs)
{
for (size_t ix = 0;ix<nrs;++ix){

double sum[4]={0.,0.,0.,0.};

for(size_t jx = 0;jx<ncs;jx +=4 ){
sum[0] += matrix[ix][jx];
sum[1] += matrix[ix][jx+1];
sum[2] += matrix[ix][jx+2];
sum[3] += matrix[ix][jx+3];
}
sums[ix] = sum[0]+sum[1]+sum[2]+sum[3];
}
}




