#include <stdlib.h>
#include <stdio.h>
int
main(
int argc,
char* argv[]
)
{
int size = 10;
int **as = (int**)malloc(sizeof(int*)*size);
for(size_t ix = 0; ix < size; ++ix){
as[ix]=(int*)malloc(sizeof(int)*10);
}
for(size_t ix = 0; ix < size; ++ix)
  for (size_t iy = 0; iy < size; ++iy){
as[ix][iy] = ix*iy;
}
FILE *file = fopen("test_file.txt","w");
if (file == NULL){
printf("Error opening file\n");
}
for(size_t ix = 0; ix < size; ++ix)
  for (size_t iy = 0; iy < size; ++iy){
fprintf(file, "%i\n", as[ix][iy]);
}
fclose(file);
FILE *datfile = fopen("test_file.dat","w");
if (datfile == NULL){
printf("Error opening file\n");
}
for(size_t ix = 0; ix < size; ++ix)
  for (size_t iy = 0; iy < size; ++iy){
fprintf(datfile, "%i\n", as[ix][iy]);
}
fclose(datfile);

free(**as);
free(as);
//below is to read files and put it into the matrix format.
FILE *readfile = fopen("test_file.txt","r");
if (readfile == NULL){
printf("Error Reading from file\n");
}
int read_matrix[10][10];
for(size_t ix = 0; ix < size; ++ix)
  for (size_t iy = 0; iy < size; ++iy){
int r = fscanf(readfile,"%i",&read_matrix[ix][iy]);
}
for(size_t ix = 0; ix < size; ++ix){
  printf("\n");

  for (size_t iy = 0; iy < size; ++iy){

printf("%i\t",read_matrix[ix][iy]);
}
printf("\n");
}
int count = 0;

for(size_t ix = 0; ix < size; ++ix)
  for (size_t iy = 0; iy < size; ++iy){
if (read_matrix[ix][iy] != ix*iy){
count += 1;
}
}
if (count == 0){
printf(" entry in the ix-th row and jx-th column equals ix*jx.");
printf("\n");
}




}





