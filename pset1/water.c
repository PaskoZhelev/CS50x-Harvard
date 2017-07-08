#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int bottlesPerMinute = 12;
    int minutes = 0;
    do {
        printf("Minutes: ");
        minutes = get_int(); 
    } while (minutes <= 0);
    
    printf("Bottles: %i\n", minutes * bottlesPerMinute);
}