#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

struct Performance *newPerformance() //creates a new Performance struct, initializes and returns it
{
    struct Performance *per = malloc(sizeof(struct Performance)); //mallocs enough memory for the size of an performance struct

    if (per == NULL) //if the malloc fails.
    {
        fprintf(stderr, "Error: Malloc failed.\n");
        exit(0);
    }

    per->frees = 0; //initializes the number of frees to 0
    per->mallocs = 0; //initializes the number of mallocs to 0
    per->reads = 0; //initializes the number of reads to 0
    per->writes = 0; //initializes the number of writes to 0

    return(per);
}

struct HashTable *createTable(struct Performance *performance, unsigned int capacity, int (*hash)(void *, int), int (*compar)(const void *,const void *))
{
    struct HashTable *table = malloc(sizeof(struct HashTable)); //mallocs enough memory for a hash table
    table->data = malloc(sizeof(void *) * capacity); //mallocs enough memory for maximum amount of void * elements in the table

    if (table == NULL || table->data == NULL) //if the malloc fails.
    {
        fprintf(stderr, "Error: Malloc failed.\n");
        exit(0);
    }

    table->capacity = capacity; //sets the tables max capacity
    table->hash = hash; //sets the hash function
    table->compar = compar; //sets the comparing function
    table->nel = 0; //initializes the table length to 0

    for(int i = 0; i < capacity; i++) //runs throught the table
    {
        table->data[i] = NULL; //sets every bucket in the table to NULL
        performance->reads++;
    }

    performance->mallocs++;

    return(table);
}

void addElement(struct Performance *performance, struct HashTable *table, void *src)
{
    if(table->nel == table->capacity) //if the table is full, prints error message
    {
        fprintf(stderr, "Error: Table Full.\n");
        exit(0);
    }
    int i = table->hash(src,table->capacity); //calculates the initial index using the hash function
    int num = 1; //used for looping
    while(num)
    {
        if(i == table->capacity) //if the index reaches the end of the table
        {
            i = 0; //go to the beginning
        }
        if(table->data[i] == NULL) //once the empty spot is found
        {
            table->data[i] = src; //store the data in the table
            table->nel++; //increment table length (number of items in table)
            performance->writes++;
            num = 0; //end the loop
        }
        else
        {
            i++; //if the spot is not empty, move to the next
        }
        if(num!=0) //if the loop is ending
            performance->reads++; //don't increment reads again
        
    }
}

int getIdx(struct Performance *performance, struct HashTable *table, void *src)
{
    int i = table->hash(src,table->capacity); //calculates the initial index using the hash function
    int init = i; //store the initial index that the hash function calculated
    int itr = 0; //used to see if jumped to top of table
    int num = 1;
    while(num)
    {
        if(i == table->capacity) //if the index is past the final bucket in the hash table
        {
            i = 0;
            itr++; //increment to show that there was a jump back to the beginning
        }
        if(table->data[i] != NULL)
        {
            if(table->compar(table->data[i],src) == 0) //if the two data values match
            {
                performance->reads++;
                return i; //return the index it was found on
            }
            else if(i == init && itr > 0) //if the index is the initial index and a jump to the beginning occured, then the whole table has already been searched
            {
                return -1; //return -1 to signify that it was not found
            }
            performance->reads++;
        }
        i++; //increment index
    }
}

void freeTable(struct Performance *performance, struct HashTable *table)
{
    free(table->data); //free the memory allocated for the data in the table
    free(table); //free the table itself
    performance->frees++;
}

void *getElement(struct Performance *performance, struct HashTable *table, void *src)
{
    int index = getIdx(performance,table,src); //get the index of the element
    if(index == -1) //if it is not found
        return NULL; //return NULL
    else
        return table->data[index]; //else return the found data
}

void removeElement(struct Performance *performance, struct HashTable *table, void *target)
{
    int index = getIdx(performance,table,target); //get the index of the element
    if(index != -1) //if the element is in the table
    {
        table->data[index] = NULL; //remove the element
        table->nel--; //decrement the number of elements in the hash table
        performance->writes++;
    }
}

int hashAccuracy(struct HashTable *table)
{
    struct Performance *per = newPerformance(); //temporary performance struct to call functions
    int accuracy = 0;
    int index1 = 0;
    int index2 = 0;
    for(int i = 0; i < table->capacity; i++) //runs through the whole hash tab;e
    {
        if(table->data[i]!=NULL) //if there is data at the current index
        {
            index1 = table->hash(table->data[i],table->capacity); //set index1 to the hash index
            index2 = getIdx(per,table,table->data[i]); //set index2 to the actual index
            if(index1 > index2) //if the hash index is greater than the actual index
            {
                accuracy += (table->capacity - index1 + index2); //add capacity - hash index + index to accuracy
            }
            else if(index1 < index2)
            {
                accuracy += (index2 - index1); //add actual index minus hash index to accuracy
            }
        }
    }
    free(per); //frees the temporary performance struct
    return(accuracy);
}

void rehash(struct HashTable *table) 
{
    struct Performance *per = newPerformance(); //temporary performance struct to call functions
    void **data = malloc(sizeof(void *) * table->nel); //mallocs enough memory for all the elements in the table
    int j = 0; //used to iterate through the list of data
    int x = 0; //used to move through the hash table
    
    while (x < table->capacity) //runs through the whole hash table
    {
        if(table->data[x]!=NULL) //if the current index has data
        {
            data[j] = table->data[x]; //store the data from the hash table in the list of data
            removeElement(per,table,table->data[x]); //remove the element from the hash table
            j++; //increment data list index
        }
        x++; //increment hash table index
    }

    for(int i = 0; i < j; i++) //runs through enough times for all the elements in the data list
    {
        addElement(per,table,data[i]); //adds elements one by one to the now empty hash table in order to rehash it
    }
    free(data); //frees the allocated list of data
    free(per); //frees the temporary performance struct
}