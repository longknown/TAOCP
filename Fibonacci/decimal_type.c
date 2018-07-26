#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE    (408/2)
#define LIMIT       100
#define STEP        1000

// every char stores 2-digits of decimal, 01~99
typedef unsigned char word_t[MAX_SIZE];

int add(word_t desc, word_t adder)
{
    unsigned char *c1, *c2, scale = 0;

    for(c1 = &desc[0], c2 = &adder[0]; c1 < desc+MAX_SIZE; c1++, c2++) {
        if(LIMIT-*c1-scale > *c2) {
            *c1 += *c2 + scale;
            scale = 0;
        } else {
            *c1 += *c2 + scale;
            scale = 1;
            // only reserve the lower 2 decimal digits
            *c1 %= LIMIT;
        }
    }

    return (int)scale;
}

int printd(word_t w)
{
    unsigned char *c = (unsigned char *)w + MAX_SIZE - 1;
    int valid = 0;

    while(c >= w) {
        if(*c > 0 || valid > 0) {
            valid  = 1;
            printf("%02d", *c);
        }
        c--;
    }
}

int main(int argc, char **argv)
{
    int i;
    int step = STEP, tmp;

    if(argc == 2) {
        if((tmp = atoi(argv[1])) > 0)
            step = tmp;
        else {
            printf("error: step number invalid!\n");
            exit(1);
        }
    }
    word_t Fa, Fb, temp;
    memset(Fa, 0, sizeof(word_t));
    memset(Fb, 0, sizeof(word_t));
    memset(temp, 0, sizeof(word_t));
    // F(1) = 1
    *(unsigned *)Fb = 1;
    for(i = 0; i < step; i++) {
        printf("F(%d) = ", i+1);
        // temp = Fb
        memcpy(temp, Fb, sizeof(word_t));
        if(add(Fb, Fa)) {
            printf("\n ERROR: Overflow in step=%d!\n", i+1);
            return 1;
        } else {
            printd(Fb);
            printf("\n");
            memcpy(Fa, temp, sizeof(word_t));
        }
    }


    return 0;
}
