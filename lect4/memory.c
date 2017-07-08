#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void f(void)
{
    int *x = malloc(10*sizeof(int));
    x[9] = 0;
    free(x);
}

int main(void)
{
    f();
    return 0;
}

