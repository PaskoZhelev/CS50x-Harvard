#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("bool is %lu bytes\n", sizeof(bool));
    printf("char is %lu bytes\n", sizeof(char));
    printf("int is %lu bytes\n", sizeof(int));
    printf("float is %lu bytes\n", sizeof(float));
    printf("double is %lu bytes\n", sizeof(double));
    printf("long long is %lu bytes\n", sizeof(long long));
    printf("string is %lu bytes\n", sizeof(string));
    
}