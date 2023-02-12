#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
int main(
int argc,
char* argv[]
)


{
int a;
int b;
for (int ix = 1; ix < argc; ++ix)
{
if (argv[ix][1] == 'a'){
a = atoi(argv[ix][2]);
}
//Q: why it's 'a' but not "a"? what's the difference?
else if (argv[ix][1] == 'b'){
b = atoi(argv[ix][2]);
}
}
printf("A is equal to %i and B is equal to %i.\n",a,b);
return 0;
}

