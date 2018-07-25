#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "panel.h"

#define MAXNAME 20

int gMode;
int gIsFirst;
char gPlayer1[MAXNAME+1];
char gPlayer2[MAXNAME+1];
int gPileSize;

static void get_player_name(char *player, int size);
static int get_pile_size(void);

int setup_game(void)
{
    int mode = 1, is_first = 1;
    char *player1 =  gPlayer1, *player2 = gPlayer2;
    printf(">>>>>>>>>>>>>> Choose Game Mode <<<<<<<<<<<<<<\n"
            "1. Default mode (player vs. computer)\n"
            "2. Two players\n"
            ">>> ");
    for(;;) {
        if(read_input_number(&mode) == 1) {
            switch(mode) {
                case 1:
                    printf("What's name plz? (<= %d characters accepted)\n", MAXNAME);
                    get_player_name(player1, MAXNAME);
                    printf("Hello, %s, choose the number of piles to play (>= 1):\n"
                            ">>> ", player1);
                    gPileSize = get_pile_size();
                    printf("Do you want to play first? [Y/N]\n"
                            ">>> ");
                    gIsFirst = get_yes_no();
                    if(gIsFirst) {
                        strcpy(player2, "Computer");
                    } else {
                        strcpy(player2, player1);
                        strcpy(player1, "Computer");
                    }
                    gMode = mode;
                    goto setup;

                case 2:
                    printf("What's the name of the first player?\n");
                    get_player_name(player1, MAXNAME);
                    printf("What's the name of the second player?\n");
                    get_player_name(player2, MAXNAME);
                    printf("Hello, %s and %s, choose the number of piles to play (>= 1):\n"
                            ">>> ", player1, player2);
                    gPileSize = get_pile_size();
                    gMode = mode;
                    goto setup;
                default:
                    break;
            }
        }
        printf("Please choose [1/2]:\n"
                ">>> ");
    }

setup:
    printf("Game setup succeeded!\n");
    switch(mode) {
        case 1:
            printf("Game mode = %d\n"
                    "%s vs. computer, and %s goes first\n"
                    "Pile size: %d\n", gMode, gPlayer1, (gIsFirst)? "gPlayer1": "computer", gPileSize);
            break;
        case 2:
            printf("Game mode = %d\n"
                    "%s vs. %s, and %s goes first\n"
                    "Pile size: %d\n", gMode, gPlayer1, gPlayer2, gPlayer1, gPileSize);
            break;
        default:
            break;
    }
    return 0;
}

int get_yes_no(void)
{
    char *line = NULL;
    size_t n = 0;
    ssize_t read;
    int valid = 0; // a valid input

    for(;;) {
        if((read = getline(&line, &n, stdin)) != -1) {
            line[read-1] = '\0';
            if(!strcasecmp(line, "yes") || !strcasecmp(line, "y"))
                return 1;
            else if(!strcasecmp(line, "no") || !strcasecmp(line, "n"))
                return 0;
        } else {
            perror("getline");
        }

        printf("Not a valid input, plz answer with [Y/YES] for yes, [N/NO] for no\n"
                ">>> ");
        free(line);
        line = NULL;
        n = 0;
    }
}

/*
 * return 1 for a successful input number, 0 for failure
 * number is stored in num
 */
int read_input_number(int *num)
{
    char *line = NULL;
    char *endptr;
    size_t n = 0;
    ssize_t read;
    long int number;

    if((read = getline(&line, &n, stdin)) != -1) {
        line[read-1] = '\0'; // remove trailing newline
        errno = 0;
        number = strtol(line, &endptr, 10);
        // check for some possible errors, including strtol() function error
        // and the validity of given number
        if((errno == ERANGE && (number == LONG_MAX || number == LONG_MIN))
                || (errno != 0 && number == 0)
                || (endptr == line)
                || (number > INT_MAX || number < INT_MIN)) {
            if(errno != 0)
                perror("strtol");
            if(number > INT_MAX || number < INT_MIN)
                printf("provided number out of range, which is [%d~%d]\n", INT_MIN, INT_MAX);
            goto fail;
        }
        if(*endptr != '\0') {
            // skip the following spaces
            while(isspace(*endptr))
                endptr++;
            // if there is any further chars rather than spaces, fail it
            if(*endptr != '\0')
                goto fail;
        }

        // succeeds in reading a valid number
        *num = (int)number;
        free(line);
        return 1;
    } else {
        perror("getline");
        free(line);
        goto fail;
    }

fail:
    free(line);
    return 0;
}

static int get_pile_size(void)
{
    int n = 0;
    for(;;) {
        if(read_input_number(&n) == 1 && n > 1) {
            break;
        }
        printf("!!! Pile size unaccepted !!!\n"
                "Choose another legal positive number:\n"
                ">>> ");
    }

    return n;
}

static void get_player_name(char *player, int size)
{
    int n, c;

    for(;;) {
        printf(">>> ");
        // skip starting spaces
        while(isspace((c = getchar())) && c != '\n')
            ;
        if(c == '\n')
            continue;
        // here we find a valid name
        n = 0;
        for(;;) {
            do {
                player[n++] = c;
            } while(isgraph((c = getchar())) && n < size);

            if(c == '\n' || n >= size) {
                player[n] = '\0';
                goto done;
            }

            // add a space, but skip the rest
            player[n++] = c;
            while(isspace((c = getchar())) && c != '\n')
                ;
            // but if the name limit or a newline reaches, give up the space and return
            if(c == '\n' || n >= size) {
                player[n-1] = '\0';
                goto done;
            }
        }
    }

done:
    // consume the rest charactors, otherwise it would affect the future input
    if(c != '\n') {
        while((c = getchar()) != '\n')
            ;
    }

    return;
}

/* test main
int main()
{
    setup_game();
    return 0;
}
*/
