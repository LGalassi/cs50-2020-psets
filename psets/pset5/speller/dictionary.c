// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 64000;

// Hash table
node *table[N];

// Count for the size function
int size_counter[1] = {0};

// free_list prototype
void free_list(node *np);

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Temporary variable for hashing
    char tmp[LENGTH + 1];
    strcpy(tmp, word);

    // Converts the word to lowercase
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (isalpha(word[i]))
        {
            tmp[i] = tolower(word[i]);
        }
    }

    int i = hash(tmp);

    node *cursor = table[i];

    // Searches for the word
    if (cursor == NULL)
    {
        return false;
    }
    else
    {
        while (cursor->next != NULL)
        {
            if (strcasecmp(word, cursor->word) == 0)
            {
                return true;
            }
            else
            {
                cursor = cursor->next;
            }
        }

        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash function djb2
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    // Tries to open the dictionary
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        return false;
    }

    char *word = malloc((LENGTH + 1) * sizeof(char));

    // Adds the words to the hash table
    for (int i = fscanf(f, "%s", word); i != EOF; i = fscanf(f, "%s", word))
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }

        strcpy(new_node->word, word);

        int n = hash(word);

        if (table[n] == NULL)
        {
            table[n] = new_node;
            new_node->next = NULL;
        }
        else
        {
            new_node->next = table[n];
            table[n] = new_node;
        }

        // Updates the counter for the size function
        size_counter[0]++;
    }

    free(word);
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return size_counter[0];
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    int free_count = 0;

    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            free_count++;
        }
        else
        {
            free_list(table[i]);
            free_count++;
        }
    }
    // Checks if the unload was successful
    if (free_count == N)
    {
        return true;
    }

    return false;
}

// Function for freeing the nodes
void free_list(node *np)
{
    if (np->next == NULL)
    {
        free(np);
    }
    else
    {
        free_list(np->next);
        free(np);
    }

    return;
}