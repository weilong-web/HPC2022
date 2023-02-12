#include <stdlib.h>
#include <string.h>
#include <math.h>

int thread_n;
int line_n;
int size_n;



int main(int argc, char *argv[])
{
if(argc>1){
    if (argv[1][0] == '-'&&argv[1][1] == 't'){
	thread_n = atoi(argv[1]+2);}
    if (argv[2][0] == '-'&&argv[2][1] == 'l'){
	line_n = atoi(argv[2]+2);}
    size_n = atoi(argv[3]);
}
printf("%i %i %i\n",thread_n, line_n, size_n);
//from -2 ,2 two-dimensional array discrete with line_n points;




}
