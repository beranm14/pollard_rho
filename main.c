#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "BigNum/BigNum.h"


static long gcdl(long a, long b) {
  long r;
  while(b!=0){
    r = a % b;
    a = b;
    b = r;
  }
  return a;
}

unsigned int * gcd(unsigned int * N, unsigned int * M, unsigned int size){
		printNum(N, size);
		printNum(M, size);
	unsigned int * nm_d = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * nm_r = (unsigned int *)malloc(sizeof(unsigned int) * size);
	while(!zeroNum(M, size)){
		divNum(N, M, nm_d, nm_r, size);
		copyNum(N, M, size);
		copyNum(M, nm_r, size);
		printNum(N, size);
		printNum(M, size);
	}
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
	A[0] = 0x00000002;
	B[0] = 0x00000001;
    divNum(A, B, D, R, 32);
    printNum(A, 32);
	printNum(B, 32);
	printNum(D, 32);
	printNum(R, 32);
	//printf("gcdl %ld\n",gcdl(0x0000000A, 0x00000003));
	return 0;
}