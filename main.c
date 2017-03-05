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
	unsigned int * nm_d = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * nm_r = (unsigned int *)malloc(sizeof(unsigned int) * size);
	while(!zeroNum(M, size)){
		/*printf("**************************\n");
		printf("N:\n");
		printNum(N, size);
		printf("M:\n");
		printNum(M, size);
		printf("++++++++++++++++++++++++++\n");*/
		divNum(N, M, nm_d, nm_r, size);
		/*printf("**************************\n");
		printf("nm_r: \n");
		printNum(nm_r, size);
		printf("++++++++++++++++++++++++++\n");*/
		copyNum(N, M, size);
		copyNum(M, nm_r, size);

		
		//getchar(); 
	}
	free(nm_r);
	free(nm_d);
	return N;
}

void fxfun(unsigned int * X, unsigned int * C, unsigned int * Y, unsigned int size){
	zeroNum(Y, size);
	unsigned int * y_tmp = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * y_sqr = (unsigned int *)malloc(sizeof(unsigned int) * size);
	copyNum(y_tmp, Y, size); // better create square function
	mulNum(Y, y_tmp, y_sqr, size);
}
/*
def pollardRho(N):
	if N%2==0:
		return 2
	x = 7
	y = ((x*x)%N+c)%N
	c = 1
	g = 1
	j = 0
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
	unsigned int * x = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * y = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * c = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * g = (unsigned int *)malloc(sizeof(unsigned int) * size);
	setZero(x, size);
	x[0] = 7;
	setZero(c, size);
	c[0] = 1;
	setZero(g, size);
	g[0] = 1;
	// count y = ((x*x)+1)%N
	unsigned int * res = (unsigned int *)malloc(sizeof(unsigned int) * size);
	setZero(res, size);
	mulNum(x, x, res, size);
	unsigned int * addoneres = (unsigned int *)malloc(sizeof(unsigned int) * size);
	setZero(addoneres, size);
	addOne(res, addoneres, size);

	while (isOne(g, size)){


	}
}


int main(int argc, char **argv) {
	unsigned int A[32];
	unsigned int B[32];
	unsigned int D[32];
	unsigned int R[32];
	setZero(A, 32);
	setZero(B, 32);
	setZero(D, 32);
	setZero(R, 32);
	//1B6 9B4B ACD0 5F15	
	//A[1] = 0x01B69B4B;
    //A[0] = 0x00000000;
	/*A[1] = 0x80000999; // to keep
    A[0] = 0x12345678; // to keep
	//A[0] = 0x80000003;
	//DB4 DA5F 7EF4 12B1
	//B[1] = 0x0DB4DA5F;
	//B[1] = 0x00000001;
	//B[0] = 0x00000000;
	B[1] = 0x80000999; // to keep
	B[0] = 0x87654321; // to keep
	gcd(A, B, 32);
    //divNum(A, B, D, R, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printNum(D, 32);
	printNum(R, 32);
	return 0;*/
	printf("********************\n");
	setZero(A, 32);
	setZero(B, 32);
	setZero(D, 32);
	setZero(R, 32);	
	//A[1] = 0x80000999;
    A[0] = 0x7530eca9;
    //B[0] = 0x00000004;
	B[1] = 0x00000002;
	B[0] = 0x00000001;
	//gcd(A, B, 32);
    divNum(A, B, D, R, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printNum(D, 32);
	printNum(R, 32);
	return 0;
	printf("********************\n");
	setZero(A, 32);
	setZero(B, 32);
	setZero(D, 32);
	setZero(R, 32);	
	A[1] = 0xF0010999;
    A[0] = 0x12345678;
    B[1] = 0xF0000000;
	B[0] = 0x7530eca9;
	gcd(A, B, 32);
    //divNum(A, B, D, R, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printNum(D, 32);
	printNum(R, 32);
	printf("********************\n");
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