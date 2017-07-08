#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n = 0;
    do {
       printf("Height: ");
       n = get_int(); 
    } while (n < 0 || n > 23);
     
    for(int i = n; i > 0; i--)
    {
        for(int a = 0; a < i - 1; a++)
        {
            printf(" ");
        }
        
        for(int b = i; b < n + 1; b++)
        {
            printf("#");
        }
        printf("#\n");
    }
    
}