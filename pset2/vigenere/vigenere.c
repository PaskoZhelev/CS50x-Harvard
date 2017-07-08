#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    if(argc != 2)  {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    else
    {
        
        string key = argv[1];
        for(int i = 0, n = strlen(key); i < n; i++)
        {
            if(!isalpha(key[i])) {
              printf("Usage: ./vigenere k\n");
            return 1;  
            }
        }
        
        printf("plaintext: ");
        string text = get_string();
        printf("ciphertext: ");
        int currentKeyLetterIndex = 0;
        int keyLength = strlen(key);
        for(int i = 0, n = strlen(text); i < n; i++)
        {
            //in case it is not an alphabetical character it should not be changed
            if(isalpha(text[i]) == false){
                printf("%c", text[i]);
                continue;
            }
            
            //extract the current letter from the key and convert it to alphabetical index
            int changingKey = toupper(key[currentKeyLetterIndex]) - 65;
            currentKeyLetterIndex++;
            if (currentKeyLetterIndex == keyLength)
            {
                //start from the beginning of the key again
                currentKeyLetterIndex = 0;
            }
            //get the alphabetical index of the plaintext letter in order to use the below formula
            int alphabeticalIndex = toupper(text[i]) - 65;
            int newAlphabeticalIndex = (alphabeticalIndex + changingKey) % 26;
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