#include <stdio.h>
#include <math.h>

int main(
int argc;
char *argv[];
)

{
long int size = 1000000000;
// long int no more than 1410065407
long int note = 0;
for (long int ix = 0; ix < size; ++ix)
{
note += ix;
}
printf("The sum from 1 to %i is %i,\n", size, note);



}
