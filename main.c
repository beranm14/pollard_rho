#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

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

void addNum(unsigned int * A, unsigned int * B, unsigned int * C, unsigned int size){
	unsigned int i;
	unsigned long int tmp;
	unsigned long int tmp_carry;
	unsigned int carry = 0;
	for(i = 0; i < size; i ++){
		tmp = ((unsigned long int) A[i] + B[i]);
		C[i] = (tmp & 0xFFFFFFFF) + carry;
		tmp_carry = ((unsigned long int) tmp & 0xFFFFFFFF00000000); 
		carry = ((unsigned long int) tmp_carry >> 32);
	}
}
void subNum(unsigned int * A, unsigned int * B, unsigned int * C, unsigned int size){
	unsigned int i;
	unsigned long int tmp;
	unsigned int carry = 0;
	for(i = 0; i < size; i ++){
		tmp = ((unsigned long int) 0xFFFFFFFF00000000 + A[i]);
		tmp = ((unsigned long int) tmp - (B[i] + carry));
		carry = (0xFFFFFFFF - ((unsigned long int) ((unsigned long int) 0xFFFFFFFF00000000 & tmp) >> 32));
		C[i] = (tmp & 0xFFFFFFFF);
	}
}

void copyNum(unsigned int * A, unsigned int * B, unsigned int size){
	unsigned int i;
	for(i = 0; i < size; i ++)
		A[i] = B[i];
}
void shiftLeftNum(unsigned int * A, unsigned int k){
    char flg;
    char flc = 0;
    for (unsigned char j = 0 ; j < k; j++){
    	((((unsigned int) A[j] & 0x80000000)) ? flg = 1 : flg = 0 );
    	A[j] = (A[j] << 1);
    	if(flc)
    		A[j] |= 1;
    	(flg == 1 ? flc = 1 : flc = 0);
    }
}

void mulNum(unsigned int * A, unsigned int * B, unsigned int * C, unsigned int size){
    unsigned int * tmpa = (unsigned int *)malloc(sizeof(unsigned int) * size);
    unsigned int * tmpc = (unsigned int *)malloc(sizeof(unsigned int) * size);
    copyNum(tmpa, A, size);
    setZero(C, size);
    setZero(tmpc, size);
    unsigned char j = 0x01;
    for(unsigned char k = 0; k < sizeof(A); k++){
        for(unsigned char i = 0; i < size; i++){
            if (B[i] & j){
                addNum(C, tmpa, tmpc, size);
                copyNum(C, tmpc, size);
            }
        }
        j <<= 1;
        shiftLeftNum(tmpa, size);
    }
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
    for (int i = size - 1; i != -1; i --){
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

void getNum(unsigned int a, unsigned int * A){
    unsigned int where = a / 32;
    unsigned int which = a % 32;
    A[where] = 1;
    while (which){
        A[where] <<= 1;
        which --;
    }
}

void divNum(unsigned int * A, unsigned int * B, unsigned int * D, unsigned int * R, unsigned int size){
	// A / B = D + R/B
    // if (alen == 0 || blen == 0 || alen < blen){
    if (zeroNum(A, size) || zeroNum(B, size) || bigger(B, A, size)){
        copyNum(R, A, size);
        setZero(D, size);
        return;
    }
    unsigned int * tmp_a = (unsigned int *)malloc(sizeof(unsigned int) * size);
    unsigned int * tmp_aa = (unsigned int *)malloc(sizeof(unsigned int) * size);
    unsigned int * tmpp = (unsigned int *)malloc(sizeof(unsigned int) * size);
    unsigned int * tmpres = (unsigned int *)malloc(sizeof(unsigned int) * size);
	unsigned int * tmpd = (unsigned int *)malloc(sizeof(unsigned int) * size);
    
    copyNum(tmp_a, A, size);
    unsigned int dr;
    while( 1 ){
        setZero(tmpp, size);
        setZero(tmpd, size);
        setZero(tmpres, size);
        setZero(tmp_aa, size);

        dr = (getHighBit(tmp_a, size) - getHighBit(B, size)); // finding number to divide to

		getNum(dr, tmpp);
        mulNum(tmpp, B, tmpres, size); // getNum * B = tmpres
        while (bigger(tmpres, A, size)){
        	dr --;
        	getNum(dr, tmpp);
        	mulNum(tmpp, B, tmpres, size); // getNum * B = tmpres
        }

        subNum(tmp_a, tmpres, tmp_aa , size); // A - tmpres

        addNum(D, tmpp, tmpd, size); // D + getNum = tmpd
        copyNum(D, tmpd, size); // D = tmpd

        
		//copyNum(tmp_a, tmp_aa, size); 
        copyNum(R, tmp_aa, size); 
        //if(getHighBit(R, size) < blen)
        if(bigger(B, R, size))
            break;
        else{
            copyNum(tmp_a, R, size);
        }
//      printNum(A, 32);
//		printNum(B, 32);
//		printNum(D, 32);
//		printNum(R, 32);
//		printf("up\n");
//		break;
	}
    free(tmp_a);
    free(tmp_aa);
    free(tmpp);
    free(tmpres);
    free(tmpd);	
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
	A[0] = 0x00000080;
	B[0] = 0x00000021;
    divNum(A, B, D, R, 32);
    printNum(A, 32);
	printNum(B, 32);
	printNum(D, 32);
	printNum(R, 32);
	/*    
  	getNum(3, A);
    printNum(A, 32);
	printf("%d\n", getHighBit(A, 32));
    subNum(A, B, C, 32);
	printNum(A, 32);
	printNum(B, 32);
	printNum(C, 32);
	printf("\n");
	unsigned long int tmp;
	unsigned int a = 0xFFFF0000;
	unsigned int b = 0x00010010;
	tmp = ((unsigned long int) a + b);
	tmp = ((unsigned long int) tmp >> 32);
	printf("%lx\n", tmp);
	*/
	return 0;
}