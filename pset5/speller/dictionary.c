/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <strings.h>

#include "dictionary.h"

#define HASH_MAX 1000000
int dicSize = 0; //global variable to count dictionary size


//using a LINKED LIST-> array of linked list
typedef struct node
{
    char word[LENGTH +1]; // string +1 to /0 perhaps
    struct node *next;
}
node;

//alocate space in memory to store node
node *hashtable[HASH_MAX] = {NULL}; //pointer to place in memory where it is reserved. Array of linked lists, each element is a node*

// create hash function
int hash (const char* word) //taken from rishabhs95/harvard-CS50/pset5/dictionary.c
{
    int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // alphabet case
        if(isalpha(word[i]))
            n = word [i] - 'a' + 1;
        
        // comma case
        else
            n = 27;
        
        hash = ((hash << 3) + n) % HASH_MAX;
    }
    return hash;
}

bool load(const char *dictionary);
unsigned int size(void);
bool check(const char *word);
bool unload(void);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    //case insensive
    //assume strings passed in are alpha + '
    
    // creates a temp variable that stores a lower-cased version of the word
    char temp[LENGTH + 1];
    int len = strlen(word);
    for(int i = 0; i < len; i++)
        temp[i] = tolower(word[i]);
    temp[len] = '\0';
    
    //check the index that the world should be in
    int idx = hash(temp);
    
    //check if hashtable is empty, word not in dict
    if(hashtable[idx] == NULL)
    {
        return 0; //false
    }
    
    //creation of cursos to iterate over the link list
    node *cursor = hashtable[idx]; // pointer to pre-exitent nodes in the link list pointing to the header of the link list
    
    while(cursor!= NULL)
    {
        //compare the strings looking to check if its the word we are looking for
        if(strcasecmp(cursor->word, word) == 0) //strcasecmp(a,b) compares if lowercase string a is equal to lowercase string b
        {
            return 1; //found the word, suceess
        }
        cursor = cursor->next; //else move to next node
    }
    
    //word was not found
    return 0;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    
    if (file == NULL)
    {
        return 0; //return false if error
    }
    
    char word[LENGTH+1];
    
    //populate hastable with words drom dictionary. set value for nodes access word variable and set its value
    while(fscanf(file,"%s\n", word)!= EOF) //scan dictionary word by word. sarch into dictioary.txt look for string, put that string into word variable
    {
        dicSize++;//for every new word increment dictionary size
        
        // malloc a node * for each new word
        node *new_node = malloc(sizeof(node));
        if(new_node == NULL)//check if malloc succeed
        {
            unload(); //unload dictionary to quit speller
            return 0;
        }
        
        //if succeed copy word into node
        strcpy(new_node->word, word);
        
        //finds index for new node
        int index = hash(word);
        
        //if index is empty, insert
        
        if(hashtable[index] == NULL)
        {
            hashtable[index] = new_node;
            new_node->next = NULL;
        }
        else //append into link list, in the beggining
        {  //hashtable[index] is the head of the node
            new_node->next = hashtable[index]; //put next in the existent/old node
            hashtable[index] = new_node; //move head to new node
        }
    }
    //close file
    fclose(file);
    
    //return true if successful
    return 1;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if(dicSize == 0)
    {
        return 0; // didn't load
    }
    else
        return dicSize;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for(int i = 0; i<HASH_MAX;i++)
    {
        node *cursor = hashtable[i]; //node cursor pointing to the 1st node in the linked list
        while(cursor!= NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return 1;
}
