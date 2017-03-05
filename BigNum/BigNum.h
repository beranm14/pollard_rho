#ifndef BigNum
#define BigNum

void printNum(unsigned int * num, unsigned int size);

void setZero(unsigned int * num, unsigned int size);

void addNum(unsigned int * A, unsigned int * B, unsigned int * C, unsigned int size);

void subNum(unsigned int * A, unsigned int * B, unsigned int * C, unsigned int size);

void copyNum(unsigned int * A, unsigned int * B, unsigned int size);

void shiftLeftNum(unsigned int * A, unsigned int k);

void mulNum(unsigned int * A, unsigned int * B, unsigned int * C, unsigned int size);

char zeroNum(unsigned int * num, unsigned int size);

char bigger(unsigned int * A, unsigned int * B, unsigned int size);

unsigned int getHighBit(unsigned int * A, unsigned int size);

char isEven(unsigned int * A, unsigned int size);

char isOne(unsigned int * A, unsigned int size);

void addOne(unsigned int * A, unsigned int * C, unsigned int size);

void getNum(unsigned int a, unsigned int * A);

void divNum(unsigned int * A, unsigned int * B, unsigned int * D, unsigned int * R, unsigned int size);
#endif