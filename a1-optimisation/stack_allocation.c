#include <stdio.h>
int
main(
int argc,
int *argv[];
)
{
int size = 10000000;
int as[size];
for (int ix = 0; ix < size; ++ix)
	as [ix] = 0;
printf("%d\n", as[0]);
}
