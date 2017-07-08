#include <cs50.h>
#include <stdio.h>
#include <string.h>


int main(void)
{
    printf("s is: ");
    char *s = get_string();
    printf("t is: ");
    char *t = get_string();
    
    if (s != NULL && t != NULL)
    {
        if(strcmp(s, t) == 0)
        {
            printf("same\n");
        } else {
            printf("different\n");
        }
    }
}