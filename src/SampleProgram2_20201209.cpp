//#comment
//#I have some modifications.
//#1. (line 29) change LOOP defenition because the adress '*ds++' become null in LOOP2. I do not the reason... 
//#2. (line 76,77) modify how to define the parameters '*D' and '**ds' (line)  However I am not sure if the defenition 'ds=&D' 

#include<stdio.h>                                                           
#include<math.h>

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
#define LOOP for(P=M,D;P<Q;P++)           //Standard loop
//#define LOOP for(P=M,D=*ds++;P<Q;P++)           //Standard loop
struct {long w,s,t,z;} M[N], *P, *Q;            //External Memory
int **ds; int *D;                         //Input Data
void load_image(FitsImage& image, String& filename);
void Integrate(int n,int *S,int *V) ;          //Samples, Out, Variance

int
main(int argc, char* argv[])
{
  //void Integrate(int n,int *S,int *V) ;          //Samples, Out, Variance

   // Load program parameters from commnd line and parameter file
  OptionSet parameter(argc, argv);
  String filename1 = parameter.getOption("frame", 1, "");
  String filename2 = parameter.getOption("frame", 2, "");
  String filename3 = parameter.getOption("frame", 3, "");

  //FitsImage *work= new FitsImage[];  //fitscombine.cpp
  FitsImage work;  //fitssubtract.cpp
  FitsImage work1;  //fitssubtract.cpp
  int width, height;                                                                                                                                        
  //load_image(work, filename,&width,&height);
  load_image(work1, filename2);
  work1.size(width, height);


  int nPixels;
  nPixels = work1.num_pixels();
  printf ("flag1 width= %d, height= %d, nPixels=%d\n",width, height,nPixels);

  int npix = 3*nPixels;
  //int ndata =0;
  //int value;
  //int *value = new int[nPixels];
  //int *value = new int[npix];
  int *value = new int[75];
  int nramp = 3;
  int S,V;
  int *DS;

  int ndata =0;


  for(int n=1; n<=nramp;n++){
   printf ("\n\n---Start ramp fitting using %d samples--------------------\n",n);
 
   D = &value[0];
   ds = &D;

   printf ("Seeing fits file no.%d\n",n);
   if (n==1) load_image(work, filename1);
   if (n==2) load_image(work, filename2);
   if (n==3) load_image(work, filename3);
  work.size(width, height);
  nPixels = work.num_pixels();
  printf ("flag2 width= %d, height= %d, nPixels=%d\n\n",width, height,nPixels);

    for (int i = 0; i < width; ++i){
    for (int j = 0; j < height; ++j){
    value[ndata] = work.value(i, j);
     printf ("flag2 *D=value[%d]= %d \n",ndata,value[ndata]);
     //printf ("flag2 file %d, value= %d\n",nwork,value[ndata]);
    ++ndata;
    }}
   printf("ndata= %d\n",ndata);
   printf ("flag3 where is the adress: D_adress= %p ds_address=%p \n",D,*ds);

    Integrate(n, &S, &V);
   printf ("n= %d\n",n);
  printf("1 S= %d V= %d \n",S,V);
  printf("2 S= %d V= %d \n",&S,&V);
    DS = &S; 
  printf("DS= %d \n",DS++);
   printf ("---End ramp fitting using %d samples-------------------\n\n\n",n);
 }

  //}
  //printf("S= %d V= %d \n",S,V);

  // Output image
//  FitsImage out;
//  out.resize(width, height);    
  //ImType correct ;
  int ndataO =0;

 printf("OUT width= %d, height= %d\n",width,height);
//  for (int i = 0; i < width; ++i){
//  for (int j = 0; j < height; ++j) {
   //out.assign(i, j, *S++);
   //printf ("(i,j)= (%d,%d), OUT= %d\n",i,j,*S++);
   //printf ("(i,j)= (%d,%d), OUT= %d\n",i,j,DS++);
   //printf ("(i,j)= (%d,%d), OUT= %d\n",i,j,*DS++);
//   ++ndataO; 
//  }}

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


 printf ("1st sample begin\n");
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
printf ("1st sample end\n\n");



//<<LOOP 2>> //Next sample
printf ("Next sample begin\n");
printf ("Next where is the adress: D_adress= %p ds_address=%p \n",D,*ds);
int test4=0;
while(--n)LOOP{
   printf ("NextIn where is the adress: D_adress= %p ds_address=%p \n",D,*ds);
   printf ("LOOP2 n= %d, test4= %d, M= %d, P= %d, Q= %d\n",n,test4, M,P,Q);    
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
printf ("Next sample end\n\n");


//<<LOOP 3>> //unpack
printf ("Make Output begin\n");
int test=0;
LOOP{
   printf ("LOOP3 test2= %d, M= %d,P= %d Q= %d\n",test, M, P,Q);    
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
printf ("Make Output end\n\n");


}


