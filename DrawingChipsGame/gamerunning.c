#include <stdio.h>
#include "panel.h"


static int sRemainPiles;
static int sDrawLimits;
static player_t *current_player;
int human_play(struct player *, int, int);

/*
 * referee, decide the turn and declare the game result
 * return 1 to continue, 0 for game over
 */
int referee(void);

void start_game(void)
{
    // init both players with human_play
    static player_t p1 = {
        .name = gPlayer1,
        .play = human_play,
    };
    static player_t p2 = {
        .name = gPlayer2,
        .play = human_play,
    };
    p1.next = &p2;
    p2.next = &p1;

    // correct according to settings
    if(gMode == 1) {
        if(gIsFirst)
            p2.play = computer_play;
        else
            p1.play = computer_play;
    } else if(gMode == 3) {
        p1.play = p2.play = computer_play;
    }

    sRemainPiles = gPileSize;
    sDrawLimits = gPileSize -1;

    current_player = &p1;
    printf("\n\n\nLet's start our game!!!\n\n\n");
    for(;referee();)
        ;
}

int referee(void)
{
    printf("Current game situation:\n"
            "\tRemaining piles: %d\n"
            "\tNext player: %s\n"
            "\tYou can draw [1 ~ %d] chips\n\n"
            , sRemainPiles, current_player->name, sDrawLimits);
    int draw_num;
    printf("%s. >>> ", current_player->name);
    draw_num = current_player->play(current_player, sDrawLimits, sRemainPiles);
    sRemainPiles -= draw_num;
    sDrawLimits = min(sRemainPiles, 2 * draw_num);
    if(sRemainPiles == 0) {
        printf("!!!!!!!!!! @%s WINs !!!!!!!!!!\n", current_player->name);
        return 0;
    } else { // switch the player
        current_player = current_player->next;
        return 1;
    }
}

int human_play(struct player *player, int limit, int __unused)
{
    int num;
    for(;;) {
        if(read_input_number(&num) && num > 0 && num <= limit) {
            return num;
        }
        printf("%s. >>> ", player->name);
    }
}
