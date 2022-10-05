#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>

static inline int read_file(short int*,char *,int,int);
static inline short int ** ComputeWithinBlock (short int ** block1, short int ** output);
static inline short int ** ComputeAcrossBlock (short int ** block1,short int ** block2,short int ** output);


int main(
int argc,
char *argv[];
)

{
int file_length = 10000;
int total_blocks = 2;
int file_each = file_length/total_blocks;
char *filename = "cell_e4";
short int *printout_entries = (short int*)malloc(3500*2*sizeof(short int));
short int **printout_list = (short int**)malloc(sizeof(short int*)*3500);
for (size_t i = 0;i<3500;++i){
printout_list[i] = printout_entries+2*i;
printout_list[i][0] = -1;
printout_list[i][1] = 0;}
//test for the outlist
//for (size_t i = 0;i<5;++i){
//printf("%i\n",printout_list[i][0]);}

short int *block1_asentries = (short int*)malloc(file_each*3*sizeof(short int));
short int **block1 = (short**)malloc(sizeof(short int*)*file_each);
short int *block2_asentries = (short int*)malloc(file_each*3*sizeof(short int));
short int **block2 = (short**)malloc(sizeof(short int*)*file_each);
for (size_t i=0;i<file_each;++i){
block1[i] = block1_asentries + 3*i;}
for (size_t i=0;i<file_each;++i){
block2[i] = block2_asentries + 3*i;}

for(size_t i = 0; i<total_blocks-1;++i){
read_file(block1_asentries,filename,i*file_each,(i+1)*file_each);

ComputeWithinBlock(block1,printout_list);
//for (int ix = 0;ix<45;ix++){
//printf("%i %i\n",printout_list[ix][0],printout_list[ix][1]);
//}
for(size_t j = i+1;j<total_blocks;++j){
read_file(block2_asentries,filename,j*file_each,(j+1)*file_each);
ComputeAcrossBlock(block1,block2,printout_list);
}
}
read_file(block1_asentries,filename,file_length-file_each,file_length);
ComputeWithinBlock(block1,printout_list);

//for (int ix = 0; ix < 10; ++ix){
//printf("%i %i %i\n",block1[ix][0],block1[ix][1],block1[ix][2]);}






//free(block1_asentries);
//free(block1);
//free(block2_asentries);
//free(block2);
int m = 0;
while(printout_list[m][0]!=-1)
{
  printf(" %.2f %i\n ",(float)printout_list[m][0]/100,printout_list[m][1]);
  m = m + 1;
}

}
//printout needs to be written

//print_read file



int read_file (short int *coords,char *filename, int start, int end) 
{
int size = end - start;
int lsz = 24; 
char *lines = (char*) malloc( sizeof(char)*size*lsz);
short int pos[3];

FILE *file = fopen(filename, "r");
if (file == NULL)
{
printf("error opening file\n");
return -1;
}

fseek(file, start*lsz, 0); 
fread (lines, sizeof(char), size*lsz, file); 
fclose(file);
for (int ix = 0; ix < size; ++ix)
{
int lskip = ix*lsz; 
pos[0] = (44-lines[0 + lskip]) * ((lines[ 1 + lskip]-48)*10000+ (lines[ 2 + lskip]-48)*1000
           + (lines[ 4 + lskip]-48)*100
           + (lines[ 5 + lskip]-48)*10
           + (lines[ 6 + lskip]-48)*1
);
pos[1] = (44-lines[8 + lskip]) * ((lines[9 + lskip]-48)*10000+(lines[10+lskip]-48)*1000
           + (lines[12 + lskip]-48)*100
           + (lines[13 + lskip]-48)*10
           + (lines[14 + lskip]-48)*1
);
pos[2] = (44-lines[16 + lskip]) * ((lines[17 + lskip]-48)*10000
           + (lines[18 + lskip]-48)*1000
           + (lines[20 + lskip]-48)*100
           + (lines[21 + lskip]-48)*10
           + (lines[22 + lskip]-48)*1
);
memcpy(coords+3*ix, pos, 3*sizeof(short int));
}

free(lines);
return 0;
}



/* Computes The Distance within a block */
short ** ComputeWithinBlock (short int ** block1,/* float * entries, short int * entries_int_comp,*/ short int** output)
{
 short int distentry =0;
   // int iec =0;
    // int ie =0 ;
     int iecf =0 ; /* for traversing through the output  to calculate the frequency*/
     int flag =0 ; /* Flag used in frequency*/
     int regrp =0 ; /*iteration variable rearranging the output matrix of distance and frequency */
    short int tempDist=-1; /* temporary variable for regrouping the  distance component of output matrix */
    short int tempFreq1=0; /*temporary variable for regrouping the frequency component of output matrix   */
    short int tempFreq2=0; /*temporary variable for regrouping the frequency component of output matrix   */


    //short int n;
  for (size_t i=0 ;i < 100 ; i++ )
    for (size_t j=i+1; j < 100 ; j++)
  {

                   distentry= (short int) roundf(sqrtf( (( ( (float)block1[i][0]/1000)
                                                                     - (( float)block1[j][0]/1000)  ) * ( ((float)block1[i][0]/1000) - ((float)block1[j][0]/1000)))+ (( ((float)block1[i][1]/1000) - ((float)block1[j][1]/1000)  ) * ( ( (float)block1[i][1]/1000)
                                                                                                                                                                                                                                                          - ( (float) block1[j][1]/1000))) + (( ( (float) block1[i][2]/1000)
                                                                                                                                                                                                                                                                                                                             - ( (float) block1[j][2]/1000)  ) * ( ( (float) block1[i][2]/1000) - ( (float) block1[j][2]/1000)))) * 100 );

          while (output[iecf][0]!=-1)
          {
              if (distentry==output[iecf][0] )
                  {
                      output[iecf][1]= output[iecf][1] +1 ;
                      flag = 1;
                      break;
                  }

              else if (iecf==0 && distentry < output[iecf][0] )  /* if the new element has to be placed as the first element of the updated output matrix */
              {
                        regrp = iecf ;
                   while (output[regrp][0]!= -1)
                   {
                      tempDist = output[regrp][0];
                      tempFreq1 = output[regrp][1];
                      output[regrp][0]= distentry ;
                      output[regrp][1]= tempFreq2 + 1;
                      distentry=tempDist;
                      tempFreq2=tempFreq1 -1;
                      regrp++;


                   }
                    output[regrp][0]= distentry ;
                    output[regrp][1]= tempFreq2 + 1;
                    regrp=0;
                    flag =1;

              break;


              }

              else if (distentry > output[iecf][0] && distentry <  output[iecf + 1][0]) /* if the new element has to be placed somewhere in between */

              {

                   regrp = iecf + 1;
                   while (output[regrp][0]!= -1)
                   {
                      tempDist = output[regrp][0];
                      tempFreq1 = output[regrp][1];
                      output[regrp][0]= distentry ;
                      output[regrp][1]= tempFreq2 + 1;
                      distentry=tempDist;
                      tempFreq2=tempFreq1 -1;
                      regrp++;


                   }
                    output[regrp][0]= distentry ;
                    output[regrp][1]= tempFreq2 + 1;
                    regrp=0;
                    flag =1;

              break;
              }

              else if (distentry > output[iecf][0] && output[iecf + 1][0]==-1) /* if the new element has to be placed after the last element of the updated output matrix */
              {
                  output[iecf + 1][0]=distentry;
                  output[iecf + 1][1]= 1;
                   flag =1;
                  break;
              }
           iecf ++ ;

          }
          if (flag ==0 )
          {
             output[iecf][0]=distentry;
             output[iecf][1]= output[iecf][1] +1 ;
          }


 iecf=0;
 flag = 0;



  }


return output;


}

/* Computes The Distance Across two blocks */
short ** ComputeAcrossBlock (short int ** block1,short int ** block2,/* float * entries, short int * entries_int_comp,*/ short int** output)
{
 short int distentry =0;
   // int iec =0;
    // int ie =0 ;
     int iecf =0 ; /* for traversing through the output  to calculate the frequency*/
     int flag =0 ; /* Flag used in frequency*/
     int regrp =0 ; /*iteration variable rearranging the output matrix of distance and frequency */
    short int tempDist=-1; /* temporary variable for regrouping the  distance component of output matrix */
    short int tempFreq1=0; /*temporary variable for regrouping the frequency component of output matrix   */
    short int tempFreq2=0; /*temporary variable for regrouping the frequency component of output matrix   */


    //short int n;
  for (size_t i=0 ;i < 100 ; i++ )
    for (size_t j=0; j < 100 ; j++)
  {

                     distentry= (short int) roundf(sqrtf( (( ( (float)block1[i][0]/1000)
                                                                     - (( float)block2[j][0]/1000)  ) * ( ((float)block1[i][0]/1000) - ((float)block2[j][0]/1000)))+ (( ((float)block1[i][1]/1000) - ((float)block2[j][1]/1000)  ) * ( ( (float)block1[i][1]/1000)
                                                                                                                                                                                                                                                          - ( (float) block2[j][1]/1000))) + (( ( (float) block1[i][2]/1000)
                                                                                                                                                                                                                                                                                                                             - ( (float) block2[j][2]/1000)  ) * ( ( (float) block1[i][2]/1000) - ( (float) block2[j][2]/1000)))) * 100 );


          while (output[iecf][0]!=-1)
          {
              if (distentry==output[iecf][0] )
                  {
                      output[iecf][1]= output[iecf][1] +1 ;
                      flag = 1;
                      break;
                  }

              else if (iecf==0 && distentry < output[iecf][0] )  /* if the new element has to be placed as the first element of the updated output matrix */
              {
                        regrp = iecf ;
                   while (output[regrp][0]!= -1)
                   {
                      tempDist = output[regrp][0];
                      tempFreq1 = output[regrp][1];
                      output[regrp][0]= distentry ;
                      output[regrp][1]= tempFreq2 + 1;
                      distentry=tempDist;
                      tempFreq2=tempFreq1 -1;
                      regrp++;


                   }
                    output[regrp][0]= distentry ;
                    output[regrp][1]= tempFreq2 + 1;
                    regrp=0;
                    flag =1;

              break;


              }

              else if (distentry > output[iecf][0] && distentry <  output[iecf + 1][0]) /* if the new element has to be placed somewhere in between */

              {

                   regrp = iecf + 1;
                   while (output[regrp][0]!= -1)
                   {
                      tempDist = output[regrp][0];
                      tempFreq1 = output[regrp][1];
                      output[regrp][0]= distentry ;
                      output[regrp][1]= tempFreq2 + 1;
                      distentry=tempDist;
                      tempFreq2=tempFreq1 -1;
                      regrp++;


                   }
                    output[regrp][0]= distentry ;
                    output[regrp][1]= tempFreq2 + 1;
                    regrp=0;
                    flag =1;

              break;
              }

              else if (distentry > output[iecf][0] && output[iecf + 1][0]==-1) /* if the new element has to be placed after the last element of the updated output matrix */
              {
                  output[iecf + 1][0]=distentry;
                  output[iecf + 1][1]= 1;
                   flag =1;
                  break;
              }
           iecf ++ ;

          }
          if (flag ==0 )
          {
             output[iecf][0]=distentry;
             output[iecf][1]= output[iecf][1] +1 ;
          }

 iecf=0;
 flag = 0;
  }
return output;

}












