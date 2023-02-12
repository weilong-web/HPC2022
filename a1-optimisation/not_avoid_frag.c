#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main(
	int argc,
	char* argv[]
)
{


int size = 10;
//as is pointers for each row start
int ** as = (int**) malloc(sizeof(int*) * size);
//each ix is malloc on heap but not contiguous
for ( size_t ix = 0; ix < size; ++ix )
  as[ix] = (int*) malloc(sizeof(int) * 1000000);


int* tmp = &as[0][0];
for ( size_t ix = 0; ix < size; ++ix )
  for ( size_t jx = 0; jx < size; ++jx ){
    as[ix][jx] = 0;
    printf("%zd\n", &as[ix][jx] - tmp);
    tmp = &as[ix][jx];
    printf("%p\n",&as[ix][jx]);
}
printf("%d\n", as[0][0]);
for ( size_t ix = 0; ix < size; ++ix )
    free(as[ix]);
free(as);

}
