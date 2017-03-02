#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "BigNum/BigNum.h"


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
}


unsigned int * gcd(unsigned int * N, unsigned int * M, unsigned int size){
	unsigned int * nm_d = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * nm_r = (unsigned int *)malloc(sizeof(unsigned int) * size);
	while(!zeroNum(M, size)){
		divNum(N, M, nm_d, nm_r, size);
		copyNum(N, M, size);
		copyNum(M, nm_r, size);

		printf("**************************\n");
		printf("N:\n");
		printNum(N, size);
		printf("M:\n");
		printNum(M, size);
		
		//getchar(); 
	}
	free(nm_r);
	free(nm_d);
	return N;
}

/*
def pollardRho(N):
	if N%2==0:
		return 2
	x = 7
	y = ((x*x)%N+1)%N
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
void PollardRho(unsigned int * A, unsigned int size){

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
	A[0] = 0x00000005;
    B[0] = 0x00000002;
	gcd(A, B, 32);
    //divNum(A, B, D, R, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printNum(D, 32);
	printNum(R, 32);
	printf("********************\n");
	setZero(A, 32);
	setZero(B, 32);
	setZero(D, 32);
	setZero(R, 32);	
	A[15] = 0x00000002;
    B[0] = 0x00000004;
	gcd(A, B, 32);
    //divNum(A, B, D, R, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printNum(D, 32);
	printNum(R, 32);
	printf("********************\n");
	setZero(A, 32);
	setZero(B, 32);
	setZero(D, 32);
	setZero(R, 32);	
	//1B6 9B4B ACD0 5F15
	A[1] = 0x01B69B4B;
    A[0] = 0xACD05F15;
    //DB4 DA5F 7EF4 12B1
	B[1] = 0x0DB4DA5F;
	B[0] = 0x7EF412B1;
	gcd(A, B, 32);
    //divNum(A, B, D, R, 32);
    printf("Results \n");
    printNum(A, 32);
	printNum(B, 32);
    printNum(D, 32);
	printNum(R, 32);
	//printf("gcdl %ld\n",gcdl(0x00000005, 0x00000002));
	return 0;
}