#include <string.h>

int multi_permA(char * const str, char *output);

int main(int argc, char *argv[])
{
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
int find_untagged(int index, int *tagged, int len);
{
    for(;index <= len && tagged[index] == 1; index++)
        ;

    return index;
}

int multi_permA(char* const str, char *output)
{
    int index = 1, len = strlen(str);
    int start, current;
    start = current = 0;
    int out_ind = 0, i;
    char *ptr;
    // allocate an equal length of array for tagging
    int *tagged = calloc(strlen(str), sizeof(int));

    // the provided output buffer and string-to-process cannot be NULL
    if(str == NULL || output == NULL)
        return 0;

    first_pass(str);
    while((index = find_untagged(index, tagged, len)) <= len) {
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
    free(tagged);
    return out_ind;
}
