#include <stdio.h>
/*
 * using ASCII's feature of continual encoding of 'a'-'z'
 */
#define ELEMSIZE    26
#define PAREN       -1

#define swap(a, b)          \
    do {                    \
        int tmp = a;        \
        a = b;              \
        b = tmp;            \
    } while(0)

void init_t(int array[])
{
    for(i = 0; i < ELEMSIZE; i++) {
        array[i] = i;
    }
}


int convert2cycle(int array[], char *output);
/*
 * implementation from Algorithm B of Multiply permutation in cycle form:
 * work backward from right to left; only difference is we set Z <- -1 while
 * encountering ')';
 */
int multipermB(const char *cycle, char *output)
{
    int T[ELEMSIZE];
    int i, j, k;
    int Z;
    char c;
    init_t(T);

    if(!cycle || !output) {
        fprintf(stderr, "error: either cycle form or output buffer is NULL!\n");
        return -1;
    }

    for(k = strlen(cycle); k > 0; k--) {
        c = cycle[k-1];
        if(c == ')')
            Z = PAREN;
        else if(c == '(')
            T[j] = Z;
        else {
            i = c - 'a';
            swap(Z, T[i]);
            if(T[i] == PAREN)
                j = i;
        }
    }

    return convert2cycle(T, output);
}

int find_untagged(int i, int *tagged)
{
    for(;i < ELEMSIZE; i++)
        if(!tagged[i])
            break;
    return i;
}
int convert2cycle(int array[], char *output)
{
    int *tagged;
    int outind, i, start;

    tagged = calloc(ELEMSIZE, sizeof(int));
    i = outind = 0;
    while((i = find_untagged(i, tagged)) < ELEMSIZE) {
        tagged[i] = 1;
        if(array[i] != i) {
            output[outind++] = '(';
            start = i;
            do {
                tagged[i] = 1;
                output[outind++] = 'a' + i;
                i = array[i];
            } while(start != i);

            output[outind++] = ')';
        }
    }
    output[outind] = '\0';
    return outind;
}
