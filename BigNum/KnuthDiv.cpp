#include "KnuthDiv.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

inline unsigned int max ( int a, int b ) { return a > b ? a : b; }
inline unsigned int min ( int a, int b ) { return a > b ? b : a; }


int countRealSize (unsigned int *v, unsigned int size){
   while (v[size - 1] == 0){
		size --;
   		if (size == 1){
            return 1;
   			break;
         }
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
 

int KnuthDiv(unsigned u[], unsigned v[],unsigned q[], unsigned r[], unsigned int size) {
   int m = countRealSize(u, size);
   int n = countRealSize(v, size);
   //printf("%d\n", m);
   //printf("%d\n", n);
   // 26 zeroes need to changed to 
   const uint64_t b = 0x100000000; //0x4000000; //67108864; 
   const uint64_t mask = 0xffffffff; //0x3ffffff; //67108863; 
   unsigned *un, *vn;                        
   uint64_t qhat;                  
   uint64_t rhat;                 
   uint64_t p;                
   int64_t t, k;
   int s, i, j;

 
   if (m < n || n <= 0 || v[n-1] == 0)
      return 1;                      
 
   if (n == 1) {                        
      k = 0;                           
      for (j = m - 1; j >= 0; j--) {   
         q[j] = (k*b + u[j])/v[0];    
         k = (k*b + u[j]) - q[j]*v[0];
      }
      if (r != NULL) r[0] = k;
      return 0;
   } 
   s = nlz(v[n-1])-7; 
   vn = (unsigned *)alloca(4*n);
   for (i = n - 1; i > 0; i--)
      vn[i] = ((v[i] << s)&mask) | (((uint64_t)v[i-1] >> (32-s))&mask);
   vn[0] = (v[0] << s)&mask;
 
   un = (unsigned *)alloca(4*(m + 1));
   un[m] = ((uint64_t)u[m-1] >> (32-s))&mask;
   for (i = m - 1; i > 0; i--)
      un[i] = ((u[i] << s)&mask) | (((uint64_t)u[i-1] >> (32-s))&mask);
   un[0] = (u[0] << s)&mask;
 
   for (j = m - n; j >= 0; j--) {
      qhat = (un[j+n]*b + un[j+n-1])/vn[n-1];
      rhat = (un[j+n]*b + un[j+n-1]) - qhat*vn[n-1];
again:
      if (qhat >= b || qhat*vn[n-2] > b*rhat + un[j+n-2])
      { qhat = qhat - 1;
        rhat = rhat + vn[n-1];
        if (rhat < b) goto again;
      }

      k = 0;
      for (i = 0; i < n; i++) {
         p = qhat*vn[i];
         t = un[i+j] - k - (p & mask);
         un[i+j] = t&mask;
         k = ((p >> 32)) - ((t >> 32));
      }
      t = un[j+n] - k;
      un[j+n] = t;
 
      q[j] = qhat;        
      if (t < 0) {           
         q[j] = q[j] - 1;      
         k = 0;
         for (i = 0; i < n; i++) {
            t = (uint64_t)un[i+j] + vn[i] + k;
            un[i+j] = t&mask;
            k = ((uint64_t)t >> 32);
         }
         un[j+n] = un[j+n] + k;
      }
   } 
   if (r != NULL) {
      for (i = 0; i < n-1; i++)
         r[i] = (((uint64_t) un[i] >> s)&mask) | (((uint64_t)un[i+1] << (32-s))&mask);
      r[n-1] = ((uint64_t)un[n-1] >> s)&mask;
   }
   return 0;
}
