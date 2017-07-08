#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    if(argc != 2) {
        printf("Usage: ./caesar k\n");
        return 1;
    }
    else
    {
        //convert string to int
        int converted = atoi(argv[1]);      
        int key = 0;
        if(converted < 0) {
            printf("Key must be a non-negative integer.\n");
            return 1;
        }
        else if(converted >= 26){
            //in order to stay in the range of 0 to 26
            key = converted - 26;
        } else {
            key = converted;
        }
        printf("plaintext: ");
        string text = get_string();
        printf("ciphertext: ");
        for(int i = 0, n = strlen(text); i < n; i++)
        {
            //in case it is not an alphabetical character it should not be changed
            if(isalpha(text[i]) == false){
                printf("%c", text[i]);
                continue;
            }
            //get the alphabetical index in order to use the below formula
            int alphabeticalIndex = toupper(text[i]) - 65;
            int newAlphabeticalIndex = (alphabeticalIndex + key) % 26;
            //convert back to ascii by adding 65 
            int asciiIndex = newAlphabeticalIndex + 65;
            
            //the case should be preserved
            if(isupper(text[i])){
                printf("%c", toupper(asciiIndex));
            } else{
                printf("%c", tolower(asciiIndex));
            }
        }
        printf("\n");
    }
}