#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string input = get_string();
    if (input != NULL)
    {
        bool foundFirstChar = false;
        for(int i = 0, n = strlen(input); i < n; i++)
        {
            if((foundFirstChar == false) && (input[i] == ' '))
            {
                continue;
            } else {
                if (input[i] == ' '){
                    foundFirstChar = false;
                } else if ((foundFirstChar == false) && (input[i] != ' ')){
                    foundFirstChar = true;
                    printf("%c", toupper(input[i]));
                } else {
                    continue;
                }
            }
        }
        printf("\n");
    }
}