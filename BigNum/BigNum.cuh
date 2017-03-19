#ifndef BigNum_cuda
#define BigNum_cuda
#define SIZE ((32))


__device__ void   cuda_printNum(unsigned int * num);

__device__ void   cuda_setZero(unsigned int * num);

__device__ void   cuda_addNum(unsigned int * A, unsigned int * B);

__device__ void   cuda_subNum(unsigned int * A, unsigned int * B);

__device__ void   cuda_copyNum(unsigned int * A, unsigned int * B);

__device__ void   cuda_copyFromTo(unsigned int * A, unsigned int * B, unsigned int from);

__device__ void   cuda_shiftLeftNum(unsigned int * A);

__device__ void   cuda_mulNum(unsigned int * A, unsigned int * B);

__device__ char cuda_zeroNum(unsigned int * num);

__device__ char cuda_bigger(unsigned int * A, unsigned int * B);

__device__ unsigned int cuda_getHighBit(unsigned int * A);

__device__ char cuda_isEven(unsigned int * A);

__device__ char cuda_isOne(unsigned int * A);

__device__ void   cuda_addOne(unsigned int * A, unsigned int * C);

__device__ void   cuda_getNum(unsigned int a, unsigned int * A);

__device__ void   cuda_powNum(unsigned int * A);

__device__ void   cuda_modNum(unsigned int * A, unsigned int * B);

__device__ void   cuda_divNum(unsigned int * A, unsigned int * B, unsigned int * D);

__device__ void   cuda_addfromto(unsigned int * A, unsigned int * B, unsigned int from);

__device__ void   cuda_shiftRightNum(unsigned int * A);

__device__ unsigned int cuda_getBit(unsigned int a, unsigned int * A);

__device__ void   cuda_setBit(unsigned int a, unsigned int * A);


#endif