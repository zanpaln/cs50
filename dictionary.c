/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"
#define TABLESIZE 27

typedef struct node
{
    char* word;
    struct node* next;
} node;

int words = 0;
char word[LENGTH + 1];
node* hashtable[TABLESIZE];

int hash(const char* word)
{
    int idx = 0;
    
    for(int i = 0; word[i] != '\0'; i++)
        idx += tolower(word[i]);
        
    return idx % TABLESIZE;
}


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    node* check_node = malloc(sizeof(node));
    int dtm = hash(word);
    check_node = hashtable[dtm];
    
    while(check_node != NULL)
    {
        if(strcasecmp(check_node->word, word) == 0)
            return true;
        check_node = check_node->next;
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    FILE* dct = fopen(dictionary, "r");
    
    if (dct == NULL)
    {
        printf("Could not open %s.\n", dictionary);
    	return false;
    }
    
    while(fscanf(dct, "%s\n", word) != EOF)
    {
        node* new_node = malloc(sizeof(node));
        new_node->word = malloc(strlen(word) + 1);
        strcpy(new_node->word, word);
        int idx = hash(word);
        if(hashtable[idx] == NULL)
        {
            hashtable[idx] = new_node;
            new_node->next = NULL;
        }
        else
        {
            new_node->next = hashtable[idx];
            hashtable[idx] = new_node;
        }
        
        words++;
        fclose(dct);
    }
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for(int i = 0; i < TABLESIZE; i++)
    {
        node* index;
        
        index = hashtable[i];
        
        while(index)
        {
            node* tmp = index;
            index = index->next;
            free(tmp);
            return true;
        }
        
        hashtable[i] = NULL;
    }
    return false;
}
