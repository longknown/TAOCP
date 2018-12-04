#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int multi_permA(char * const str, char *output);

int main(int argc, char *argv[])
{
    char *output, *str;
    int res;

    if(argc != 2) {
        printf("error input: plz input cycle form string.\n");
        exit(1);
    }

    str = strdup(argv[argc-1]);
    printf("Original Cycle form: %s\n", str);
    output = malloc(sizeof(char) * (1+strlen(str)));
    res = multi_permA(str, output);

    if(res > 0)
        printf("Transformed formular: %s\n", output);
    else
        printf("Transformation failed!\n");

    free(str);
    free(output);
    return 0;
}

/*
 * 1. tag all left parens;
 * 2. replace each right paren be a tagged copy of
 *      input symbol follows its matching right paren;
 */
void first_pass(char *str, int *tagged)
{
    char ch;
    int c;
    char *ptr = str;

    while(*ptr) {
        ptr = strchr(ptr, '(');
        if(!ptr)
            break;

        tagged[ptr-str] = 1;        // tag '('
        ch = *++ptr;

        ptr = strchr(ptr, ')');
        if(!ptr) {
            fprintf(stderr, "error: this is an ABNORMAL cycle form, parentheses is NOT closed!\n");
            exit(1);
        }
        tagged[ptr-str] = 1;
        *ptr++ = ch;
    }
}

/*
 * find the first untagged element, but won't tag it;
 */
int find_untagged(int index, int *tagged, int len)
{
    for(;index < len && tagged[index] == 1; index++)
        ;

    return index;
}

int rm_singleton(char *str)
{
    char *ptr, *pi, *pj;
    int k, size;

    ptr = malloc(strlen(str) + 1);
    k = 0;
    pi = pj = str;
    while((pi = strchr(pj, '('))) {
        pj = strchr(pi, ')');

        if(!pj) {
            fprintf(stderr, "error: this is an ABNORMAL cycle form, parentheses is NOT closed!\n");
            exit(1);
        }

        size = pj-pi+1;
        if(size > 3) {
            memcpy(ptr+k, pi, size);
            k += size;
        }
    }
    *(ptr+k) = '\0';
    strcpy(str, ptr);
    free(ptr);

    return k;
}

/*
int main()
{
    char *str = strdup("(bf)(c)");
    rm_singleton(str);

    printf("%s\n", str);
    return 0;
}
*/

int multi_permA(char* const str, char *output)
{
    int index, len = strlen(str);
    int start, current;
    start = current = 0;
    int out_ind = 0, i;
    char *ptr;
    // allocate an equal length of array for tagging
    int *tagged = calloc(len, sizeof(int));

    // the provided output buffer and string-to-process cannot be NULL
    if(str == NULL || output == NULL)
        return 0;

    first_pass(str, tagged);
    index = 1;          // starting from "([x]"
    while((index = find_untagged(index, tagged, len)) < len) {
        tagged[index] = 1;
        start = str[index];
        output[out_ind++] = '(';
        output[out_ind++] = start;

        for(current=str[index+1], i=index+2;; ) {
            ptr = strchr(str+i, current);
            if(ptr) {
                i = ptr-str;
                tagged[i++] = 1;
                current = str[i++];     // i now points to ptr+2
            } else if(current != start) {
                output[out_ind++] = current;
                i = index;              // starts from the left of the formula
            } else /*(if current == start)*/
                break;
        }
        output[out_ind++] = ')';
    }

    output[out_ind] = '\0';
    out_ind = rm_singleton(output);
    free(tagged);
    return out_ind;
}
