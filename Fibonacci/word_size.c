#include <limits.h>
#include <stdio.h>
#include <string.h>

// approximately, 2^1600, to cover 10^408 range
#define MAX_SIZE    50
#define STEP        1000

typedef unsigned int word_t[MAX_SIZE];

// return 0 if not overflowed, 1 otherwise
int add(word_t dest, word_t adder)
{
    unsigned int scale = 0;
    unsigned int *a, *b;

    for(a = &dest[0], b= &adder[0]; a < (unsigned int *)dest+MAX_SIZE; ++a, ++b) {
        // test scaling
        if(UINT_MAX-*a-scale > *b) {
            *a += *b+scale;
            scale = 0;
        }
        else {
            *a += *b+scale;
            scale = 1;
        }
    }

    return scale;
}

void printh(word_t word)
{
    int valid = 0;
    unsigned int *a = (unsigned int*)word+MAX_SIZE-1;
    printf("0X");
    while(a >= (unsigned int *)word) {
        if(*a > 0u || valid > 0) {
            valid = 1;
            printf("%08X", *a);
        }
        a--;
    }

}

// for test
int main()
{
    int i;
    word_t Fa, Fb, temp;
    memset(Fa, 0, sizeof(word_t));
    memset(Fb, 0, sizeof(word_t));
    memset(temp, 0, sizeof(word_t));
    // F(1) = 1
    *(unsigned *)Fb = 1;
    for(i = 0; i < STEP; i++) {
        printf("F(%d) = ", i+1);
        // temp = Fb
        memcpy(temp, Fb, sizeof(word_t));
        if(add(Fb, Fa)) {
            printf("\n ERROR: Overflow in step=%d!\n", i+1);
            return 1;
        } else {
            printh(Fb);
            printf("\n");
            memcpy(Fa, temp, sizeof(word_t));
        }
    }


    return 0;
}
