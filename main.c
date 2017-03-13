#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "BigNum/BigNum.h"


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


void gcd(unsigned int * A, unsigned int * B, unsigned int size){
	unsigned int * R = (unsigned int *)malloc(sizeof(unsigned int) * size);
	while(!zeroNum(B, size)){
		/*printf("**************************\n");
		printf("N:\n");
		printNum(N, size);
		printf("M:\n");
		printNum(M, size);
		printf("++++++++++++++++++++++++++\n");*/
		copyNum(R, A, size);
		modNum(R, B, size);
		copyNum(A, B, size);
		copyNum(B, R, size);
		
		/*printf("**************************\n");
		printf("nm_r: \n");
		printNum(nm_r, size);
		printf("++++++++++++++++++++++++++\n");*/
		//getchar(); 
	}
	free(R);
}

void fxfun(unsigned int * N, unsigned int * X, unsigned int * C, unsigned int size){
	//setZero(Y, size);
	powNum(X, size);
	modNum(X, N, size);
	addNum(X, C, size);
	modNum(X, N, size);
	//copyNum(Y, X, size);
}
/*
def pollardRho(N):
	if N%2==0:
		return 2
	x = 7
	y = ((x*x)%N+c)%N
	c = 1
	g = 1
	while g==1:             
		x = ((x*x)%N+c)%N
		y1 = ((y*y)%N+c)%N
		y = ((y1*y1)%N+c)%N
		g = gcd(abs(x-y),N)
	return g
*/

void PollardRho(unsigned int * N, unsigned int size){
	if (isEven(N, size)){
		zeroNum(N, size);
		N[0] = 2;
		return;
	}
	unsigned int j = 0;
	unsigned int * X = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * Y = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * C = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * G = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * N_tmp = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * abs_mxy = (unsigned int *)malloc(sizeof(unsigned int) * size);
	
	setZero(X, size);
	X[0] = 7;
	setZero(C, size);
	C[0] = 1;
	setZero(G, size);
	G[0] = 1;
	copyNum(Y, X, size);
	fxfun(N, Y, C, size);
	//printf();
	while (isOne(G, size)){
		/*printf("Tady to padlo\n");
		printNum(N, size);
		printNum(x, size);
		printNum(c, size);*/
		fxfun(N, X, C, size);
		fxfun(N, Y, C, size);
		fxfun(N, Y, C, size);
		/*printf("B========================\n");
		printf("X\n");
		printNum(X, size);
		printf("Y\n");
		printNum(Y, size);
		printf("N\n");
		printNum(N, size);
		printf("B========================\n"); */
		if(bigger(X, Y, size) == 1){
			copyNum(abs_mxy, X, size);
			subNum(abs_mxy, Y, size);
		}else{
			copyNum(abs_mxy, Y, size);
			subNum(abs_mxy, X, size);	
		}
		copyNum(G, abs_mxy, size);
		copyNum(N_tmp, N, size);
		gcd(G, N_tmp, size);
		/*printf("A========================\n");
		printf("X\n");
		printNum(X, size);
		printf("Y\n");
		printNum(Y, size);
		printf("N\n");
		printNum(N, size);
		printf("A========================\n");*/
		j++;
		if(j%1000 == 0){
			printf("%d\n", j);
			//return;
		}
		//sleep(1);
	}
	printf("%d\n", j);
	copyNum(N, G, size);
	free(X);
	free(Y);
	free(C);
	free(G);
	free(abs_mxy);
}



int main(int argc, char **argv) {
	unsigned int N[17];
	setZero(N, 17);
    //N[8] = 1;
    N[1] = 0x00000111;
    N[0] = 0x10000009;
    PollardRho(N, 17);
	printf("Results \n");
    printNum(N, 17);
	printf("********************\n");
	/*unsigned int X[32];
	unsigned int C[32];
	unsigned int X[32];
	unsigned int Y[32];
	unsigned int C[32];
	unsigned int N[32];*/
	//void fxfun(unsigned int * N, unsigned int * X, unsigned int * C, unsigned int * Y, unsigned int size)
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
	/*printf("********************\n"); 	
	setZero(A, 32);
	setZero(B, 32);
	//1B6 9B4B ACD0 5F15	
	//A[1] = 0x01B69B4B;
    //A[0] = 0x00000000;
	A[1] = 0x80000999; // to keep
    A[0] = 0x10000002; // to keep
	//A[0] = 0x80000003;
	//DB4 DA5F 7EF4 12B1
	//B[1] = 0x0DB4DA5F;
	//B[1] = 0x00000001;
	//B[0] = 0x00000000;
	B[1] = 0x80000999; // to keep
	B[0] = 0x00001000; // to keep
	//gcd(A, B, 32);
    mulNum(A, B, 32);
    printf("Results \n");
    printNum(A, 32);
	//printNum(B, 32);*/
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