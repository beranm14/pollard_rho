#include "BigNum.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

void  printNum(unsigned int * num){
	unsigned int i;
	for(i = 0; i < SIZE; i ++)
		printf("%x ", num[i]);
	printf("\n");
}

void  setZero(unsigned int * num){
	unsigned int i;
	for(i = 0; i < SIZE; i ++)
		num[i] = 0;
}

void  addNum(unsigned int * A, unsigned int * B){
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
void  addOne(unsigned int * A){
	unsigned int i;
	unsigned long int tmp;
	unsigned long int tmp_carry;
	unsigned int carry = 0;
	for(i = 0; i < SIZE; i ++){
		if ( i == 0)
			tmp = ((unsigned long int) A[i] + 1);
		else
			tmp = ((unsigned long int) A[i]);	
		A[i] = (tmp & 0xFFFFFFFF) + carry;
		tmp_carry = ((unsigned long int) tmp & 0xFFFFFFFF00000000); 
		carry = ((unsigned long int) tmp_carry >> 32);
	}
}
void  subNum(unsigned int * A, unsigned int * B){
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

void  copyNum(unsigned int * A, unsigned int * B){
	unsigned int i;
	for(i = 0; i < SIZE; i ++)
		A[i] = B[i];
}

void  copyFromTo(unsigned int * A, unsigned int * B, unsigned int from){
	unsigned int i;
	for(i = from; i < SIZE; i ++)
		A[i] = B[i - from];
}

void  shiftLeftNum(unsigned int * A){ // shift for 32 bits in one step would be nice to consider
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
void  shiftRightNum(unsigned int * A){
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

void  addfromto(unsigned int * A, unsigned int * B, unsigned int from){
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

void  mulNum(unsigned int * A, unsigned int * B){
    //unsigned int * tmpa = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmpa[SIZE];
    //unsigned int * tmpc = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmpc[SIZE];
    copyNum(tmpa, A);
    setZero(tmpc);
    unsigned long int j = 1;
    for(unsigned int k = 0; k < 32; k++){ // needed to be changed accordingly with sizeof the datatype
        for(unsigned int i = 0; i < SIZE; i++){
            if (B[i] & j){
                addfromto(tmpc, tmpa, i);
            }
        }
        j <<= 1;
        shiftLeftNum(tmpa);
    }
    copyNum(A, tmpc);
    //free(tmpa);
    //free(tmpc);
}

void  powNum(unsigned int * A){
    //unsigned int * tmpa = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmpa[SIZE];
    //unsigned int * tmpc = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmpc[SIZE];
    copyNum(tmpa, A);
    setZero(tmpc); 
    unsigned long int j = 1;
    for(unsigned int k = 0; k < 32; k++){ // needed to be changed accordingly with sizeof the datatype
        for(unsigned int i = 0; i < SIZE; i++){
            if (A[i] & j){
                addfromto(tmpc, tmpa, i);
            }
        }
        j <<= 1;
        shiftLeftNum(tmpa);
    }
    copyNum(A, tmpc);
    //free(tmpa);
    //free(tmpc);
}

char zeroNum(unsigned int * num){
	unsigned int i;
	for(i = 0; i < SIZE; i ++)
		if (num[i] > 0)
			return 0;
	return 1;
}

char bigger(unsigned int * A, unsigned int * B){
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

unsigned int getHighBit(unsigned int * A){
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


char isEven(unsigned int * A){
	if ((A[0] & 1) == 0)
		return 1;
	return 0;
}
char isOne(unsigned int * A){
	for (unsigned int i = 1; i < SIZE; i++){
		if (A[i] > 0)
			return 0;
	}
	if (A[0] == 1)
		return 1;
	return 0;
}

void  getNum(unsigned int a, unsigned int * A){
    unsigned int where = a / SIZE;
    unsigned int which = a % SIZE;
    A[where] = 1;
    while (which){
        A[where] <<= 1;
        which --;
    }
}

unsigned int getBit(unsigned int a, unsigned int * A){
    unsigned int where = a / SIZE;
    unsigned int which = a % SIZE;
    unsigned int pnt = ((unsigned int)1 << which);
    return ((unsigned int) A[where] & pnt); 
}
void  setBit(unsigned int a, unsigned int * A){
    unsigned int where = a / SIZE;
    unsigned int which = a % SIZE;
    unsigned int pnt = ((unsigned int) 1 << which);
    A[where] |= pnt; 
}
void  unsBit(unsigned int a, unsigned int * A){
    unsigned int where = a / SIZE;
    unsigned int which = a % SIZE;
    unsigned int pnt = ((unsigned int) 1 << which);
    A[where] &= ~(pnt); 
}


void  divNum(unsigned int * N, unsigned int * D, unsigned int * Q){
    if (zeroNum(D) || zeroNum(N) || bigger(D, N) == 1){
        return;
    }
    //unsigned int * R = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int R[SIZE];
    setZero(Q);
    setZero(R);
    unsigned int i = SIZE*32-1;
    while (1){ // 32 is sizeof int, need to changed to sizeof
        shiftLeftNum(R);
        if (getBit(i, N)){
            R[0] |= 1;
        }else{
            R[0] &= ~((unsigned int) 1);
        }
        if(bigger(R, D) == 2 || bigger(R, D) == 1){
            subNum(R, D);
            setBit(i, Q);
        }
        if (i == 0)
            break;
        i --;
    }
    copyNum(N, R);
    //free(R);
}

void  shiftLeftNumBy(unsigned int * A, unsigned int gen){ // shift for 32 bits in one step would be nice to consider
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
    	shiftLeftNum(A);
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

void  modNum(unsigned int * A, unsigned int * B){
    if (zeroNum(A) || zeroNum(B) || bigger(B, A) == 1){
        return;
    }
    //unsigned int * tmp_b = (unsigned int *)malloc(sizeof(unsigned int) * SIZE);
    unsigned int tmp_b[SIZE];
    char gr_des;
    unsigned int dr;
    unsigned int gen;
    while( 1 ){
        copyNum(tmp_b, B);
        dr = 0;
        //gr_des = bigger(tmp_b, A);
        gen = getHighBit(A) - getHighBit(tmp_b);
        shiftLeftNumBy(tmp_b, gen);
        gr_des = bigger(tmp_b, A);
        // pokud bylo rovno ok
        // pokud bylo větší shift doprava
        if(gr_des == 1){
            shiftRightNum(tmp_b);
            dr --;
        }
        subNum(A, tmp_b); // residuo
        if(bigger(B, A)){
            break;
        }
    }
    //free(tmp_b);
}

/*
void  modNum(unsigned int * A, unsigned int * B){
    if (zeroNum(A) || zeroNum(B) || bigger(B, A) == 1){
        return;
    }
    unsigned int * tmp_b = (unsigned int *)malloc(sizeof(unsigned int) * size);
    while( 1 ){
        copyNum(tmp_b, B);
        unsigned int dr = 0;
        char gr_des = bigger(tmp_b, A);
        while(gr_des == 0){ // dokud je B menší než A
            // shiftuj doleva
            shiftLeftNum(tmp_b);
            gr_des = bigger(tmp_b, A);
            dr ++;
        }
        // pokud bylo rovno ok
        // pokud bylo větší shift doprava
        if(gr_des == 1){
            shiftRightNum(tmp_b);
            dr --;
        }
        subNum(A, tmp_b); // residuo
        if(bigger(B, A)){
            break;
        }
    }
    free(tmp_b);
}
*/
