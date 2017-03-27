#ifndef BigNum
#define BigNum
#define SIZE ((17))


 void   printNum(unsigned int * num);

 void   setZero(unsigned int * num);

 void   addNum(unsigned int * A, unsigned int * B);

 void   addOne(unsigned int * A);

 void   subNum(unsigned int * A, unsigned int * B);

 void   copyNum(unsigned int * A, unsigned int * B);

 void   copyFromTo(unsigned int * A, unsigned int * B, unsigned int from);

 void   shiftLeftNum(unsigned int * A);

 void   mulNum(unsigned int * A, unsigned int * B);

 char zeroNum(unsigned int * num);

 char bigger(unsigned int * A, unsigned int * B);

 unsigned int getHighBit(unsigned int * A);

 char isEven(unsigned int * A);

 char isOne(unsigned int * A);

 void   getNum(unsigned int a, unsigned int * A);

 void   powNum(unsigned int * A);

 void   modNum(unsigned int * A, unsigned int * B);

 void   divNum(unsigned int * A, unsigned int * B, unsigned int * D);

 void   addfromto(unsigned int * A, unsigned int * B, unsigned int from);

 void   shiftRightNum(unsigned int * A);

 unsigned int getBit(unsigned int a, unsigned int * A);

 void   setBit(unsigned int a, unsigned int * A);


#endif