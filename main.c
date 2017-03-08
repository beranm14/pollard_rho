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


unsigned int * gcd(unsigned int * N, unsigned int * M, unsigned int size){
	unsigned int * nm_r = (unsigned int *)malloc(sizeof(unsigned int) * size);
	while(!zeroNum(M, size)){
		/*printf("**************************\n");
		printf("N:\n");
		printNum(N, size);
		printf("M:\n");
		printNum(M, size);
		printf("++++++++++++++++++++++++++\n");*/
		modNum(N, M, size);
		/*printf("**************************\n");
		printf("nm_r: \n");
		printNum(nm_r, size);
		printf("++++++++++++++++++++++++++\n");*/
		copyNum(nm_r, M, size);
		copyNum(N, M, size);
		copyNum(M, nm_r, size);

		
		//getchar(); 
	}
	free(nm_r);
	return N;
}

void fxfun(unsigned int * N, unsigned int * X, unsigned int * C, unsigned int * Y, unsigned int size){
	setZero(Y, size);
	powNum(X, size);
	modNum(X, N, size);
	addNum(X, C, size);
	modNum(X, N, size);
	copyNum(Y, X, size);
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

void PollardRho(unsigned int * N, unsigned int * R, unsigned int size){
	zeroNum(R, size);
	if (isEven(N, size)){
		R[0] = 2;
		return;
	}
	unsigned int j = 0;
	unsigned int * x = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * x1 = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * y = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * y1 = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * c = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * g = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * abs_mxy = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * n_b = (unsigned int *)malloc(sizeof(unsigned int) * size);
	copyNum(n_b, N, size);
	setZero(y1, size);
	setZero(x, size);
	x[0] = 7;
	setZero(c, size);
	c[0] = 1;
	setZero(g, size);
	g[0] = 1;
	fxfun(N, x, c, y , size);
	//printf();
	while (isOne(g, size)){
		/*printf("Tady to padlo\n");
		printNum(N, size);
		printNum(x, size);
		printNum(c, size);*/
		fxfun(N, x, c, x1, size);
		copyNum(x, x1, size);
		fxfun(N, y, c, y1, size);
		fxfun(N, y1, c, y, size);
		if(bigger(x, y, size) == 1){
			copyNum(abs_mxy, x, size);
			subNum(abs_mxy, y, size);
		}else{
			copyNum(abs_mxy, y, size);
			subNum(abs_mxy, x, size);	
		}
		copyNum(g, abs_mxy, size);
		gcd(g, n_b, size);
		copyNum(n_b, N, size);
		
		/*printNum(x, size);
		printNum(y, size);
		printNum(abs_mxy, size);
		printf("%d\n",j);
		printf("=====================\n");*/
		j++;
		if(j%1000 == 0)
			printf("1000\n");
		//sleep(1);
	}
	printf("%d\n", j);
	copyNum(R, g, size);
	free(x);
	free(y);
	free(y1);
	free(c);
	free(g);
	free(abs_mxy);
}



int main(int argc, char **argv) {
	unsigned int A[32];
	unsigned int B[32];
	unsigned int R[32];
	unsigned int D[32];
	/*unsigned int X[32];
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
    printf("********************\n");
	setZero(A, 32);
	setZero(B, 32);
	setZero(D, 32);
	setZero(R, 32);	
	//A[1] = 0x80000999;
	A[1] = 0xF0010999;
    A[0] = 0x00000008;
    //B[1] = 0xF0000000;
	B[0] = 0x10000003;
	//shiftRightNum(A, 32);
	modNum(A, B, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printf("********************\n");
	return 0;
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