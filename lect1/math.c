#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("x is ");
    int x = get_int();
    printf("y is ");
    int y = get_int();
    
    printf("The sum of %i and %i is %i\n", x, y, x + y);
    printf("%i minus %i is %i\n", x, y, x - y);
    printf("%i divided by %i is %i\n", x, y, x / y);
    printf("%i multiplied by %i is %i\n", x, y, x * y);
    printf("The remainder of %i divided by %i is %i\n", x, y, x % y);
    
}