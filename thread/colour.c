#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void fwrite_pictures(TYPE_ATTR,int);
typedef struct {
double real;
double img;
}TYPE_ATTR;

int main(int argc,char *argv[])
{


int a[] = {1,2,3,4};
int b[] = {10,20,30,50};
int n = 2;
fwrite_pictures(a,b,n);


}



fwrite_pictures(TYPE_ATTR * attractor,int d,int n){



///////////////////////////////////////fwrite with colour///////////////////////////////////
FILE *file = fopen("output_colour.ppm","w");
if (file ==NULL){
    printf("error opening file.\n");
    return -1;
}
int colour_type = {{128,0,128},{128,0,0},{0,255,0},{0,0,255},{255,153,0},{192,192,192},{0,51,102},{255,255,0},{153,51,102},{255,102,0}};


fprintf(file,"P3\n");
fprintf(file,"%i %i\n",n,n);
fprintf(file,"255\n");
int colour_pixel = 255/
for (int ix = 0;ix<n;++ix){
for (int iy = 0;iy<n;++iy){

fprintf(file,"%i %i %i ",colour_type[attractor.real][0],colour_type[attractor.real][1],colour_type[attractor.real][2]);
}
fprintf(file,"\n");
}
fclose(file);
//////////////////////////////////////fwrite with grayscale////////////////////////////////
FILE *file1 = fopen("output_grayscale.ppm","w");
if (file1 ==NULL){
    printf("error opening file.\n");
    return -1;
}
fprintf(file1,"P2\n");
fprintf(file1,"%i %i\n",n,n);
fprintf(file1,"50\n");
for (int ix = 0;ix<n;++ix){
for (int iy = 0;iy<n;++iy){
fprintf(file1,"%i ",attractor.img);
}
fprintf(file1,"\n");
}
fclose(file1);








}










