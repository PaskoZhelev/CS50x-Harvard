#include <stdio.h>

int main(void)
{
    int n = 65 + 26;
    for(int i = 65; i < n; i++) 
    {
        printf("%c is %i\n", i, i);
    }
}