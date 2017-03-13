/* This divides an n-word dividend by an m-word divisor, giving an
n-m+1-word quotient and m-word remainder. The bignums are in arrays of
words. Here a "word" is 32 bits. This routine is designed for a 64-bit
machine which has a 64/64 division instruction. */
 
#include <stdio.h>
#include <stdlib.h>     //To define "exit", req'd by XLC.
 
#define max(x, y) ((x) > (y) ? (x) : (y))
 
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
 
void dumpit(char *msg, int n, unsigned v[]) {
   int i;
   printf(msg);
   for (i = n-1; i >= 0; i--) printf(" %08x", v[i]);
   printf("\n");
}

 
int divmnu(unsigned q[], unsigned r[],
     const unsigned u[], const unsigned v[],
     int m, int n) {
 
   const unsigned long long b = 67108864LL; 
   const unsigned long long mask = 67108863LL; 
   unsigned *un, *vn;                        
   unsigned long long qhat;                  
   unsigned long long rhat;                 
   unsigned long long p;                
   long long t, k;
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
      vn[i] = ((v[i] << s)&mask) | (((unsigned long long)v[i-1] >> (26-s))&mask);
   vn[0] = (v[0] << s)&mask;
 
   un = (unsigned *)alloca(4*(m + 1));
   un[m] = ((unsigned long long)u[m-1] >> (26-s))&mask;
   for (i = m - 1; i > 0; i--)
      un[i] = ((u[i] << s)&mask) | (((unsigned long long)u[i-1] >> (26-s))&mask);
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
         k = ((p >> 26)) - ((t >> 26));
      }
      t = un[j+n] - k;
      un[j+n] = t;
 
      q[j] = qhat;        
      if (t < 0) {           
         q[j] = q[j] - 1;      
         k = 0;
         for (i = 0; i < n; i++) {
            t = (unsigned long long)un[i+j] + vn[i] + k;
            un[i+j] = t&mask;
            k = (t >> 26);
         }
         un[j+n] = un[j+n] + k;
      }
   } 
   if (r != NULL) {
      for (i = 0; i < n-1; i++)
         r[i] = ((un[i] >> s)&mask) | (((unsigned long long)un[i+1] << (26-s))&mask);
      r[n-1] = (un[n-1] >> s)&mask;
   }
   return 0;
}

 
int main() {
   unsigned int D[32];
   unsigned int N[32];
   unsigned int Q[32];
   unsigned int R[32];
   setZero(D, 32);
   setZero(N, 32);
   setZero(Q, 32);
   setZero(Q, 32);
   divmnu(unsigned q[], unsigned r[], const unsigned u[], const unsigned v[], int m, int n);

 
   printf("%d errors out of %d cases; there should be 3.\n", errors, ncases);
   return 0;
}
