#define min(a, b) ((a) > (b)? (b) : (a))
// reference to the global vars in setup.c
extern int gMode;
extern int gIsFirst;
extern char gPlayer1[];
extern char gPlayer2[];
extern int gPileSize;

// type for player
typedef struct player {
    char *name;
    // choose the number of chips, and change the draw limits for the next player
    int (*play)(struct player *, int, int);
    struct player *next;
} player_t;

/*
 * Game setup for game mode and number of piles;
 * return 1 if set up succeeded.
 */
int setup_game(void);
void start_game(void);
int computer_play(struct player *, int, int);

/*
 * handy functions to read the yes/no answer from user's input,
 * return 1 on YES, 0 on NO
 */
int get_yes_no(void);

/*
 * return 1 for a successful input number, 0 for failure
 * number is stored in num
 */
int read_input_number(int *num);
