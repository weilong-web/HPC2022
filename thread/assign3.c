#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<threads.h>
#include<string.h>
#include <complex.h>

typedef struct{

int val;
char pad[60];


}int_padded;

typedef struct{
mtx_t *mtx;
cnd_t *cnd;
int nmbr_thrds;
int sz;
int_padded *status;
}thrd_info_write_t;

typedef struct{
mtx_t *mtx;	
cnd_t *cnd;
double step;
int sz;
int_padded *status;
int ib;
int tx;
int nmbr_thrds;
// Andhika Code Required variables 
// Polynomial Degree
int d;
double complex * root_thrd_var;
}thrd_info_compute_t;



typedef struct {
double real;
double img;

}TYPE_ATTR;

typedef struct{
short int itr;
short int label;
}TYPE_CONV;

TYPE_ATTR ** attractors;
TYPE_CONV ** convergences; //1

//TYPE_ATTR** attractors   =   (TYPE_ATTR **)   malloc(1000  *  sizeof(TYPE_ATTR *));
//TYPE_CONV** convergences =   (TYPE_CONV **)   malloc( 1000 * sizeof(TYPE_CONV *));

void analytical_root (int, double complex *);
TYPE_CONV* root (TYPE_ATTR*, TYPE_CONV*,double complex*, int); //3
void fwrite_pictures(short int*, short int *,int);
// Polynomial degree
//int d = 4;
double complex* roots;


int main_thrd_compute (void* args)
{
  
double start_real = -2.00000000000000;
double start_img  =  2.000000000000000;
const thrd_info_compute_t* thrd_info_compute = (thrd_info_compute_t*) args;
const int sz = thrd_info_compute->sz;
const double step = thrd_info_compute->step;
const int nmbr_thrds = thrd_info_compute->nmbr_thrds;
const int ib = thrd_info_compute->ib;
const int tx = thrd_info_compute->tx;
mtx_t *mtx = thrd_info_compute->mtx;
cnd_t *cnd = thrd_info_compute->cnd;
int_padded * status = thrd_info_compute->status;

//Andhika code variables here 
TYPE_CONV * element;
int d =thrd_info_compute->d;
double complex *  root_thrd_var = thrd_info_compute->root_thrd_var;




for (int img_c =ib; img_c < sz ; img_c = img_c + nmbr_thrds )
{
  TYPE_ATTR * attractor = (TYPE_ATTR * )malloc(sz*sizeof(TYPE_ATTR));
  TYPE_CONV * convergence = (TYPE_CONV * )malloc(sz*sizeof(TYPE_CONV)); //5
  attractor[0].img = start_img - step * (double) img_c;
  //double temp = attractor[0].img; // we need this value as imaginary component is replaced by the iteration when root function is called . Temp just stores the value of the imaginary compnent to be used in later iterations while calling root function.
  //attractor[img_c][0].real = start_real ;
	 for (int real_c = 0; real_c < sz ; ++real_c   )
 { 
 attractor [real_c].real = start_real + step * (double) real_c; 
  if (real_c !=0 )
  {  
 attractor[real_c].img = attractor[0].img;  //6
  }
  /*Andhika Code called here */
   element = root (&attractor[real_c],&convergence[real_c],root_thrd_var,d);  //7
   convergence[real_c].label = element->label;
   convergence[real_c].itr = element->itr;
 }


mtx_lock(mtx);

convergences[img_c] = convergence; 
status[tx].val = img_c + nmbr_thrds ;

/* We will print here which is not required in the assignment. Here Instead we will write the code which Andhika has written. We will call the functions here .*/
//printf ("%s%d %s %d %s %d %s %d %s %d %s %d %s %d %s"," !Thread: ",tx,"working : label  ", (int) attractors[img_c][0].real,", Iteration",(int) attractors[img_c][0].img, ", label ",(int) attractors[img_c][1].real, ", Iteration ", (int) attractors[img_c][1].img, ", Label ",(int)attractors[img_c][999].real, ", Iteration",(int)attractors[img_c][999].img, " \n");



mtx_unlock(mtx);
cnd_signal(cnd);

//thrd_sleep(&(struct timespec){.tv_sec=0, .tv_nsec=1000}, NULL);

	
}




return 0;

}



int main_thrd_write (void* args) /*This function is used by only 1 thread to write to a file the the roots of the polynomial equation as well as the iterations*/

{
 
const thrd_info_write_t * thrd_info_write= (thrd_info_write_t *) (args);
mtx_t* mtx = thrd_info_write->mtx;
cnd_t* cnd = thrd_info_write->cnd;
const int sz = thrd_info_write->sz;
int_padded *status = thrd_info_write->status;
const int nmbr_thrds = thrd_info_write->nmbr_thrds;
short int label [sz];
short int  itr [sz];
for (int img_c =0, trail ; img_c < sz ;  )
{

for (mtx_lock(mtx);;)
{


trail = sz;

 //We will wait for the compute to process a line and then only write that line in the final file.
for ( int x = 0; x < nmbr_thrds ; ++x )
{
	if ( trail > status[x].val )
            trail = status[x].val;
}			                
            if ( trail <= img_c )
 	      { cnd_wait(cnd,mtx);}
	
	     else {
	      mtx_unlock(mtx);
              break;
              }

} 




 for ( ; img_c < trail ;  ++ img_c) {
  


/* We will print here which is not required in the assignment. Here Instead we will write the
code which Chen has written. We will call the write functions here .*/
//for (int test = 0; test < sz ; test++ )
//{

//printf("%i ",(int) attractors[img_c][test].real );	
//printf ("%s%d %s %d %s %d %s %d %s %d %s %d %s %d %s"," !Writing Thread: ",1,"working : label  ",(int) attractors[img_c][0].real,", Iteration ",(int)attractors[img_c][0].img, ", label ",(int)attractors[img_c][1].real, ", Iteration",(int)attractors[img_c][1].img, ", label ",(int)attractors[img_c][999].real, ", Iteration",(int)attractors[img_c][999].img, " \n");
// we separate the values of the structure TYPE_CONV ,so that iterations and label can be written separately in fwrite function.

for (int sep = 0 ; sep < sz; sep ++ )
{
	
label[sep] =convergences[img_c][sep].label;
itr[sep] =convergences[img_c][sep].itr;

//printf("%s %d %s %d", "Label #### " , label[sep], "Iteration  ", itr[sep]);
//printf ("%s","\n");
}
 fwrite_pictures(label,itr/*convergences[img_c]*/,sz); //8
/*if (img_c == 0)

{	

for (int test =0 ; test < sz ; ++test)
{
printf("%i  ", (int)attractors [img_c][test].img);
}	
}*/
 free(attractors[img_c]); /*Now no need for the line which is already written in the file*/
 free(convergences[img_c]); //9
//}
//printf("%s", "\n");
 
}

}
return 0;


}


int main(int argc, char * argv[])
{
    
    
int size ;
int nmbr_thrds;
int d;
if(argc>1){
	    if (argv[1][0] == '-'&&argv[1][1] == 't'){
		    nmbr_thrds = atoi(argv[1]+2);}
	        if (argv[2][0] == '-'&&argv[2][1] == 'l'){
	            size = atoi(argv[2]+2);}
		     d = atoi(argv[3]);
}
//printf("%i %i %i\n",thread_n, line_n, size_n);
////from -2 ,2 two-dimensional array discrete with line_n points;
///

	
// Analytical root array (d+1 to accomodate root for divergence)
// Heap
//double complex *roots = (double complex*) malloc ((d+2) * sizeof(double complex));
// Stack

//int d = 5;
roots= (double complex *) malloc ((d+1)*sizeof(double complex));
double complex extra_root = 0.00000 + 0.00000 * I;
roots[d] = extra_root;



analytical_root(d, roots);

   
 //int nmbr_thrds = 1;
 // printf("%s", argv[2]);

thrd_t thrds[nmbr_thrds];
double step = ((double)4/(double) size);
//TYPE_ATTR output.real = -2.00000000000000;
//double sum;
//
//const int size = 1000;
 attractors =   (TYPE_ATTR **)   malloc(size  *   sizeof(TYPE_ATTR *));
 convergences = (TYPE_CONV **)   malloc(size   * sizeof(TYPE_CONV *)); //2
 thrd_t thrds_compute [nmbr_thrds];
 thrd_info_compute_t thrd_info_compute[nmbr_thrds];
 thrd_t thrd_write ;
 thrd_info_write_t thrd_info_write;
	
 mtx_t mtx;
 mtx_init(&mtx, mtx_plain);
 cnd_t cnd;
 cnd_init(&cnd);
 int_padded status[nmbr_thrds];


// Chen Code written here for making a file for the first time and initializing it .

FILE *file = fopen("output_colour.ppm","w");
if (file ==NULL){
    printf("error opening file.\n");
    return -1;
}
//int colour_type[10][3]= {{128,0,128},{128,0,0},{0,255,0},{0,0,255},{255,153,0},{192,192,192},{0,51,102},{255,255,0},{153,51,102},{255,102,0}};


fprintf(file,"P3\n");
fprintf(file,"%i %i\n",size,size);
fprintf(file,"255\n");
fclose(file);




FILE *file1 = fopen("output_grayscale.ppm","w");
if (file1 ==NULL){
    printf("error opening file.\n");
    return -1;
}
fprintf(file1,"P2\n");
fprintf(file1,"%i %i\n",size,size);
fprintf(file1,"50\n");
fclose(file1);

for (int i=0; i < nmbr_thrds; ++i)
{
 
	thrd_info_compute[i].ib = i;
	thrd_info_compute[i].status = status;
	status[i].val=-1;
	thrd_info_compute[i].nmbr_thrds=(nmbr_thrds) ;
	thrd_info_compute[i].mtx= &mtx;
	thrd_info_compute[i].cnd = &cnd;
	thrd_info_compute[i].tx= i;
        thrd_info_compute[i].step = step;
        thrd_info_compute[i].sz = size;

   // Andhika Code related variables 
    thrd_info_compute[i].d =d;
    thrd_info_compute[i].root_thrd_var=roots;

       int r = thrd_create(thrds_compute+i,main_thrd_compute, (void *) (thrd_info_compute+i));

//printf("%s","Hello");

     if ( r != thrd_success ) {
      fprintf(stderr, "failed to create thread\n");
      exit(1);
     }
     


   thrd_detach(thrds_compute[i]);
//printf("%s%d","Hello", i);
}
{

   thrd_info_write.mtx = &mtx;
   thrd_info_write.cnd = &cnd;
   thrd_info_write.nmbr_thrds=(nmbr_thrds); 
   thrd_info_write.sz = size;
   thrd_info_write.status = status;
    int r = thrd_create(&thrd_write, main_thrd_write, (void*) (&thrd_info_write));
        if ( r != thrd_success ) {
		      fprintf(stderr, "failed to create thread\n");
		            exit(1); }


//printf("%s%d","Hello");
}


 {
	     int r;
	     thrd_join(thrd_write, &r);
 
 }
free (attractors);
//free (convergences);
mtx_destroy(&mtx);
cnd_destroy(&cnd);

      return 0;

}




// ANALYTICAL_ROOT: this function receives order of the function d and return roots (xa + i ya)
void analytical_root (int d, double complex *roots)
{
// Variable declaration
double complex za_polar, za_euler;

for (int k = 0; k < d; ++k)
	{
	//za_polar = cos (2*k*PI/d) + (sin (2*k*PI/d))*I;
	za_euler = cexpf(2* I * M_PI * k / d);
	roots[k] = za_euler;
	//printf("analytical root z[%d] = %f +%fi\n\n", k+1, creal(roots[k]), cimag(roots[k]));
	}
}




// ROOT: this function receives an initial complex value (x + i y), absolute complex root (xa + i ya), order of the function (d),
// and return the root that the initial complex value converges to.
TYPE_CONV* root (TYPE_ATTR *input, TYPE_CONV * output,/*double complex zn*/ double complex *roots, int d) //4
{


//printf("%s %lf %s %lf", "Real Part:", input->real," Imaginary Part : ",input->img);
int max_iter = 51; // maybe make this global(?)
int iter = 0;
double sqtol = 0.000001; // maybe make this global(?)
int root_index; // to get the index of the root (or "label") which the initial complex number converges to
int brake = 0;


double complex zn = input->real + input->img * I;

for (int i = 0; i < max_iter; ++i)
	{
	//iter += 1;
	//double complex z_pow = 1.0; // This one is for power calculation using for loop, not needed
	double norm_sq = zn * conj(zn);

	// Check if zn too close to origin point or its real or imaginary component exceeds 1.0e+10
	if (norm_sq < sqtol || fabs(creal(zn)) > 1.0e+10 || fabs(cimag(zn)) > 1.0e+10)
		{
		root_index = d ;
		//printf ("Number of iterations: %d\n",  iter);
		break;
		}

	// Check to which root the initial complex number converges to, this for loop is gonna iterate
	// through the roots array
	for (int k = 0; k < d; ++k)
		{
		if ( ((creal(roots[k] - zn))*(creal(roots[k] - zn)) + (cimag(roots[k] - zn))*(cimag(roots[k] -zn)) ) < sqtol)
			{
			root_index = k;
			//printf ("No. of iteration: %d\n", iter);
			//printf ("zn converges to root z =  %f + %fi\n", creal(roots[root_index]), cimag(roots[root_index]));
			//printf ("zn = %f + %fi\n", creal(zn), cimag(zn));
			brake += 1;
			break;
			}
		}

	 if (brake == 1)
		          {
			break;
	}

/*
	// For loop to calculate the power of zn (stops at d-1 which is equal to the power of the
	// denumerator)
	for (int i = 0; i < d-1; ++i)
		{
		z_pow = z_pow * zn;
		}

	zn = zn - ((z_pow * zn) - 1)/(d * (z_pow));
*/

	// Newton's method for each degree (d)
	switch (d)
		{
		case 1:
			zn = 1;
			break;
		case 2:
			                    
			//zn = (zn*zn + 1) / (2*zn);
                         zn = (zn) / 2 + 1 / (2*zn);
			break;
		case 3:
			//zn = (2*zn*zn*zn + 1) / (3 * zn*zn);
                        zn = (2*zn) / 3 + 1 / (3 * zn*zn);
			break;
		case 4:
			//zn = (3*zn*zn*zn*zn + 1) / (4 * zn*zn*zn);
			  //zn = (3*zn*zn*zn*zn + 1) / (4 * zn*zn*zn);  //This always works
                          //zn = (3/4 * zn) + 1 / (4 * zn*zn*zn); //why doesn't this work??
                        zn = (3*zn) / 4 + 1 / (4 * zn*zn*zn);   //This guy works as well
                       
                         break;
		case 5:
		 	 //zn = (4*zn*zn*zn*zn*zn + 1) / (5 * zn*zn*zn*zn);
                          zn = (4*zn) / 5 + 1 / (5 * zn*zn*zn*zn);
                         break;

			break;
		case 6:
			//zn = (5*zn*zn*zn*zn*zn*zn + 1) / (6 * zn*zn*zn*zn*zn);
		         //zn = (5*zn*zn*zn*zn*zn*zn + 1) / (6 * zn*zn*zn*zn*zn);
                          zn = (5*zn) / 6 + 1 / (6 * zn*zn*zn*zn*zn);
	
                       break;
		case 7:
			//zn = (6*zn*zn*zn*zn*zn*zn*zn + 1) / (7 * zn*zn*zn*zn*zn*zn);
			//zn = (6*zn*zn*zn*zn*zn*zn*zn + 1) / (7 * zn*zn*zn*zn*zn*zn);
                        zn = (6*zn) / 7 + 1 / (7 * zn*zn*zn*zn*zn*zn);


                        break;
		case 8:
			//zn = (7*zn*zn*zn*zn*zn*zn*zn*zn + 1) / (8 * zn*zn*zn*zn*zn*zn*zn);
			  //zn = (7*zn*zn*zn*zn*zn*zn*zn*zn + 1) / (8 * zn*zn*zn*zn*zn*zn*zn);
                        zn = (7*zn) / 8 + 1 / (8 * zn*zn*zn*zn*zn*zn*zn);


                        break;
		case 9:
			//zn = (8*zn*zn*zn*zn*zn*zn*zn*zn*zn + 1) / (9 * zn*zn*zn*zn*zn*zn*zn*zn);
			
                          //zn = (8*zn*zn*zn*zn*zn*zn*zn*zn*zn + 1) / (9 * zn*zn*zn*zn*zn*zn*zn*zn);
                         zn = (8*zn) / 9 + 1 / (9 * zn*zn*zn*zn*zn*zn*zn*zn);


                        break;
		case 10:
		        //zn = (9*zn*zn*zn*zn*zn*zn*zn*zn*zn*zn + 1) / (10 * zn*zn*zn*zn*zn*zn*zn*zn*zn);
                        zn = (9*zn) / 10 + 1 / (10 * zn*zn*zn*zn*zn*zn*zn*zn*zn);

	

                      //zn = (9*zn*zn*zn*zn*zn*zn*zn*zn*zn*zn + 1) / (10 * zn*zn*zn*zn*zn*zn*zn*zn*zn);
			break;
		}


	iter += 1;

	if (iter == max_iter)

  {
     root_index=d ;
  break;
  
  }



	}



output->label = /*0.00000 + (double)*/ root_index;
output->itr = /*0.00000+ (double)*/ iter;

//printf ("Index of the root: %lf\n", input->real/*root_index*/);
//printf ("zn = %f + %fi\n", creal(zn), cimag(zn));
//printf ("iteration: %lf\n", input->img /*iter*/);

//printf ("Index of the root: %d\n", root_index);
//printf ("zn = %f + %fi\n", creal(zn), cimag(zn));
//printf ("iteration: %d\n", iter);

//printf("%d %s %d %s",(int)input->real," ######## ", (int) input->img,"\n" );
return output;
}


void fwrite_pictures( short int * label, short int *itr , int n){




//for (int test =0 ; test < 1000 ; ++test)
//{
//printf("%i  ", (int)attractor[test].img);
//}



	/*for (int sep = 0 ; sep < n; sep ++ )
	{

		printf("%s %d %s %d", "Label #### " , label[sep], "Iteration  ", itr[sep]);
		printf ("%s","\n");
	}*/

///////////////////////////////////////fwrite with colour///////////////////////////////////
FILE *file = fopen("output_colour.ppm","a");
if (file ==NULL){
    printf("error opening file.\n");
    //return -1;
}
int colour_type[10][3]= {{128,0,128},{128,0,0},{0,255,0},{0,0,255},{255,153,0},{192,192,192},{0,51,102},{255,255,0},{153,51,102},{255,102,0}};


//fprintf(file,"P3\n");
//fprintf(file,"%i %i\n",n,n);
//fprintf(file,"255\n");
//int colour_pixel = 255;
//for (int ix = 0;ix<n;++ix){
//for (int iy = 0;iy<n;++iy){

//fprintf(file,"%i %i %i ",colour_type[convergence[ix].label][0],colour_type[convergence[ix].label][1],colour_type[convergence[ix].label ][2]);
//}
//fprintf(file,"\n");
//}
fclose(file);
//////////////////////////////////////fwrite with grayscale////////////////////////////////
FILE *file1 = fopen("output_grayscale.ppm","a");
if (file1 ==NULL){
    printf("error opening file.\n");
   // return 0;
}
//fprintf(file1,"P2\n");
//fprintf(file1,"%i %i\n",n,n);
//fprintf(file1,"50\n");

//for (int test =0 ; test < n ; ++test)
//{
//	printf("%i  ", (int)attractor[test].img);
//}

//for (int ix = 0;ix<n;++ix){
//for (int iy = 0;iy<n;++iy){
//fprintf(file1,"%i ",convergence[ix].itr);
fwrite(itr,n,sizeof(short int),file1);
//}
//fprintf(file1,"\n");
//}
fclose(file1);








}





