#include <stdio.h>
#include "panel.h"

int main()
{
restart:
    setup_game();
    start_game();

    printf("\n Do you wish to play again?\n"
            ">>> ");
    if(get_yes_no())
        goto restart;
    else {
        printf("Thank you for playing our game, see u next time!\n");
        return 0;
    }
}
