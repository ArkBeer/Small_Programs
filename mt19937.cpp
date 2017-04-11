#include <stdio.h>
#include<iostream>
#include<array>
const int N = 624;
const int M = 397;
const unsigned int MATRIX_A = 0x9908b0dfUL;
const unsigned int UPPER_MASK = 0x80000000UL;
const unsigned int LOWER_MASK = 0x7fffffffUL;

//static unsigned long mt[N];
static std::array<unsigned long,N> mt;
static int mti=N+1;

void init_genrand(const unsigned long s){
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; ++mti) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        mt[mti] &= 0xffffffffUL;
    }
}

template <int Size>
void _init_by_array(std::array<unsigned long,Size>& init_key){
    int key_length=init_key.size();

    init_genrand(19650218UL);
    for (int k=1; k<= (N>key_length ? N : key_length); ++k) {
        int i=k%N!=0?k%N:1;
        int j=(k-1)%key_length;
        mt.at(i) = (mt.at(i) ^ ((mt.at(i-1) ^ (mt.at(i-1) >> 30)) * 1664525UL))
          + init_key.at(j) + j; /* non linear */
        mt.at(i) &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        if (i==N-1) { mt.at(0) = mt.at(N-1);}
    }
    for (int k=2; k<=N; ++k) {
        int i=k%N!=0?k%N:1;
        mt.at(i) = (mt.at(i) ^ ((mt.at(i-1) ^ (mt.at(i-1) >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt.at(i) &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        if (i>=N-1) { mt.at(0) = mt.at(N-1); }
    }

    mt.at(0) = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}
template<typename T>
void init_by_array(T& t){
    _init_by_array<t.size()>(t);
}
/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void)
{
    return (long)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void) 
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */

int main(void)
{
    int i;
    std::array<unsigned long,4> init={0x123, 0x234, 0x345, 0x456};
    init_by_array(init);
    printf("1000 outputs of genrand_int32()\n");
    for (i=0; i<1000; i++) {
      printf("%10lu ", genrand_int32());
      if (i%5==4) printf("\n");
    }
    printf("\n1000 outputs of genrand_real2()\n");
    for (i=0; i<1000; i++) {
      printf("%10.8f ", genrand_real2());
      if (i%5==4) printf("\n");
    }
    return 0;
}