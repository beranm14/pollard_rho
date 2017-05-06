#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> 
#include "BigNum/BigNum.h"
#include "BigNum/BigNum.cuh"
#define SIZE ((5))

//

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


void getGpuNfo(){
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
        printf("CUDA Device %d: %s Gflops %f Processors %d Threads/Block %d\n",dev,prop.name,1e-6*Gflops,prop.multiProcessorCount,prop.maxThreadsPerBlock);
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
    // cudaSetDevice(MaxDevice);    
    //cudaSetDevice(0);
    //  Return max thread count
	printf("maxThreadsPerBlock %d \n", prop.maxThreadsPerBlock);
	return;
}


void genNum(unsigned int * x){
	setZero(x);
	x[0] = 0x07;
}

void genC(unsigned int * x, unsigned int * counter){
	setZero(x);
	copyNum(x, counter);
	addOne(counter);
}

void printMat(unsigned int * mem_xyc, unsigned int blocks, unsigned int threads){
	for(unsigned int i = 0; i < 3 * blocks * threads; i += 3){
		printNum ( mem_xyc + SIZE * ( i     )); // X
		printNum ( mem_xyc + SIZE * ( i + 1 )); // Y
		printNum ( mem_xyc + SIZE * ( i + 2 )); // C 
		printf("===================================================== \n");
	}
}

bool compareFields(unsigned int * a, unsigned int * b, unsigned int count){
	for (unsigned int i = 0; i < count; ++i){
		if (a [i] != b[i])
			return false;
	}
	return true;
}

void PollardRhoCu(unsigned int * N, unsigned int blocks, unsigned int threads){
	/*
The idea I've got in mind is to prepare multiple starting points
for X, Y and C
and each thread can actually count Y in place without bothering CPU
Therefore reasonable aproach would be to just prepare huge chunk of memory
for GPU to work with.

This is going to be changed. Let's make just one kernel which is going to start
on it's own given X, Y, C. All of the threads are going to iterate in algorithm
as it is and check in memory array if anyone actually found sthing.
If so all the threads just stop leaving result for the CPU to copy and show to user.
	*/
	if (isEven(N)){
		zeroNum(N);
		N[0] = 2;
		return;
	}
	
	// unsigned int sz = 3 * blocks * threads * SIZE;
	unsigned int * result = (unsigned int *) malloc(sizeof(unsigned int) * SIZE);
	setZero(result);
	
	unsigned int * gpu_N;
  	cudaMalloc((void **)&gpu_N, SIZE * sizeof(unsigned int));
	cudaMemcpy(gpu_N, N, SIZE * sizeof(unsigned int), cudaMemcpyHostToDevice);

  	unsigned int * gpu_result;
  	cudaMalloc((void **)&gpu_result, SIZE * sizeof(unsigned int));
	cudaMemcpy(gpu_result, result, SIZE * sizeof(unsigned int), cudaMemcpyHostToDevice);

  	cudaFuncSetCacheConfig(pollardKernel, cudaFuncCachePreferL1);
	
	printf("Running Kernels\n");
	
	pollardKernel<<<blocks, threads>>>(gpu_N, gpu_result);
	//SinglePollardKernel(N, result);

	cudaThreadSynchronize();
	cudaMemcpy(result, gpu_result, SIZE * sizeof(unsigned int), cudaMemcpyDeviceToHost);
	
	copyNum(N, result);
	cudaFree(gpu_result);
	cudaFree(gpu_N); 
}


int main(int argc, char **argv) {
	unsigned int blocks = 0;
	unsigned int threads = 0;
	unsigned int debug = 0;
	if(argc == 4){
		debug = atoi(argv[1]);
		blocks = atoi(argv[2]);
		threads = atoi(argv[3]);
	}
	cudaSetDevice(0);
	//getGpuNfo();
	unsigned int N[SIZE];
	setZero(N);
    //N[8] = 1;
    //N[8] = 1;
    //N[0] = 1;
    //N[1] = 0x00000111;
    //N[0] = 0x10010009;
    //N[0] = 0x00044d69;
    //123432322333420120051
    //N[0] = 0x20120051;
    //N[1] = 0x23223334;
    //N[3] = 0x00012343;
    // cb3b6f92b9d54fd
    //N[1] = 0x0cb3b6f9;
    //N[0] = 0x2b9d54fd;
    //a8bc39b45
    //N[1] = 0x0000000a;
    //N[0] = 0x8bc39b45;
    //
    //N[0] = 0x00000121;
    //fd42d4eb2c4b7b1
	
    if(debug == 1){
    	float milliseconds = 0;
    	cudaEvent_t start, stop;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);

    	//N[1] = 0x0fd42d4e;
    	//N[0] = 0xb2c4b7b1;
    	//N[1] = 0x0000000a;
    	//N[0] = 0x8bc39b45;
    	// 3b52bc bb1a2c33
    	N[1] = 0x003b52bc;
    	N[0] = 0xbb1a2c33;

    	cudaEventRecord(start, 0);

    	PollardRhoCu(N, blocks, threads);
    	
    	cudaEventRecord(stop, 0);
		cudaEventSynchronize(stop);

		cudaEventElapsedTime(&milliseconds, start, stop);
		
		printf("Results \n");
   		printNum(N);
		printf("Time: ");
		printf("%f\n", milliseconds);
		printf("********************\n");
    }else{
    	// custom
    	//fd42d4eb2c4b7b1 * 3fa81e47
    	N[1] = 0x0fd42d4e;
    	N[0] = 0xb2c4b7b1;
    	PollardRhoCu(N, 128, 128);
		printf("Results \n");
   		printNum(N);
		printf("********************\n");
	}
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
	cudaDeviceReset();
	return 0;
}
