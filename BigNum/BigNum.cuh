#ifndef BigNum_cuda
#define BigNum_cuda
#define SIZE ((5))


//__device__ inline void   cuda_printNum(unsigned int * num);

__device__ inline void   cuda_setZero(unsigned int * num);

__device__ inline void   cuda_addNum(unsigned int * A, const unsigned int * B);

__device__ inline void   cuda_subNum(unsigned int * A, const unsigned int * B);

__device__ inline void   cuda_copyNum(unsigned int * A, const unsigned int * B);

__device__ inline void   cuda_copyFromTo(unsigned int * A, const unsigned int * B, unsigned int from);

__device__ inline void   cuda_shiftLeftNum(unsigned int * A);

__device__ inline void   cuda_mulNum(unsigned int * A, const unsigned int * B);

__device__ inline char cuda_zeroNum(const unsigned int * num);

__device__ inline char cuda_bigger(const unsigned int * A, const unsigned int * B);

__device__ inline unsigned int cuda_getHighBit(unsigned int * A);

__device__ inline char cuda_isEven(const unsigned int * A);

__device__ inline char cuda_isOne(const unsigned int * A);

__device__ inline void   cuda_addOne(unsigned int * A, unsigned int * C);

__device__ inline void   cuda_getNum(unsigned int a, unsigned int * A);

__device__ inline void   cuda_powNum(unsigned int * A);

__device__ inline void   cuda_modNum(unsigned int * A, const unsigned int * B);

__device__ inline void   cuda_divNum(unsigned int * A, unsigned int * B, unsigned int * D);

__device__ inline void   cuda_addfromto(unsigned int * A, const unsigned int * B, unsigned int from);

__device__ inline void   cuda_shiftRightNum(unsigned int * A);

__device__ inline unsigned int cuda_getBit(unsigned int a, const unsigned int * A);

__device__ inline void   cuda_setBit(unsigned int a, unsigned int * A);

__device__  inline void cuda_gcd(unsigned int * A, const unsigned int * B);

__device__  inline void cuda_fxfun(const unsigned int * N, unsigned int * X, unsigned int * C);

//__global__ inline void pollardKernel(unsigned int * N, unsigned int * mem_xyc, unsigned int * result, unsigned int * dbgs);
__global__  void pollardKernel(const unsigned int * N, unsigned int * mem_xyc, unsigned int * result);

__global__  void prepareDataKernel(const unsigned int * N, unsigned int * mem_xyc);

#endif
