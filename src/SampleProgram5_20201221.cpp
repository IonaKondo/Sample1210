#include<stdio.h>                                                           
#include<math.h>
#include<stdlib.h> // malloc

#include "include.h"
#include "boolean.h"
#include "options.h"
#include "quicksort.h"
#include "fitsimage.h"
#include "sstring.h"


#define B 2                       //Nummber of shift bits //Noise bits to save
#define C 5                       //Cutoff/Gain //Glitch Cutoff/Gain
#define E 2250                    //2*C*readvar/Gain //2C*V**2/G
#define F 32000                   //Full Well //Full-well (saturation)cutoff
#define G 1                       //Gain e/count //Detector Gain
#define N 25                 //Num Dat 1024**2 //Number of consecutive good intervals (current “winning streak”)
//#define N 524288                 //Num Dat 128*4096 for an output
#define I 0x1000000               //For inc of n,m (1000000000000000000000000 or 16777216)
#define L 0xFFFFFF                //Low 24 bits (111111111111111111111111 or 16777215)
#define W 0x1000001               //Init weight (1000000000000000000000001 or 16777217)
#define A (z&0xFFFF0000)          //Mask for A (z&11111111111111110000000000000000)
#define Z (z&0xFFFF)              //Mask for Z (z&1111111111111111)
#define LOOP for(P=M,D=*ds++;P<Q;P++)           //Standard loop
struct {long w,s,t,z;} M[N], *P, *Q;            //External Memory
//int **ds; int *D;                         //Input Data
double **ds; double *D;                         //Input Data
//void Integrate(int n,int *S,int *V) ;          //Samples, Out, Variance
void Integrate(int n,double *S,double *V) ;          //Samples, Out, Variance

int
main(int argc, char* argv[])
{
   // Load program parameters from commnd line and parameter file
  OptionSet parameter(argc, argv);
  FitsImage work;  //fitssubtract.cpp
  int nramp = 3;
  //int *S,*V, *S0, *V0; //S and V are int pointers to estimated signal and variance array.
  double *S,*V, *S0, *V0; //S and V are int pointers to estimated signal and variance array.

  //ALlocate memory for ds
  //int **ds0 = (int**)malloc(sizeof(int*)*nramp);  //number of samples
  double **ds0 = (double**)malloc(sizeof(double*)*nramp);  //number of samples
 
  for(int p=0;p<nramp;p++){
    //ds0[p] = (int*)malloc(sizeof(int)*N);  //number of pixels
    ds0[p] = (double*)malloc(sizeof(double)*N);  //number of pixels
  }

  //ALlocate memory for S and V
  //S0 = (int*)malloc(sizeof(int)*N);  //number of pixels per sample/frame
  //V0 = (int*)malloc(sizeof(int)*N);  //number of pixels per sample/frame
  S0 = (double*)malloc(sizeof(double)*N);  //number of pixels per sample/frame
  V0 = (double*)malloc(sizeof(double)*N);  //number of pixels per sample/frame
  
  //Sample for loop
  for(int n=1; n<=nramp;n++){
    int ndata =0;
    String filename = parameter.getOption("frame", n, "");
    work.read(filename);
    cout << "Up-the-ramp input file" << n << ": "  << filename << endl;

    //Fill ds array with data
    for (int i = 0; i < 5; ++i){
      for (int j = 0; j < 5; ++j){
	      ds0[n-1][ndata] = work.value(i, j);
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
  cout << "Up-the-ramp output file: " << outfile << endl;
  FitsImage out;
  out.resize(5, 5);
  for (int i = 0; i<5; ++i)  
   for (int j = 0; j <5; ++j) {
     //Shift output S right by number of noise bits, B
     //out.assign(i, j, *S++>>B);
     out.assign(i, j, *S++/4);
  }
  out.create(outfile); 
  out.write(); 
  out.close();

  free(S0);
  free(V0);
  for(int i=0; i< nramp; ++i){
    free(ds0[i]);
  }
  free(ds0);

}

//void Integrate(int n,int *S,int *V){            //Samples, Out, Variance
void Integrate(int n,double *S,double *V){            //Samples, Out, Variance
//int e,m,r,t,x,z; Q=M+N;                         //Registers
double e,m,r,t,x,z; Q=M+N;                         //Registers
LOOP{P->t=P->z=*D++; P->s=P->w=0;}               //1st Sample
while(--n)LOOP{if((r=*D++)<F){z=P->z;           //Next sample, not full
    //e=(x=(r-Z))-(t=P->s/((m=P->w)?m&L:1));       //Calc err
    e=(x=(r-z))-(t=P->s/((m=P->w)?m:1));       //Calc err
    //if(e*e<C*t+E){m=((t=P->t+I)>>24);           //Good? Update n
    if(e*e<C*t+E){m=((t=P->t+1.0));           //Good? Update n
        //P->w+=I+(m*(m+1)>>1); P->s+=m*r-(t&L);} //Update m,W,S
        P->w+=1.0+(m*(m+1.0)/2.0); P->s+=m*r-(t);} //Update m,W,S
    else{if(m<=1.0){P->w=1.0; P->s=x; t=1.0 +0.0; z=0.0;}  //Begin again
        else x=t=0;}} else x=t=0;               //Bad or full, restart Int
    //P->t=t+r; P->z=A+(x<<16)+r;}                //Finish Update n,T,A,Z
    P->t=t+r; P->z=z+(x)+r;}                //Finish Update n,T,A,Z
//LOOP{t=P->w?P->w:W; m=t>>24; t&=L; z=P->z>>16;  //unpack
LOOP{t=P->w?P->w:1.0; m=t; t=t; z=P->z;  //unpack
    //*S++=x=P->s*((E*4+z)<<B)/(m*P->s+t*E*4);    //Optimum estimate
    *S++=x=P->s*((E*4+z)*4)/(m*P->s+t*E*4);    //Optimum estimate
    //*V++=(E<<2*B)/(t*G)+(x<<B)/(m*G);}}         //Variance estimate
    *V++=(E*4*B)/(t*G)+(x*4)/(m*G);}}         //Variance estimate

