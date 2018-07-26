#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "panel.h"

/*
 * AI chooses the number of piles according to Fibonacci number system,
 * generally, each remaining number N can be represented as:
 * N = Fk1 + Fk2 + ... + Fkr, where k1 >> k2 >> ... >> kr >> 0,
 * (k1 >> k2, means k1 >= k2+2)
 * 1)   if N is not a Fibonacci number, we tend to choose Fkr if Fkr <= limit, 
 *      in this way, we will surely win out;
 * 2)   if N is a Fibonacci number, then we don't have an absolute grasp of winning,
 *      we would choose randomly from 1 ~ max((N+2)/3-1), 1), any time the other player
 *      fails to fetch the Fkr, we may have the chance to win back;
 * 3)   if N is a NOT a Fibonacci number, but Fkr > limit, which often occurs when AI
 *      is the second player, still we adopt the previous method.
 *
 * In order to generate Fibonacci number series, we would allocate a hash map of Fibonacci
 * numbers for reference, and adopting binary search to generate Fk1, Fk2, ..., until we
 * calculate Fkr. Up until now, I don't have a more efficient way to directly calculate Fkr
 * without calculating Fk1 to Fk(r-1).
 */

// F45 = 1836311903, F46 = 2971215073 > INT_MAX
#define F_SIZE  45

static int Fn[F_SIZE];
static int isInited = 0;

int init_fibonacci(void);
int generate_kr(int number);
int binsearch(int value, int low, int high);

int computer_play(struct player *player, int limit, int remain)
{
    int kr, range_max, number;
    // when the limit reaches the remain, we win
    if (limit >= remain) {
        number = remain;
        goto done;
    }
    if(!isInited) {
        init_fibonacci();
        isInited = 1;
    }

    kr = generate_kr(remain);
    if(remain == Fn[kr] || limit < Fn[kr]) {
        range_max = min(limit, (remain+2)/3-1);
        // set the random seed
        srand(time(NULL));
        number = (rand() * (range_max-1))/RAND_MAX+1;
    } else // we surely will win out
        number = Fn[kr];

done:
#if 0
    sleep(1);
#endif
    printf("%d\n", number);
    return number;
}

int generate_kr(int total)
{
    int k, remain = total;
    // if F45 < total <= INT_MAX
    if(total >= Fn[F_SIZE-1]) {
        remain = total - Fn[F_SIZE-1];
    }

    k = F_SIZE-1;
    for(;;) {
        if(remain == 0)
            return k;
        k = binsearch(remain, 1, k);
        remain -= Fn[k];
    }
}

// return k, so that F(k) <= value < F(k+1)
int binsearch(int value, int low, int high)
{
    int mid = (low+high)/2;

    if(mid == low || value == Fn[mid])
        return mid;
    else if(value > Fn[mid])
        return binsearch(value, mid, high);
    else
        return binsearch(value, low, mid);
}

int init_fibonacci(void)
{
    int i;

    Fn[0] = 1, Fn[1] = 1;
    for(i = 2; i < F_SIZE; i++) {
        Fn[i] = Fn[i-1] + Fn[i-2];
    }

    return 0;
}

// test main
#if 0
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    int value, i;
    init_fibonacci();
    for(i = 0; i < F_SIZE; i++) {
        printf("F(%d) = %d\n", i+1, Fn[i]);
    }
    if(argc == 2) {
        value = atoi(argv[1]);
        printf("%d\n", Fn[generate_kr(value)]);
    }

    return 0;
}
#endif
