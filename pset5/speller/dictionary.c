/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

//define node struct
typedef struct node
{
    bool is_word;
    struct node *children[27];
}node;

struct node *root;
unsigned int wordCount = 0;
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
        node *temp = root;      //temporary node
        int index = 0;
        
        for(int i = 0 ; word[i] != '\0' ; i++)
        {
            index = (int) tolower(word[i]) - 97;
            
            if (!(index >= 0 && index <= 25))
            {
                index = 26;     //apostrophe
            }
            
            temp = temp -> children[index];
            if (temp == NULL) {
                return false;
            }
        }
        return temp -> is_word;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    char word[45];
    
    FILE *dict = fopen(dictionary, "r");
    root = malloc(sizeof(node));
    
    int index = 0;
    
    while(fscanf(dict, "%s", word) != EOF)
    {
        node *temp = root;      //temporary node
        
        for(int i = 0 ; word[i] != '\0' ; i++)
        {
            index = (int) tolower(word[i]) - 97;
            
            if (!(index >= 0 && index <= 25))
            {
                index = 26;     //apostrophe
            }
            
            if (temp -> children[index] != NULL) {
                temp = temp -> children[index];
            }
            else {
                temp -> children[index] = malloc(sizeof(node));
                temp = temp -> children[index];
            }
        }
        
        temp -> is_word = true;
        wordCount++;
    }
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    
    return wordCount;
}


void unloadAdditional(node *temp)
{
    for (int i = 0; i < 26; i++)
    {
        if (temp -> children[i] != NULL)
        {
            unloadAdditional(temp -> children[i]);
        }
        
    }
    free(temp);
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    unloadAdditional(root);
    return true;
}

