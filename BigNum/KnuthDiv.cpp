#include "KnuthDiv.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>
#include <iostream>

/*
Source code found at: http://llvm.org/docs/doxygen/html/APInt_8cpp_source.html#l01509
And edited to work on 32 bit unsigned integers
*/


inline unsigned int max ( int a, int b ) { return a > b ? a : b; }
inline unsigned int min ( int a, int b ) { return a > b ? b : a; }


int countRealSize (unsigned int *v, unsigned int size){
   while (v[size - 1] == 0){
		size --;
   }
   return size;
}

int nlz(unsigned x) {
   int n;
 
   if (x == 0) return(32);
   n = 0;
   if (x <= 0x0000FFFF) {n = n +16; x = x <<16;}
   if (x <= 0x00FFFFFF) {n = n + 8; x = x << 8;}
   if (x <= 0x0FFFFFFF) {n = n + 4; x = x << 4;}
   if (x <= 0x3FFFFFFF) {n = n + 2; x = x << 2;}
   if (x <= 0x7FFFFFFF) {n = n + 1;}
   return n;
}

unsigned countLeadingZeros(unsigned in){
	unsigned out = 0;
	unsigned pnt = 0x80000000;
	while((in & pnt) == 0){
		out ++;
		pnt >>= 1;
	}
	return out;
}
void KnuthDiv(unsigned *u, unsigned *v, unsigned *q, unsigned* r,
                     unsigned m, unsigned n) {

  uint64_t b = uint64_t(1) << 32;

  unsigned shift = countLeadingZeros(v[n-1]);
  unsigned v_carry = 0;
  unsigned u_carry = 0;
  if (shift) {
    for (unsigned i = 0; i < m+n; ++i) {
      unsigned u_tmp = u[i] >> (32 - shift);
      u[i] = (u[i] << shift) | u_carry;
      u_carry = u_tmp;
    }
    for (unsigned i = 0; i < n; ++i) {
      unsigned v_tmp = v[i] >> (32 - shift);
      v[i] = (v[i] << shift) | v_carry;
      v_carry = v_tmp;
    }
  }
  u[m+n] = u_carry;
  int j = m;
  do {
    uint64_t dividend = ((uint64_t(u[j+n]) << 32) + u[j+n-1]);
    uint64_t qp = dividend / v[n-1];
    uint64_t rp = dividend % v[n-1];
    if (qp == b || qp*v[n-2] > b*rp + u[j+n-2]) {
      qp--;
      rp += v[n-1];
      if (rp < b && (qp == b || qp*v[n-2] > b*rp + u[j+n-2]))
        qp--;
    }
    bool isNeg = false;
    for (unsigned i = 0; i < n; ++i) {
      uint64_t u_tmp = uint64_t(u[j+i]) | (uint64_t(u[j+i+1]) << 32);
      uint64_t subtrahend = uint64_t(qp) * uint64_t(v[i]);
      bool borrow = subtrahend > u_tmp;
      uint64_t result = u_tmp - subtrahend;
      unsigned k = j + i;
      u[k++] = (unsigned)(result & (b-1));
      u[k++] = (unsigned)(result >> 32);
      while (borrow && k <= m+n) {
        borrow = u[k] == 0;
        u[k]--;
        k++;
      }
      isNeg |= borrow;
    }
    if (isNeg) {
      bool carry = true; 
      for (unsigned i = 0; i <= m+n; ++i) {
        u[i] = ~u[i] + carry;
        carry = carry && u[i] == 0;
      }
    }
    q[j] = (unsigned)qp;
    if (isNeg) {
      q[j]--;
      bool carry = false;
      for (unsigned i = 0; i < n; i++) {
        unsigned limit = std::min(u[j+i],v[i]);
        u[j+i] += v[i] + carry;
        carry = u[j+i] < limit || (carry && u[j+i] == limit);
      }
      u[j+n] += carry;
    }
  } while (--j >= 0);
  if (r) {
    if (shift) {
      unsigned carry = 0;
      for (int i = n-1; i >= 0; i--) {
        r[i] = (u[i] >> shift) | carry;
        carry = u[i] << (32 - shift);
      }
    } else {
      for (int i = n-1; i >= 0; i--) {
        r[i] = u[i];
      }
    }
  }
}