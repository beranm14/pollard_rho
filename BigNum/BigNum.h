#ifndef BigNum
#define BigNum

void printNum(unsigned int * num, unsigned int size);

void setZero(unsigned int * num, unsigned int size);

void addNum(unsigned int * A, unsigned int * B, unsigned int size);

void subNum(unsigned int * A, unsigned int * B, unsigned int size);

void copyNum(unsigned int * A, unsigned int * B, unsigned int size);

void copyFromTo(unsigned int * A, unsigned int * B, unsigned int from, unsigned int size);

void shiftLeftNum(unsigned int * A, unsigned int k);

void mulNum(unsigned int * A, unsigned int * B, unsigned int size);

char zeroNum(unsigned int * num, unsigned int size);

char bigger(unsigned int * A, unsigned int * B, unsigned int size);

unsigned int getHighBit(unsigned int * A, unsigned int size);

char isEven(unsigned int * A, unsigned int size);

char isOne(unsigned int * A, unsigned int size);

void addOne(unsigned int * A, unsigned int * C, unsigned int size);

void getNum(unsigned int a, unsigned int * A);

void powNum(unsigned int * A, unsigned int size);

void modNum(unsigned int * A, unsigned int * B, unsigned int size);

void testModNum(unsigned int * A, unsigned int * B, unsigned int size);

void TestModNum(unsigned int * A, unsigned int * B, unsigned int size);

void divNum(unsigned int * A, unsigned int * B, unsigned int * D, unsigned int size);

void addfromto(unsigned int * A, unsigned int * B, unsigned int from, unsigned int size);

void shiftRightNum(unsigned int * A, unsigned int k);

unsigned int getBit(unsigned int a, unsigned int * A, unsigned int size);

void setBit(unsigned int a, unsigned int * A, unsigned int size);


#endif