/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"

// Trie Structure
typedef struct _trie
{
    bool is_word;
    struct _trie *children[27];
} trie;

trie *root;

unsigned int counter = 0;
int loaded = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    int letter, i = 0;

    // Trivarsal NOde
    trie *triv = root;

    while (word[i] != '\0')
    {
        letter = tolower(word[i]) - 97;
        // Check for "\'"
        if (word[i] == '\'')
        {
                letter = 26;
        }

        if (triv->children[letter] == NULL)
        {
            return false;
        }
        else
        {
            triv = triv->children[letter];
        }
        i++;
    }
    //Check For Word Finally
    if (triv->is_word)
        return true;

    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    root = malloc(sizeof(trie));
    for (int i = 0; i < 27; i++)
        root->children[i] = NULL;

    root->is_word = false;
    // TODO
    int letter;

    //Trivarsal Node
    trie *triv;
    //Open Dictionary
    FILE *fp = fopen(dictionary, "r");

    //Check For File Opening
    if (fp == NULL)
    {
        loaded = 1;
        return false;
    }

    //Start Loading in Trie DS
    for (char c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        triv = root;
        while(c != '\n'){
            letter = c - 97;
            //Check For the Letter
            if (c == '\'')
            {
                letter = 26;
            }
            if (triv->children[letter] == NULL)
            {
                triv->children[letter] = malloc(sizeof(trie));
                for(int i = 0; i < 27; i++)
                    triv->children[letter]->children[i] = NULL;

                triv->children[letter]->is_word = false;
                //Assign Node
                triv = triv->children[letter];
            }
            else
            {
                triv = triv->children[letter];
            }

            c = fgetc(fp);
        }
        if(c == '\n')
        {
            triv->is_word = true;
            counter++;
        }
    }
    fclose(fp);
    if(triv->is_word == true) return true;
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if(loaded == 1)
    {
        return 0;
    }
    else
        return counter;
    return 0;
}

void freeNode(trie **ptr)
{
    trie *prev = *ptr;
    trie *next;

    for (int i = 0; i < 27; i++)
    {
        if(prev->children[i] != NULL){
            next = prev->children[i];
            freeNode(&next);
        }
    }

    free(prev);

}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{

    trie *prev = root;
    trie *next;

    for (int i = 0; i < 27; i++)
    {
        if (prev->children[i] != NULL)
        {
            next = prev->children[i];
            freeNode(&next);
        }
        else
            continue;
    }

    free(root);

    return true;
}
