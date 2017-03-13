#include "BigNum.h"
#include "KnuthDiv.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

void printNum(unsigned int * num, unsigned int size){
	unsigned int i;
	for(i = 0; i < size; i ++)
		printf("%x ", num[i]);
	printf("\n");
}

void setZero(unsigned int * num, unsigned int size){
	unsigned int i;
	for(i = 0; i < size; i ++)
		num[i] = 0;
}

void addNum(unsigned int * A, unsigned int * B, unsigned int size){
	unsigned int i;
	unsigned long int tmp;
	unsigned long int tmp_carry;
	unsigned int carry = 0;
	for(i = 0; i < size; i ++){
		tmp = ((unsigned long int) A[i] + B[i]);
		A[i] = (tmp & 0xFFFFFFFF) + carry;
		tmp_carry = ((unsigned long int) tmp & 0xFFFFFFFF00000000); 
		carry = ((unsigned long int) tmp_carry >> 32);
	}
}
void addOne(unsigned int * A, unsigned int * C, unsigned int size){
	unsigned int i;
	unsigned long int tmp;
	unsigned long int tmp_carry;
	unsigned int carry = 0;
	for(i = 0; i < size; i ++){
		if ( i == 0)
			tmp = ((unsigned long int) A[i] + 1);
		else
			tmp = ((unsigned long int) A[i]);	
		C[i] = (tmp & 0xFFFFFFFF) + carry;
		tmp_carry = ((unsigned long int) tmp & 0xFFFFFFFF00000000); 
		carry = ((unsigned long int) tmp_carry >> 32);
	}
}
void subNum(unsigned int * A, unsigned int * B, unsigned int size){
	unsigned int i;
	unsigned long int tmp;
	unsigned int carry = 0;
	for(i = 0; i < size; i ++){
		tmp = ((unsigned long int) 0xFFFFFFFF00000000 + A[i]);
		tmp = ((unsigned long int) tmp - (B[i] + carry));
		carry = (0xFFFFFFFF - ((unsigned long int) ((unsigned long int) 0xFFFFFFFF00000000 & tmp) >> 32));
		A[i] = (tmp & 0xFFFFFFFF);
	}
}

void copyNum(unsigned int * A, unsigned int * B, unsigned int size){
	unsigned int i = 0;
	for(; i < size; i ++)
		A[i] = B[i];
}

void copyFromTo(unsigned int * A, unsigned int * B, unsigned int from, unsigned int size){
	unsigned int i;
	for(i = from; i < size; i ++)
		A[i] = B[i - from];
}

void shiftLeftNum(unsigned int * A, unsigned int k){ // shift for 32 bits in one step would be nice to consider
    char flg;
    char flc = 0;
    for (unsigned int j = 0 ; j < k; j++){
    	((((unsigned int) A[j] & 0x80000000)) ? flg = 1 : flg = 0 );
    	A[j] = (A[j] << 1);
    	if(flc)
    		A[j] |= 1;
    	(flg == 1 ? flc = 1 : flc = 0);
    }
}
void shiftRightNum(unsigned int * A, unsigned int k){
    char flg;
    char flc = 0;
    for (unsigned char j = k - 1; ; j --){
        ((((unsigned int) A[j] & 0x00000001)) ? flg = 1 : flg = 0 );
        A[j] = (A[j] >> 1);
        if(flc)
            A[j] |= 0x80000000;
        (flg == 1 ? flc = 1 : flc = 0);
        if (j == 0)
            break;
    }
}

void addfromto(unsigned int * A, unsigned int * B, unsigned int from, unsigned int size){
	unsigned int i;
	unsigned long int tmp;
	unsigned long int tmp_carry;
	unsigned int carry = 0;
	for(i = from; i < size; i ++){
		tmp = ((unsigned long int) A[i] + B[i-from]);
		A[i] = (tmp & 0xFFFFFFFF) + carry;
		tmp_carry = ((unsigned long int) tmp & 0xFFFFFFFF00000000); 
		carry = ((unsigned long int) tmp_carry >> 32);
	}
}

void mulNum(unsigned int * A, unsigned int * B, unsigned int size){
    unsigned int * tmpa = (unsigned int *)malloc(sizeof(unsigned int) * size);
    unsigned int * tmpc = (unsigned int *)malloc(sizeof(unsigned int) * size);
    copyNum(tmpa, A, size);
    setZero(tmpc, size);
    unsigned long int j = 1;
    for(unsigned int k = 0; k < 32; k++){ // needed to be changed accordingly with sizeof the datatype
        for(unsigned int i = 0; i < size; i++){
            if (B[i] & j){
                addfromto(tmpc, tmpa, i, size);
            }
        }
        j <<= 1;
        shiftLeftNum(tmpa, size);
    }
    copyNum(A, tmpc, size);
    free(tmpa);
    free(tmpc);
}

void powNum(unsigned int * A, unsigned int size){
    unsigned int * tmpa = (unsigned int *)malloc(sizeof(unsigned int) * size);
    unsigned int * tmpc = (unsigned int *)malloc(sizeof(unsigned int) * size);
    copyNum(tmpa, A, size);
    setZero(tmpc, size); 
    unsigned long int j = 1;
    for(unsigned int k = 0; k < 32; k++){ // needed to be changed accordingly with sizeof the datatype
        for(unsigned int i = 0; i < size; i++){
            if (A[i] & j){
                addfromto(tmpc, tmpa, i, size);
            }
        }
        j <<= 1;
        shiftLeftNum(tmpa, size);
    }
    copyNum(A, tmpc, size);
    free(tmpa);
    free(tmpc);
}

char zeroNum(unsigned int * num, unsigned int size){
	unsigned int i;
	for(i = 0; i < size; i ++)
		if (num[i] > 0)
			return 0;
	return 1;
}

char bigger(unsigned int * A, unsigned int * B, unsigned int size){
	unsigned int i;
	for(i = size - 1; ; i --){
		if (A[i] > B[i])
			return 1;
		if (A[i] < B[i])
			return 0;
		if (i == 0)
			break;
	}
	return 2;
}

unsigned int getHighBit(unsigned int * A, unsigned int size){
   if(zeroNum(A, size))
        return 0;
    unsigned int bits_size = size * 32;
    unsigned int l = 0;
    for (long int i = size - 1; i != -1; i --){
        unsigned int tmp = 0x80000000;
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

char isEven(unsigned int * A, unsigned int size){
	if ((A[0] & 1) == 0)
		return 1;
	return 0;
}
char isOne(unsigned int * A, unsigned int size){
	for (unsigned int i = 1; i < size; i++){
		if (A[i] > 0)
			return 0;
	}
	if (A[0] == 1)
		return 1;
	return 0;
}

void getNum(unsigned int a, unsigned int * A, unsigned int size){
    unsigned int where = a / size;
    unsigned int which = a % size;
    A[where] = 1;
    while (which){
        A[where] <<= 1;
        which --;
    }
}

unsigned int getBit(unsigned int a, unsigned int * A, unsigned int size){
    unsigned int where = a / size;
    unsigned int which = a % size;
    unsigned int pnt = ((unsigned int)1 << which);
    return ((unsigned int) A[where] & pnt); 
}
void setBit(unsigned int a, unsigned int * A, unsigned int size){
    unsigned int where = a / size;
    unsigned int which = a % size;
    unsigned int pnt = ((unsigned int) 1 << which);
    A[where] |= pnt; 
}
void unsBit(unsigned int a, unsigned int * A, unsigned int size){
    unsigned int where = a / size;
    unsigned int which = a % size;
    unsigned int pnt = ((unsigned int) 1 << which);
    A[where] &= ~(pnt); 
}

/*
void modNum(unsigned int * N, unsigned int * D, unsigned int size){
    if (zeroNum(D, size) || zeroNum(N, size) || bigger(D, N, size) == 1){
        return;
    }
    unsigned int * R = (unsigned int *)malloc(sizeof(unsigned int) * size);
    setZero(R, size);
    for (unsigned int i = size*32 - 1; ; i--){ // 32 is sizeof int, need to changed to sizeof
        shiftLeftNum(R, size);
        (getBit(i, N, size) ? R[0] |= 1 : R[0] &= ~1);
        if(bigger(R, D, size) == 1 || bigger(R, D, size) == 2){
            subNum(R, D, size);
        }
        if (i == 0)
            break;
    }
    copyNum(N, R, size);
    free(R);
}
*/

 
void divNum(unsigned int * A, unsigned int * B, unsigned int * D, unsigned int size){
    /*unsigned int * R = (unsigned int *)malloc(sizeof(unsigned int) * size);
    setZero(R, size);
    KnuthDiv(A, B, D, R, size);
    copyNum(A, R, size);
    free(R);*/
    if (zeroNum(A, size) || zeroNum(B, size) || bigger(B, A, size) == 1){
        setZero(D, size);
        return;
    }
    setZero(D, size);
    unsigned int * get_num = (unsigned int *)malloc(sizeof(unsigned int) * size);
    unsigned int * tmp_b = (unsigned int *)malloc(sizeof(unsigned int) * size);
    setZero(get_num, size);
    while( 1 ){
        copyNum(tmp_b, B, size);
        unsigned int dr = 0;
        char gr_des = bigger(tmp_b, A, size);
        while(gr_des == 0){ 
            shiftLeftNum(tmp_b, size);
            gr_des = bigger(tmp_b, A, size);
            dr ++;
        }
        if(gr_des == 1){
            shiftRightNum(tmp_b, size);
            dr --;
        }
        getNum(dr, get_num, size);
        subNum(A, tmp_b, size); // residuo
        addNum(D, get_num, size);
        setZero(get_num, size);

        if(bigger(B, A, size)){
            break;
        }
    }
    free(tmp_b);
    free(get_num);
}

void modNum(unsigned int * A, unsigned int * B, unsigned int size){
	if (zeroNum(A, size) || zeroNum(B, size) || bigger(B, A, size) == 1){
        return;
    }
	//printf("==============\n");
    unsigned int * R = (unsigned int *)malloc(sizeof(unsigned int) * size);
    unsigned int * D = (unsigned int *)malloc(sizeof(unsigned int) * size);
    /*printf("A ");
    printNum(A, size);
    printf("B ");
    printNum(B, size);*/
    setZero(R, size);
    setZero(D, size);
    KnuthDiv(A, B, D, R, size);
    copyNum(A, R, size);
    free(R);	
    free(D);
    /*printf("D ");
    printNum(D, size);
    printf("R ");
    printNum(R, size);
    printf("RA ");
    printNum(A, size);
    printf("++++++++++++++\n");*/
    /*if (zeroNum(A, size) || zeroNum(B, size) || bigger(B, A, size) == 1){
        return;
    }
    unsigned int * tmp_b = (unsigned int *)malloc(sizeof(unsigned int) * size);
    while( 1 ){
        copyNum(tmp_b, B, size);
        unsigned int dr = 0;
        char gr_des = bigger(tmp_b, A, size);
        while(gr_des == 0){ // dokud je B menší než A
            shiftLeftNum(tmp_b, size);
            gr_des = bigger(tmp_b, A, size);
            dr ++;
        }
        if(gr_des == 1){
            shiftRightNum(tmp_b, size);
            dr --;
        }
        subNum(A, tmp_b, size); // residuo
        if(bigger(B, A, size)){
            break;
        }
    }
    free(tmp_b);*/
}