CXX=g++
LD=g++
#CFLAGS=-Wall -pedantic -g -std=c++0x
CFLAGS=-Wall -pedantic -O3 -std=c++0x
LIBS=

all: ./BigNum/BigNum.o main.o
	$(LD) -o try ./BigNum/BigNum.o main.o $(LIBS) $(FLAGS)

BigNum.o: ./BigNum/BigNum.h ./BigNum/BigNum.cpp
	$(CXX) $(CFLAGS) -c -o ./BigNum/BigNum.o ./BigNum/BigNum.cpp

main.o: main.c ./BigNum/BigNum.h
	$(CXX) $(CFLAGS) -c -o main.o main.c

clm:
	[ -f main.o ] && rm main.o || exit 0;

cln:
	[ -f ./BigNum/BigNum.o ] && rm ./BigNum/BigNum.o || exit 0;

clt:
	[ -f try ] && rm try || exit 0;

clean: clm cln clt
