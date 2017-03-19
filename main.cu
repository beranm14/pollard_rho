#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> 
#include "BigNum/BigNum.h"
//#include "BigNum/BigNum.cuh"
#define SIZE ((32))

// =======================================

__device__ void  cuda_printNum(unsigned int * num){
	unsigned int i;
	for(i = 0; i < SIZE; i ++)
		printf("%x ", num[i]);
	printf("\n");
}

__device__ void  cuda_setZero(unsigned int * num){
	unsigned int i;
	for(i = 0; i < SIZE; i ++)
		num[i] = 0;
}

__device__ void  cuda_addNum(unsigned int * A, unsigned int * B){
	unsigned int i;
	unsigned long int tmp;
	unsigned long int tmp_carry;
	unsigned int carry = 0;
	for(i = 0; i < SIZE; i ++){
		tmp = ((unsigned long int) A[i] + B[i]);
		A[i] = (tmp & 0xFFFFFFFF) + carry;
		tmp_carry = ((unsigned long int) tmp & 0xFFFFFFFF00000000); 
		carry = ((unsigned long int) tmp_carry >> 32);
	}
}
__device__ void  cuda_addOne(unsigned int * A, unsigned int * C){
	unsigned int i;
	unsigned long int tmp;
	unsigned long int tmp_carry;
	unsigned int carry = 0;
	for(i = 0; i < SIZE; i ++){
		if ( i == 0)
			tmp = ((unsigned long int) A[i] + 1);
		else
			tmp = ((unsigned long int) A[i]);	
		C[i] = (tmp & 0xFFFFFFFF) + carry;
		tmp_carry = ((unsigned long int) tmp & 0xFFFFFFFF00000000); 
		carry = ((unsigned long int) tmp_carry >> 32);
	}
}
__device__ void  cuda_subNum(unsigned int * A, unsigned int * B){
	unsigned int i;
	unsigned long int tmp;
	unsigned int carry = 0;
	for(i = 0; i < SIZE; i ++){
		tmp = ((unsigned long int) 0xFFFFFFFF00000000 + A[i]);
		tmp = ((unsigned long int) tmp - (B[i] + carry));
		carry = (0xFFFFFFFF - ((unsigned long int) ((unsigned long int) 0xFFFFFFFF00000000 & tmp) >> 32));
		A[i] = (tmp & 0xFFFFFFFF);
	}
}

__device__ void  cuda_copyNum(unsigned int * A, unsigned int * B){
	unsigned int i;
	for(i = 0; i < SIZE; i ++)
		A[i] = B[i];
}

__device__ void  cuda_copyFromTo(unsigned int * A, unsigned int * B, unsigned int from){
	unsigned int i;
	for(i = from; i < SIZE; i ++)
		A[i] = B[i - from];
}

__device__ void  cuda_shiftLeftNum(unsigned int * A){ // shift for 32 bits in one step would be nice to consider
    char flg;
    char flc = 0;
    for (unsigned int j = 0 ; j < SIZE; j++){
    	((((unsigned int) A[j] & 0x80000000)) ? flg = 1 : flg = 0 );
    	A[j] = (A[j] << 1);
    	if(flc)
    		A[j] |= 1;
    	(flg == 1 ? flc = 1 : flc = 0);
    }
}
__device__ void  cuda_shiftRightNum(unsigned int * A){
    char flg;
    char flc = 0;
    for (unsigned char j = SIZE - 1; ; j --){
        ((((unsigned int) A[j] & 0x00000001)) ? flg = 1 : flg = 0 );
        A[j] = (A[j] >> 1);
        if(flc)
            A[j] |= 0x80000000;
        (flg == 1 ? flc = 1 : flc = 0);
        if (j == 0)
            break;
    }
}

__device__ void  cuda_addfromto(unsigned int * A, unsigned int * B, unsigned int from){
	unsigned int i;
	unsigned long int tmp;
	unsigned long int tmp_carry;
	unsigned int carry = 0;
	for(i = from; i < SIZE; i ++){
		tmp = ((unsigned long int) A[i] + B[i-from]);
		A[i] = (tmp & 0xFFFFFFFF) + carry;
		tmp_carry = ((unsigned long int) tmp & 0xFFFFFFFF00000000); 
		carry = ((unsigned long int) tmp_carry >> 32);
	}
}

__device__ void  cuda_mulNum(unsigned int * A, unsigned int * B){
    //unsigned int * tmpa = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmpa[SIZE];
    //unsigned int * tmpc = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmpc[SIZE];
    cuda_copyNum(tmpa, A);
    cuda_setZero(tmpc);
    unsigned long int j = 1;
    for(unsigned int k = 0; k < 32; k++){ // needed to be changed accordingly with sizeof the datatype
        for(unsigned int i = 0; i < SIZE; i++){
            if (B[i] & j){
                cuda_addfromto(tmpc, tmpa, i);
            }
        }
        j <<= 1;
        cuda_shiftLeftNum(tmpa);
    }
    cuda_copyNum(A, tmpc);
    //free(tmpa);
    //free(tmpc);
}

__device__ void  cuda_powNum(unsigned int * A){
    //unsigned int * tmpa = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmpa[SIZE];
    //unsigned int * tmpc = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmpc[SIZE];
    cuda_copyNum(tmpa, A);
    cuda_setZero(tmpc); 
    unsigned long int j = 1;
    for(unsigned int k = 0; k < 32; k++){ // needed to be changed accordingly with sizeof the datatype
        for(unsigned int i = 0; i < SIZE; i++){
            if (A[i] & j){
                cuda_addfromto(tmpc, tmpa, i);
            }
        }
        j <<= 1;
        cuda_shiftLeftNum(tmpa);
    }
    cuda_copyNum(A, tmpc);
    //free(tmpa);
    //free(tmpc);
}

__device__ char cuda_zeroNum(unsigned int * num){
	unsigned int i;
	for(i = 0; i < SIZE; i ++)
		if (num[i] > 0)
			return 0;
	return 1;
}

__device__ char cuda_bigger(unsigned int * A, unsigned int * B){
	unsigned int i;
	for(i = SIZE - 1; ; i --){
		if (A[i] > B[i])
			return 1;
		if (A[i] < B[i])
			return 0;
		if (i == 0)
			break;
	}
	return 2;
}

__device__ unsigned int cuda_getHighBit(unsigned int * A){
    unsigned int bits_size = SIZE * 32;
    unsigned int l = 0;
    unsigned int tmp;
    for (long int i = SIZE - 1; i != -1; i --){
    	tmp = 0x80000000;
        while ((A[i] & tmp) == 0 && tmp != 0){
            tmp >>= 1;
            l ++;
        }
        if(tmp != 0)
            break;
    }
    l = bits_size - l;
    return l;
}


__device__ char cuda_isEven(unsigned int * A){
	if ((A[0] & 1) == 0)
		return 1;
	return 0;
}
__device__ char cuda_isOne(unsigned int * A){
	for (unsigned int i = 1; i < SIZE; i++){
		if (A[i] > 0)
			return 0;
	}
	if (A[0] == 1)
		return 1;
	return 0;
}

__device__ void  cuda_getNum(unsigned int a, unsigned int * A){
    unsigned int where = a / SIZE;
    unsigned int which = a % SIZE;
    A[where] = 1;
    while (which){
        A[where] <<= 1;
        which --;
    }
}

__device__ unsigned int cuda_getBit(unsigned int a, unsigned int * A){
    unsigned int where = a / SIZE;
    unsigned int which = a % SIZE;
    unsigned int pnt = ((unsigned int)1 << which);
    return ((unsigned int) A[where] & pnt); 
}
__device__ void  cuda_setBit(unsigned int a, unsigned int * A){
    unsigned int where = a / SIZE;
    unsigned int which = a % SIZE;
    unsigned int pnt = ((unsigned int) 1 << which);
    A[where] |= pnt; 
}
__device__ void  cuda_unsBit(unsigned int a, unsigned int * A){
    unsigned int where = a / SIZE;
    unsigned int which = a % SIZE;
    unsigned int pnt = ((unsigned int) 1 << which);
    A[where] &= ~(pnt); 
}


__device__ void  cuda_divNum(unsigned int * N, unsigned int * D, unsigned int * Q){
    if (cuda_zeroNum(D) || cuda_zeroNum(N) || cuda_bigger(D, N) == 1){
        return;
    }
    //unsigned int * R = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int R[SIZE];
    cuda_setZero(Q);
    cuda_setZero(R);
    unsigned int i = SIZE*32-1;
    while (1){ // 32 is sizeof int, need to changed to sizeof
        cuda_shiftLeftNum(R);
        if (cuda_getBit(i, N)){
            R[0] |= 1;
        }else{
            R[0] &= ~((unsigned int) 1);
        }
        if(cuda_bigger(R, D) == 2 || cuda_bigger(R, D) == 1){
            cuda_subNum(R, D);
            cuda_setBit(i, Q);
        }
        if (i == 0)
            break;
        i --;
    }
    cuda_copyNum(N, R);
    //free(R);
}

__device__ void  cuda_shiftLeftNumBy(unsigned int * A, unsigned int gen){ // shift for 32 bits in one step would be nice to consider
    unsigned int howmuch = gen / 32; // needed to be changed accordingly to datatype
	unsigned int wheremc = gen % 32; // needed to be changed accordingly to datatype
	//printf("%u \n", howmuch);
	if(howmuch > 0){
		for(unsigned int k = SIZE - 1;  ; k --){
			A[k] = A[k - howmuch];
			if (k - howmuch == 0)
				break;
		}
    }
	//printf("%u \n", wheremc);
	while(wheremc){
    	cuda_shiftLeftNum(A);
    	wheremc --;
    }
    /*while(gen){
    	shiftLeftNum(A);
    	gen --;
    }*/
    /*char flg;
    char flc = 0;
    for (unsigned int j = 0 ; j < size; j++){
    	((((unsigned int) A[j] & 0x80000000)) ? flg = 1 : flg = 0 );
    	A[j] = (A[j] << 1);
    	if(flc)
    		A[j] |= 1;
    	(flg == 1 ? flc = 1 : flc = 0);
    }*/
}

__device__ void  cuda_modNum(unsigned int * A, unsigned int * B){
    if (cuda_zeroNum(A) || cuda_zeroNum(B) || cuda_bigger(B, A) == 1){
        return;
    }
    //unsigned int * tmp_b = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmp_b[SIZE];
    char gr_des;
    unsigned int dr;
    unsigned int gen;
    while( 1 ){
        cuda_copyNum(tmp_b, B);
        dr = 0;
        //gr_des = bigger(tmp_b, A);
        gen = cuda_getHighBit(A) - cuda_getHighBit(tmp_b);
        cuda_shiftLeftNumBy(tmp_b, gen);
        gr_des = cuda_bigger(tmp_b, A);
        // pokud bylo rovno ok
        // pokud bylo větší shift doprava
        if(gr_des == 1){
            cuda_shiftRightNum(tmp_b);
            dr --;
        }
        cuda_subNum(A, tmp_b); // residuo
        if(cuda_bigger(B, A)){
            break;
        }
    }
    //free(tmp_b);
}
// =======================================

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


__device__  void cuda_gcd(unsigned int * A, unsigned int * B){
	//unsigned int * R = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
	unsigned int R[SIZE];
	while(!cuda_zeroNum(B)){
		/*printf("**************************\n");
		printf("N:\n");
		printNum(N);
		printf("M:\n");
		printNum(M);
		printf("++++++++++++++++++++++++++\n");*/
		cuda_copyNum(R, A);
		cuda_modNum(R, B);
		cuda_copyNum(A, B);
		cuda_copyNum(B, R);
		
		/*printf("**************************\n");
		printf("nm_r: \n");
		printNum(nm_r);
		printf("++++++++++++++++++++++++++\n");*/
		//getchar(); 
	}
	//free(R);
}

__device__  void cuda_fxfun(unsigned int * N, unsigned int * X, unsigned int * C){
	//setZero(Y);
	cuda_powNum(X);
	cuda_modNum(X, N);
	cuda_addNum(X, C);
	cuda_modNum(X, N);
	//copyNum(Y, X);
}

__global__ void pollardKernel(unsigned int * N, unsigned int * mem_xyc, unsigned int * result){
	int threadID = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int * X = &mem_xyc[3 * threadID + SIZE * 0];
	unsigned int * Y = &mem_xyc[3 * threadID + SIZE * 1];
	unsigned int * C = &mem_xyc[3 * threadID + SIZE * 2];
	unsigned int G[SIZE];
	unsigned int N_tmp[SIZE];
	unsigned int abs_mxy[SIZE];
	
	/*
	// THIS SHOULD BE PREPARED IN MEMORY
	setZero(X);
	X[0] = 7;
	setZero(C);
	C[0] = 1;
	setZero(G);
	G[0] = 1;
	*/
	cuda_copyNum(Y, X);
	cuda_fxfun(N, Y, C);
	//while (isOne(G)){
		cuda_fxfun(N, X, C);
		cuda_fxfun(N, Y, C);
		cuda_fxfun(N, Y, C);
		if(cuda_bigger(X, Y) == 1){
			cuda_copyNum(abs_mxy, X);
			cuda_subNum(abs_mxy, Y);
		}else{
			cuda_copyNum(abs_mxy, Y);
			cuda_subNum(abs_mxy, X);	
		}
		cuda_copyNum(G, abs_mxy);
		cuda_copyNum(N_tmp, N);
		cuda_gcd(G, N_tmp);
	//}
	if(! cuda_isOne(G))
		cuda_copyNum(result, G);	
}

void genNum(unsigned int * x){
	for (unsigned int i = 0; i < SIZE; i ++){
		x[i] = rand();
	}
}

void PollardRhoCu(unsigned int * N, unsigned int blocks, unsigned int threads){
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
	unsigned int * mem_xyc = (unsigned int *) malloc(3 * blocks * threads * SIZE * sizeof(unsigned int));
	for(unsigned int i = 0; i < 3 * blocks * threads; i ++){
		genNum(mem_xyc); // X
		mem_xyc += SIZE;		
		copyNum(mem_xyc, (mem_xyc - SIZE)); // Y
		mem_xyc += SIZE;
		genNum(mem_xyc); // C
	}
	unsigned int * result = (unsigned int *) malloc(sizeof(unsigned int) * SIZE);
	setZero(result);
	
	unsigned int * gpu_xyc; 
  	cudaMalloc((void **)&gpu_xyc, 3 * blocks * threads * SIZE * sizeof(unsigned int));
	cudaMemcpy(gpu_xyc, mem_xyc, 3 * blocks * threads * SIZE * sizeof(unsigned int), cudaMemcpyHostToDevice);

  	unsigned int * gpu_N;
  	cudaMalloc((void **)&gpu_N, SIZE * sizeof(unsigned int));
	cudaMemcpy(gpu_N, N, SIZE * sizeof(unsigned int), cudaMemcpyHostToDevice);

  	unsigned int * gpu_result;
  	cudaMalloc((void **)&gpu_result, SIZE * sizeof(unsigned int));
	cudaMemcpy(gpu_result, result, SIZE * sizeof(unsigned int), cudaMemcpyHostToDevice);

	do{
		pollardKernel<<<blocks, threads>>>(gpu_N, gpu_xyc, gpu_result);
		cudaThreadSynchronize();
		cudaMemcpy(result, gpu_result, SIZE * sizeof(unsigned int), cudaMemcpyDeviceToHost);
	} while (zeroNum(result));

	printNum(result);
	cudaFree(gpu_result);
	cudaFree(gpu_N);
	cudaFree(gpu_xyc);
}

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
    cudaSetDevice(MaxDevice);
    //  Return max thread count
	printf("maxThreadsPerBlock %d \n", prop.maxThreadsPerBlock);
	return;
}

int main(int argc, char **argv) {
	srand (time(NULL));
	getGpuNfo();
	unsigned int N[SIZE];
	setZero(N);
    //N[8] = 1;
    //N[8] = 1;
    //N[0] = 1;
    N[1] = 0x00000111;
    N[0] = 0x10010009;
    PollardRhoCu(N, 1, 1);
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