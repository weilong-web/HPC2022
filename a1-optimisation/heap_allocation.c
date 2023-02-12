#include <stdlib.h>
#include <stdio.h>
int main(
int argc,
char* argv[]
)
{
int size = 10000000;
int *as = (int*)malloc(sizeof(int)*size);
for (int ix = 0; ix < size; ++ix)
	as[ix] = 0;
printf("%d\n",as[0]);
free(as);
}
