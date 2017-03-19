#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "BigNum/BigNum.h"
#define SIZE ((32))


/*
static long gcdl(long a, long b) {
  long r;
  while(b!=0){
    r = a % b;
    a = b;
    b = r;
		printf("**************************\n");
		printf("N:\n");
		printf("%ld \n", a);    
		printf("M:\n");
		printf("%ld \n", b);    
    getchar(); 
  }
  return a;
}*/


void gcd(unsigned int * A, unsigned int * B){
	//unsigned int * R = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
	unsigned int R[SIZE];
	while(!zeroNum(B)){
		/*printf("**************************\n");
		printf("N:\n");
		printNum(N);
		printf("M:\n");
		printNum(M);
		printf("++++++++++++++++++++++++++\n");*/
		copyNum(R, A);
		modNum(R, B);
		copyNum(A, B);
		copyNum(B, R);
		
		/*printf("**************************\n");
		printf("nm_r: \n");
		printNum(nm_r);
		printf("++++++++++++++++++++++++++\n");*/
		//getchar(); 
	}
	//free(R);
}

void fxfun(unsigned int * N, unsigned int * X, unsigned int * C){
	//setZero(Y);
	powNum(X);
	modNum(X, N);
	addNum(X, C);
	modNum(X, N);
	//copyNum(Y, X);
}


void PollardRho(unsigned int * N){
	/*
The idea I've got in mind is to prepare multiple starting points
for X, Y and C
and each thread can actually count Y in place without bothering CPU
Therefore reasonable aproach would be to just prepare huge chunk of memory
for GPU to work with.
	*/
	if (isEven(N)){
		zeroNum(N);
		N[0] = 2;
		return;
	}
// run some tests on CUDA device
    int num;
    if (cudaGetDeviceCount(&num)){
    	printf("Cannot get number of CUDA devices\n");
    	return;
	}
	if (num < 1){
		printf("No CUDA devices found\n");
    	return;
	}
    cudaDeviceProp prop;
    int   MaxDevice = -1;
    int   MaxGflops = -1;
    for (int dev=0;dev<num;dev++)
    {
        if (cudaGetDeviceProperties(&prop,dev)){
        	printf("Error getting device %d properties\n",dev);
        	return;	
    	}
    	int Gflops = prop.multiProcessorCount * prop.clockRate;
        if (verbose){ 
        	printf("CUDA Device %d: %s Gflops %f Processors %d Threads/Block %d\n",dev,prop.name,1e-6*Gflops,prop.multiProcessorCount,prop.maxThreadsPerBlock);
        }
        if(Gflops > MaxGflops)
        {
            MaxGflops = Gflops;
            MaxDevice = dev;
        }
    }
    
    //  Print and set device
    if (cudaGetDeviceProperties(&prop,MaxDevice)){
    	printf("Error getting device %d properties\n", MaxDevice);
    	return;
    }
    printf("Fastest CUDA Device %d: %s\n",MaxDevice,prop.name);
    cudaSetDevice(MaxDevice);
    //  Return max thread count
	printf("maxThreadsPerBlock %d \n", prop.maxThreadsPerBlock);
	return;

	unsigned int j = 0;
	//unsigned int * X = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
	unsigned int X[SIZE];
	//unsigned int * Y = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
	unsigned int Y[SIZE];
	//unsigned int * C = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
	unsigned int C[SIZE];
	//unsigned int * G = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
	unsigned int G[SIZE];
	//unsigned int * N_tmp = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
	unsigned int N_tmp[SIZE];
	//unsigned int * abs_mxy = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
	unsigned int abs_mxy[SIZE];
	
	setZero(X);
	X[0] = 7;
	setZero(C);
	C[0] = 1;
	setZero(G);
	G[0] = 1;
	copyNum(Y, X);
	fxfun(N, Y, C);
	//printf();
	while (isOne(G)){
		/*printf("Tady to padlo\n");
		printNum(N);
		printNum(x);
		printNum(c);*/
		fxfun(N, X, C);
		fxfun(N, Y, C);
		fxfun(N, Y, C);
		/*printf("B========================\n");
		printf("X\n");
		printNum(X);
		printf("Y\n");
		printNum(Y);
		printf("N\n");
		printNum(N);
		printf("B========================\n"); */
		if(bigger(X, Y) == 1){
			copyNum(abs_mxy, X);
			subNum(abs_mxy, Y);
		}else{
			copyNum(abs_mxy, Y);
			subNum(abs_mxy, X);	
		}
		copyNum(G, abs_mxy);
		copyNum(N_tmp, N);
		gcd(G, N_tmp);
		/*printf("A========================\n");
		printf("X\n");
		printNum(X);
		printf("Y\n");
		printNum(Y);
		printf("N\n");
		printNum(N);
		printf("A========================\n");*/
		j++;
		if(j%1000 == 0){
			printf("%d\n", j);
			/*if (j == 10000) 
				return;*/
		}
		//sleep(1);
	}
	printf("%d\n", j);
	copyNum(N, G);
	//free(X);
	//free(Y);
	//free(C);
	//free(G);
	//free(abs_mxy);
}



int main(int argc, char **argv) {
	unsigned int N[SIZE];
	setZero(N);
    //N[8] = 1;
    //N[8] = 1;
    //N[0] = 1;
    N[1] = 0x00000111;
    N[0] = 0x10010009;
    PollardRho(N);
	printf("Results \n");
    printNum(N);
	printf("********************\n");
	/*unsigned int X[32];
	unsigned int C[32];
	unsigned int X[32];
	unsigned int Y[32];
	unsigned int C[32];
	unsigned int N[32];*/
	//void fxfun(unsigned int * N, unsigned int * X, unsigned int * C, unsigned int * Y)
	//setZero(N, 32);	
	//setZero(R, 32);
	//N[0] = 0x00000009;
	//0xF00000007530eca9
    /*N[3] = 0x8ebaf1b9;
    N[2] = 0xfebaf1ba;
    N[1] = 0xfeba9876;
    N[0] = 0x7530eca9;*/
    /*0 x 1 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000001*/
    /*N[8] = 1;
    N[0] = 1;
    PollardRho(N, R, 32);
	printf("Results \n");
    printNum(N, 32);
	printNum(R, 32);
	printf("********************\n");
	*//*	
	setZero(A, 32);
	setZero(C, 32);
	setZero(N, 32);	
	setZero(X, 32);	
	setZero(Y, 32);	
	//N[1] = 0xF0010999;
    N[0] = 0x12345678;
    C[1] = 0x00000001;
	X[15] = 0x00000001;
	X[0] = 0x10000002;
	fxfun(N, X, C, Y , 32);
	printf("Results \n");
    printNum(N, 32);
	printNum(X, 32);
    printNum(C, 32);
	printNum(Y, 32);	*/
	/*unsigned int A[32];
	unsigned int B[32];
	printf("********************\n"); 	
	setZero(A, 32);
	setZero(B, 32);
	//1B6 9B4B ACD0 5F15	
	//A[1] = 0x01B69B4B;
    //A[0] = 0x00000000;
	A[31] = 0x80000999; // to keep
    A[0] = 0x10000002; // to keep
	//A[0] = 0x80000003;
	//DB4 DA5F 7EF4 12B1
	//B[1] = 0x0DB4DA5F;
	//B[1] = 0x00000001;
	//B[0] = 0x00000000;
	//B[1] = 0x80000999; // to keep
	B[0] = 0x10001010; // to keep
	//gcd(A, B, 32);
    modNum(A, B, 32);
    printf("Results \n");
    printNum(A, 32);*/
	
	//printNum(B, 32);
/*
	printf("********************\n");
    unsigned int A[2*32];
	unsigned int B[2*32];
	setZero(A, 2*32);
	setZero(B, 2*32);
	A[0] = 1;    
	copyFromTo(B, A, 32, 2*32);
	printNum(B, 2*32);
*/
   /* printf("********************\n");
    unsigned int N[32];
	setZero(N, 32);
    unsigned int D[32];
	setZero(D, 32);
    unsigned int Q[32];
	setZero(Q, 32);
    
    //N[1] = 0x00000111;
    N[0] = 0x00000008;
    //D[1] = 0x00000111;
    D[0] = 0x00000002;
    
    divNum(N, D, Q, 32);
    printf("Results\n");
    printNum(N, 32);
    printNum(D, 32);
    printNum(Q, 32);
    printf("********************\n");
	return 0;*/
	/*
	setZero(A, 32);
	setZero(B, 32);
	setZero(D, 32);
	setZero(R, 32);	
	A[1] = 0xF0010999;
    A[0] = 0x12345678;
    B[1] = 0xF0000000;
	B[0] = 0x7530eca9;
	gcd(A, B, 32);
    //modNum(A, B, R, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printNum(R, 32);
	printf("********************\n");*/
	/*setZero(A, 32);
	setZero(B, 32);
	setZero(D, 32);
	setZero(R, 32);	
	//1B6 9B4B ACD0 5F15
	A[0] = 0x00000008;
	//DB4 DA5F 7EF4 12B1
	//B[1] = 0x0DB4DA5F;
	B[0] = 0x00000008;
	divNum(A, B, D, R, 32);
    //divNum(A, B, D, R, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printNum(D, 32);
	printNum(R, 32);
	printf("********************\n");
	printf("********************\n");
	printf("********************\n");
	printf("********************\n");
	printf("********************\n");
	setZero(A, 32);
	setZero(B, 32);
	setZero(D, 32);
	setZero(R, 32);	
	A[0] = 0x89;
    //DB4 DA5F 7EF4 12B1
	B[0] = 0x21;
	gcd(A, B, 32);
    //divNum(A, B, D, R, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printNum(D, 32);
	printNum(R, 32);*/
	//printf("gcdl %ld\n",gcdl(0x00000005, 0x00000002));
	return 0;
}