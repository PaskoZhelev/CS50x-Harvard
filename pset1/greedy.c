#include <cs50.h>
#include <stdio.h>
#include <math.h>

int coinsNeeded(int money);

int main(void)
{
    float owingChangeFloat = 0;
    printf("O hai! ");
    do {
       printf("How much change is owed?\n");
       owingChangeFloat = get_float();

    } while (owingChangeFloat < 0);

    owingChangeFloat *= 100.0;
    int owingChangeInt = (int) round(owingChangeFloat);
    
    printf("%i\n", coinsNeeded(owingChangeInt));
}

int coinsNeeded(int money)
{
    int neededCoins = 0;
    while(money > 0) {
        if(money >= 25) {
            money -= 25;
            neededCoins++;
        } else if (money >= 10) {
            money -= 10;
            neededCoins++;
        } else if (money >= 5) {
            money -= 5;
            neededCoins++;
        } else if (money >= 1) {
            money -= 1;
            neededCoins++;
        } else {
            break;
        }
    }
    
    return neededCoins;
}