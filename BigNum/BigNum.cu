#include "BigNum.cuh"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#define SIZE ((5))

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

/*
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
*/

__device__ void  cuda_modNum(unsigned int * A, unsigned int * B){
    if (cuda_zeroNum(A) || cuda_zeroNum(B) || cuda_bigger(B, A) == 1){
        return;
    }
    unsigned int tmp_b[SIZE];
    while( 1 ){
        cuda_copyNum(tmp_b, B);
        unsigned int dr = 0;
        char gr_des = cuda_bigger(tmp_b, A);
        while(gr_des == 0){ // dokud je B menší než A
            // shiftuj doleva
            cuda_shiftLeftNum(tmp_b);
            gr_des = cuda_bigger(tmp_b, A);
            dr ++;
        }
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



__device__  void cuda_gcd(unsigned int * A, unsigned int * B){
    
    unsigned int R[SIZE];
    while(!cuda_zeroNum(B)){
        cuda_copyNum(R, A);
        cuda_modNum(R, B);
        cuda_copyNum(A, B);
        cuda_copyNum(B, R);
    }
    /*
    unsigned int t[SIZE];
    unsigned int shift;

    if(cuda_zeroNum(B)){
        return;      
    }
    if(cuda_zeroNum(A)){
        cuda_copyNum(A, B);
        return;
    }
    for(shift = 0; ((A[0] | B[0]) & 1) == 0; ++ shift){
        cuda_shiftRightNum(A);
        cuda_shiftRightNum(B);
    }
    while((A[0] & 1) == 0){
        cuda_shiftRightNum(A);
    }
    do{
        while((B[0] & 1) == 0){
            cuda_shiftRightNum(B);
        }
        if(cuda_bigger(A, B) == 1){
            cuda_copyNum(t, B);
            cuda_copyNum(B, A);
            cuda_copyNum(A, t);
        }
        cuda_subNum(B, A);
    } while (! cuda_zeroNum(B));
    cuda_shiftLeftNumBy(A, shift);
    */
}

__device__  void cuda_fxfun(unsigned int * N, unsigned int * X, unsigned int * C){
    //setZero(Y);
    cuda_powNum(X);
    cuda_modNum(X, N);
    cuda_addNum(X, C);
    cuda_modNum(X, N);
    //copyNum(Y, X);
}

//__global__ void pollardKernel(unsigned int * N, unsigned int * mem_xyc, unsigned int * result, unsigned int * dbgs){
__global__ void pollardKernel(unsigned int * N, unsigned int * mem_xyc, unsigned int * result){
    unsigned int threadID = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int * X = &mem_xyc[3 * threadID * SIZE + SIZE * 0];
    unsigned int * Y = &mem_xyc[3 * threadID * SIZE + SIZE * 1];
    unsigned int * C = &mem_xyc[3 * threadID * SIZE + SIZE * 2];
    
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
    
    //cuda_copyNum(Y, X);
    //cuda_fxfun(N, Y, C);
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
    //if(!cuda_isOne(G) && cuda_bigger(G, N) != 2)
    if(!cuda_isOne(G))
        cuda_copyNum(result, G);    
}
