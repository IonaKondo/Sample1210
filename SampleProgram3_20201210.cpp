#comment                                                                                                                                              
#I have some modifications from version2.
#1. (line 32) undo the change of the defenition of 'LOOP'
#2. (line 55-58,) define the 2D array for '**ds' 
#(line 63 & 66-70 )I do not know how to initialize the adress position '*ds' increased by the LOOP in the Integral function.

#include<stdio.h>                                                           
#include<math.h>
#include <stdlib.h> // malloc

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
#define G 2                       //Gain e/count //Detector Gain
//#define N 1048576                 //Num Dat 1024**2 //Number of consecutive good intervals (current “winning streak”)
#define N 25                 //Num Dat 1024**2 //Number of consecutive good intervals (current “winning streak”)
//#define N 524288                 //Num Dat 128*4096 for an output
#define I 0x1000000               //For inc of n,m
#define L 0xFFFFFF                //Low 24 bits
#define W 0x1000001               //Init weight
#define A (z&0xFFFF0000)          //Mask for A
#define Z (z&0xFFFF)              //Mask for Z
#define LOOP for(P=M,D=*ds++;P<Q;P++)           //Standard loop
struct {long w,s,t,z;} M[N], *P, *Q;            //External Memory
//extern int**ds; int *D;                         //Input Data
int **ds; int *D;                         //Input Data
void load_image(FitsImage& image, String& filename);
void Integrate(int n,int *S,int *V) ;          //Samples, Out, Variance

//main()
int
main(int argc, char* argv[])
{
   // Load program parameters from commnd line and parameter file
  OptionSet parameter(argc, argv);
  String filename1 = parameter.getOption("frame", 1, "");
  String filename2 = parameter.getOption("frame", 2, "");
  String filename3 = parameter.getOption("frame", 3, "");

  FitsImage work;  //fitssubtract.cpp
  int nramp = 3;
  int S,V;
  int *DS;


   ds = (int**)malloc(sizeof(int*)*3);  //number of samples
   for(int p=0;p<3;p++){
    ds[p] = (int*)malloc(sizeof(int)*N);  //number of pixels
   }

  for(int n=1; n<=nramp;n++){
   printf ("\n\n---Start ramp fitting using %d samples--------------------\n",n);
    D= ds[0];
    *ds= ds[0];             //I do not know how to initialize the adress position of '*ds'
   int ndata =0;
   
   if(n==3) {               //I do not know how to initialize the adress position of '*ds'
     *ds--;
     *ds--;
     *ds--;
   }

   printf ("Seeing fits file no.%d\n",n);
   if (n==1) load_image(work, filename1);
   if (n==2) load_image(work, filename2);
   if (n==3) load_image(work, filename3);



    for (int i = 0; i < 5; ++i){
    for (int j = 0; j < 5; ++j){
    ds[n-1][ndata] = work.value(i, j);
     printf ("flag0 ds[%d][%d]= %d \n",n-1,ndata,ds[n-1][ndata]);
    ++ndata;
    }}
    printf("ndata= %d\n",ndata);


    printf ("Before Integral: where is the adress: D_adress= %p ds_address=%p \n",D,*ds);
    //printf ("flag1 where is the adress: D_adress= %p ds_address=%p \n",D,*ds++);
    //printf ("flag1 where is the adress: D_adress= %p ds_address=%p \n",D,*ds);

    if(n==1) continue;  // do ramp fitting when the number of samples is more than two samples.
    

    Integrate(n, &S, &V);
    DS = &S;
    printf("1 S= %d V= %d \n",S,V);
    printf("DS= %d \n",*DS);  
    printf ("After Integral1: where is the adress: D_adress= %p ds_address=%p \n",D,*ds);
  
    printf ("---End ramp fitting using %d samples-------------------\n\n\n",n);
 }


for(int pp=0;pp<3;pp++){
  free(ds[pp]);
}
free(ds); 



  return 0;
}

void load_image(FitsImage& image, String& filename)
  /* Simple wrapper to read an FITS image of given filename into the
     instance of the FITS image class. */
{
  if (image.read(filename)) {
    int width, height;
    image.size(width, height);
    cout << filename << " " << width << " " << height << endl;
  }
  else {
    cout << "Couldn't load image from file ";
    cout << filename << endl;
    cout << " ... crashing out" << endl;
    exit(2);
  }
}

void Integrate(int n,int *S,int *V){            //Samples, Out, Variance
int e,m,r,t,x,z; Q=M+N;                         //Registers
//<<LOOP 1>> //first sample

 // for(int ndatat=0;ndatat<25;ndatat++ ){
 //    printf ("flag3 address D=%p, *ds= %p \n",D,*ds);
 // D++;
 // }


 printf ("\n\nLOOP1 1st sample begin\n");
 //printf ("1st where is the adress: D_adress= %p ds_address=%p \n",D,*ds++);
 printf ("1st where is the adress: D_adress= %p ds_address=%p \n",D,*ds);
int test1=0;
LOOP{
   //printf ("LOOP1 test1= %d, M= %d, P= %d, Q= %d\n",test1, M,P,Q);    
   //printf ("*D= %d *D++= %d\n", *D,*D++);
   P->t=P->z=*D++; 
   P->s=P->w=0;
   test1++;
 printf ("LOOP1 where is the adress: D_adress= %p ds_address=%p \n",D,*ds);
}               //1st Sample
printf ("LOOP1 1st sample end\n\n");



//<<LOOP 2>> //Next sample
printf ("LOOP2 Next sample begin\n");
printf ("Next where is the adress: D_adress= %p ds_address=%p \n",D,*ds);
int test4=0;
while(--n)LOOP{
   printf ("NextIn where is the adress: D_adress= %p ds_address=%p \n",D,*ds);
   //printf ("LOOP2 n= %d, test4= %d, M= %d, P= %d, Q= %d\n",n,test4, M,P,Q);    
    //if((r=*D++)<F){z=P->z;           //Next sample, not full
 // printf ("*D= %d\n", *D);
  //printf ("*D++= %d\n", *D++);
    if((r=*D++)<F){
    z=P->z;           //Next sample, not full
    e=(x=(r-Z))-(t=P->s/((m=P->w)?m&L:1));       //Calc err
    if(e*e<C*t+E){m=((t=P->t+I)>>24);           //Good? Update n
        P->w+=I+(m*(m+1)>>1); P->s+=m*r-(t&L);} //Update m,W,S
    else{if(m<=W){P->w=W; P->s=x; t=I+Z; z=0;}  //Begin again
        else x=t=0;}} else x=t=0;               //Bad or full, restart Int
    //P->t=t+r; P->z=A+(x<<16)+r;}                //Finish Update n,T,A,Z
    P->t=t+r; P->z=A+(x<<16)+r;
    test4++;
    }                //Finish Update n,T,A,Z
printf ("LOOP2 Next sample end\n\n");


//<<LOOP 3>> //unpack
printf ("LOOP3 Make Output begin\n");
int test=0;
LOOP{
   //printf ("LOOP3 test2= %d, M= %d,P= %d Q= %d\n",test, M, P,Q);    
    t=P->w?P->w:W; m=t>>24; t&=L; z=P->z>>16;  //unpack
    *S++=x=P->s*((E*4+z)<<B)/(m*P->s+t*E*4);    //Optimum estimate
 // printf ("Integrate output S= %d\n",*S);
 // printf ("Integrate  t= %d m= %d\n",t,m);
    //*V++=(E<<2*B)/(t*G)+(x<<B)/(m*G);}}         //Variance estimate
    *V++=(E<<2*B)/(t*G)+(x<<B)/(m*G);        //Variance estimate
  //printf ("Integrate output V= %d\n", *V);
 // printf ("Integrate flag3 test= %d\n", test);
   test++;
}
printf ("LOOP3 Make Output end\n\n");


}


