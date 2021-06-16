#include<stdio.h>                                                           
#include<math.h>
#include<stdlib.h> // malloc
#include<time.h>
#include <sys/time.h>

#include "include.h"
#include "boolean.h"
#include "options.h"
#include "quicksort.h"
#include "fitsimage.h"
#include "sstring.h"


//#define B 2                       //Nummber of shift bits //Noise bits to save
#define C 5                       //Cutoff/Gain //Glitch Cutoff/Gain
#define E 2250                    //2*C*readvar/Gain //2C*V**2/G
#define F 32000                   //Full Well //Full-well (saturation)cutoff
#define G 1                       //Gain e/count //Detector Gain
//#define N 25                 //Num Dat 1024**2 //Number of consecutive good intervals (current “winning streak”)
#define N 524288                 //Num Dat 128*4096 for an output
//#define I 0x1000000               //For inc of n,m
//#define L 0xFFFFFF                //Low 24 bits
//#define W 0x1000001               //Init weight
//#define A (z&0xFFFF0000)          //Mask for A
//#define Z (z&0xFFFF)              //Mask for Z
#define LOOP for(P=M,D=*ds++;P<Q;P++)           //Standard loop
//struct {long w,s,t,z;} M[N], *P, *Q;            //External Memory
//struct {float w,s,t,z;} M[N], *P, *Q;            //External Memory
//struct {float w,s,t,z;} M[N], *P, *Q;            //External Memory
struct {float w,s,t,z,a; int n,m;} M[N], *P, *Q;            //External Memory
//int **ds; int *D;                         //Input Data
float **ds; float *D;                         //Input Data
//double **ds; double *D;                         //Input Data
//void Integrate(int n,int *S,int *V) ;          //Samples, Out, Variance
void Integrate(int n,float *S,float *V) ;          //Samples, Out, Variance
//void Integrate(int n,double *S,double *V) ;          //Samples, Out, Variance


long getCurrentTime() {
      struct timeval tv;
      gettimeofday(&tv,NULL);
      return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


int
main(int argc, char* argv[])
{
  clock_t start = clock();
  struct timeval tv;
  //int ret = gettimeofday(&tv, NULL);
  cout<<"milliseconds: "<<getCurrentTime()<<endl;

   // Load program parameters from commnd line and parameter file
  OptionSet parameter(argc, argv);
  //String subname = parameter.getOption("subname", 1, "");  

  FitsImage work;  //fitssubtract.cpp
  int nramp = 3;
  //int *S,*V, *S0, *V0; //S and V are int pointers to estimated signal and variance array.
  float *S,*V, *S0, *V0; //S and V are int pointers to estimated signal and variance array.
  //double *S,*V, *S0, *V0; //S and V are int pointers to estimated signal and variance array.

  //ALlocate memory for ds
  //int **ds0 = (int**)malloc(sizeof(int*)*nramp);  //number of samples
  float **ds0 = (float**)malloc(sizeof(float*)*nramp);  //number of samples
  //double **ds0 = (double**)malloc(sizeof(double*)*nramp);  //number of samples
 
  for(int p=0;p<nramp;p++){
    //ds0[p] = (int*)malloc(sizeof(int)*N);  //number of pixels
    ds0[p] = (float*)malloc(sizeof(float)*N);  //number of pixels
  //  ds0[p] = (double*)malloc(sizeof(double)*N);  //number of pixels
  }

  //ALlocate memory for S and V
  //S0 = (int*)malloc(sizeof(int)*N);  //number of pixels per sample/frame
  //V0 = (int*)malloc(sizeof(int)*N);  //number of pixels per sample/frame
  S0 = (float*)malloc(sizeof(float)*N);  //number of pixels per sample/frame
  V0 = (float*)malloc(sizeof(float)*N);  //number of pixels per sample/frame
  //S0 = (double*)malloc(sizeof(double)*N);  //number of pixels per sample/frame
  //V0 = (double*)malloc(sizeof(double)*N);  //number of pixels per sample/frame
 

//for(int nsub= 0; nsub<32;nsub++){//end of subframe for loop   
  //Sample for loop
  for(int n=1; n<=nramp;n++){
    int ndata =0;

    //String filename = subname + num_to_str(n) + "-" + num_to_str(nsub) + ".fit.ref";
    String filename = parameter.getOption("frame", n, "");
    work.read(filename);
    cout << "Up-the-ramp input file" << n << ": "  << filename << endl;

    //Fill ds array with data
    for (int i = 0; i < 128; ++i){
      for (int j = 0; j < 4096; ++j){
	      ds0[n-1][ndata] = work.value(i, j)+8000;
	      if (i==127 && j==4092) {printf("(i,j)= (%d, %d)= %f\n",i,j,ds0[n-1][ndata]);}
	      if (i==127 && j==4093) {printf("(i,j)= (%d, %d)= %f\n",i,j,ds0[n-1][ndata]);}
	      if (i==127 && j==4094) {printf("(i,j)= (%d, %d)= %f\n",i,j,ds0[n-1][ndata]);}
	      if (i==127 && j==4095) {printf("(i,j)= (%d, %d)= %f\n",i,j,ds0[n-1][ndata]);}
	      //ds0[n-1][ndata] = work.value(i, j);
        //if(ds0[n-1][ndata] <0){ printf("input (i,j)= %d\n",ds0[n-1][ndata]);} 
	      ++ndata;
      }
    }
    work.close();
  }//end of sample for loop


  //Create copies of pointers to increment in integrate function
  ds = ds0;
  S = S0;
  V = V0;

  //Integrate runs on all data in **ds and outputs results to S an V
  Integrate(nramp, S, V); //S and V are already type int*

  //make output fits file
  String outfile  = parameter.getOption("out", 1, "Ramp_output.fit");
  //String outname  = parameter.getOption("outname", 1, "Ramp_output.fit"); 
  //String outfile = outname + "-" + num_to_str(nsub) + ".fit.ramp";
  cout << "Up-the-ramp output file: " << outfile << endl;
  FitsImage out;
  out.resize(128, 4096);
  for (int i = 0; i<128; ++i)  
   for (int j = 0; j <4096; ++j) {
	   if (i==127 && j==4092) {printf("(i,j)= (%d, %d), *S= %f, *V= %f\n",i,j,*S,*V);}
	   if (i==127 && j==4093) {printf("(i,j)= (%d, %d), *S= %f, *V= %f\n",i,j,*S,*V);}
	   if (i==127 && j==4094) {printf("(i,j)= (%d, %d), *S= %f, *V= %f\n",i,j,*S,*V);}
	   if (i==127 && j==4095) {printf("(i,j)= (%d, %d), *S= %f, *V= %f\n",i,j,*S,*V);}
     //printf("(i,j)= (%d, %d), *S= %d, *V= %d\n",i,j,*S,*V);
     //Shift output S right by number of noise bits, B
     //out.assign(i, j, *S++>>B);
     out.assign(i, j, *S++/4.0);
     *V++;
  }
  out.create(outfile); 
  out.write(); 
  out.close();
//}//end of subframe for loop 


  clock_t end = clock();
  const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
  printf("time %lf[ms]\n", time);

  cout<<"milliseconds: "<<getCurrentTime()<<endl;
  free(S0);
  free(V0);
  for(int i=0; i< nramp; ++i){
    free(ds0[i]);
  }
  free(ds0);

}

void Integrate(int nramp,float *S,float *V){            //Samples, Out, Variance
int m,n;                          //Registers
float e,x,y,w,s,t,z,a,r; Q=M+N;                         //Registers
LOOP{P->t=P->z=*D++; P->s=P->w=P->a=0;P->m=P->n=0;}               //1st Sample
int test = 0;
while(--nramp)LOOP{if((r=*D++)<F){z=P->z;           //Next sample, not full
    x=(r-z);       //Calc err
    y=P->s/((w=P->w)?P->w:1);       //Calc err
    e=x-y;       //Calc err
    n=P->n;
    m=P->m;
    //printf("test %d X= %f Y= %f Q2= %f Comp= %f \n", test,x,y, e*e, (C*y+E));
    //printf("test %d s= %f w= %f \n", test,P->s,w);
    if(e*e<C*y+E){
        n+=1;                 //Update n
        m+=1;                 //Update m
        P->a+=r-z;         //Good? Update a
        P->w+=n*(n+1)/2.0 ;   //Update W
        P->s+=n*r-(t=P->t);   //Update S
        //n+=1;                 //Update n
        //m+=1;                 //Update m
        //n+=1;                 //Update n
        //m+=1;                 //Update m
        z=r;                  //Update z
        t=t+r; }              //Update t
    //    printf("Happy path %d m= %d n= %d z= %f t= %f \n",test,P->m,P->n,z,t );}
    else{if(m<=1){P->w=1; P->s=x; m=1; n=1;P->a=x; t=r+z; z=r;}  //Begin again
        else{n=0;z=r;t=r;}}} else{n=0;z=r;t=r;}               //Bad or full, restart Int
    P->t=t; P->z=z ;P->n=n;P->m=m;
    //P->t=t; P->z=z ;
    //printf("test %d, t= %f, m= %d, n= %d, z= %f, x= %f, P->s= %f,w= %f \n" ,test,t,P->m,P->n,z,x,P->s,P->w);
      ++test;
    }                //Finish Update n,T,A,Z
int test1=0;
LOOP{w=P->w?P->w:1; //m=t>>24; t&=L; z=P->z>>16;  //unpack
    *S++=x=P->s*(4*E+P->a)/(P->m*P->s+4*w*E);    //Optimum estimate
    //*V++=E/(t*G)+x/(P->m*G);}}         //Variance estimate
    printf("test1 %d P->t= %f w= %f x= %f P->m= %d E= %d G= %d\n", test1,P->t,w,x,P->m,E,G);
    ++test1;
    //*V++=E/(P->t*G)+x/(P->m*G);}}         //Variance estimate
    *V++=E/(w*G)+x/(P->m*G);}}         //Variance estimate





