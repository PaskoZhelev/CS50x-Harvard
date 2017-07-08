#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void swap(int *a, int *b);

int main(void)
{
    int x = 1;
    int y = 2;
    printf("x is %d\n", x);
    printf("y is %d\n", y);
    swap(&x, &y);
    printf("x is %d\n", x);
    printf("y is %d\n", y);
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}