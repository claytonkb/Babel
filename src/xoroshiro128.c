// xoroshiro128.c

#include "xoroshiro128.h"

/*  Written in 2016 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* This is the successor to xorshift128+. It is the fastest full-period
   generator passing BigCrush without systematic failures, but due to the
   relatively short period it is acceptable only for applications with a
   mild amount of parallelism; otherwise, use a xorshift1024* generator.

   Beside passing BigCrush, this generator passes the PractRand test suite
   up to (and included) 16TB, with the exception of binary rank tests,
   which fail due to the lowest bit being an LFSR; all other bits pass all
   tests.
   
   Note that the generator uses a simulated rotate operation, which most C
   compilers will turn into a single instruction. In Java, you can use
   Long.rotateLeft(). In languages that do not make low-level rotation
   instructions accessible xorshift128+ could be faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill xoroshiro128_s. */


//
//
inline uint64_t xoroshiro128_rotl(const uint64_t x, int k) {

    return (x << k) | (x >> (64 - k));

}


//
//
uint64_t xoroshiro128_next(void) {

    const uint64_t s0 = xoroshiro128_s[0];
    uint64_t s1 = xoroshiro128_s[1];
    const uint64_t result = s0 + s1;

    s1 ^= s0;
    xoroshiro128_s[0] = xoroshiro128_rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
    xoroshiro128_s[1] = xoroshiro128_rotl(s1, 36); // c

    return result;
}


/* This is the xoroshiro128_jump function for the generator. It is equivalent
   to 2^64 calls to xoroshiro128_next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */
void xoroshiro128_jump(void) {

    static const uint64_t JUMP[] = { 0xbeac0467eba5facb, 0xd86b048b86aa9922 };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    int i,b;

    for(i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
        for(b = 0; b < 64; b++) {
            if (JUMP[i] & 1ULL << b) {
                s0 ^= xoroshiro128_s[0];
                s1 ^= xoroshiro128_s[1];
            }
            xoroshiro128_next();
        }

    xoroshiro128_s[0] = s0;
    xoroshiro128_s[1] = s1;

}


