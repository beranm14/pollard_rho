CXX=nvcc
LD=nvcc
#CFLAGS=-g -G
#FLAGS=-g -G
CFLAGS=-O3
FLAGS=-O3
LIBS=

all: ./BigNum/BigNum.o ./BigNum/BigNum_cuda.o main.o
	$(LD) ./BigNum/BigNum.o ./BigNum/BigNum_cuda.o main.o $(LIBS) $(FLAGS) -o pollard_opt

BigNum/BigNum.o: ./BigNum/BigNum.h ./BigNum/BigNum.cpp
	$(CXX) $(CFLAGS) -c -o ./BigNum/BigNum.o ./BigNum/BigNum.cpp

BigNum/BigNum_cuda.o: ./BigNum/BigNum.cuh ./BigNum/BigNum.cu
	$(CXX) $(CFLAGS) -c -o ./BigNum/BigNum_cuda.o ./BigNum/BigNum.cu

main.o: main.cu ./BigNum/BigNum.h ./BigNum/BigNum.cuh
	$(CXX) $(CFLAGS) -c -o main.o main.cu

clm:
	[ -f main.o ] && rm main.o || exit 0;

cln:
	[ -f ./BigNum/BigNum.o ] && rm ./BigNum/BigNum.o || exit 0;

clo:
	[ -f ./BigNum/BigNum_cuda.o ] && rm ./BigNum/BigNum_cuda.o || exit 0;

clt:
	[ -f try ] && rm pollard_opt || exit 0;

clean: clm cln clt clo
