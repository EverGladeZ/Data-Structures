#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

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

struct Array *newArray( struct Performance *performance, unsigned int width, unsigned int capacity) //creates a new Array struct, initializes and returns it
{
    struct Array *arr = malloc(sizeof(struct Array)); //mallocs enough memory for the size of an array struct

    if (arr == NULL) //if the malloc fails.
    {
        fprintf(stderr, "Error: Malloc failed.\n");
        exit(0);
    }

    arr->width = width; //initializes the width to the passed in width
    arr->capacity = capacity; //initializes the capacity to the passed in capacity
    arr->nel = 0; //initializes the starting array length to zero
    arr->data = malloc(width*capacity);  //mallocs enough memory for the max amount of data that can be stored in the array
    if (arr->data == NULL) //if malloc fails
    {
        fprintf(stderr, "Error: Malloc failed.\n");
        exit(0);
    }
    performance->mallocs++; //increase the amount of total mallocs by one

    return(arr);
}

void readItem(struct Performance *performance, struct Array *array, unsigned int index, void *dest) //reads in an item from a certain index
{
    if(index >= array->nel) //if the index is out of bounds
    {
        fprintf(stderr, "Error: Index greater than array length.\n");
        exit(0);        
    }

    memcpy(dest, (unsigned char *)array->data + (index * array->width), array->width); //uses pointer arithmetic to find the correct item and copies it over to the destination

    performance->reads++; //increases the amount of reads by one
}

void writeItem(struct Performance *performance, struct Array *array, unsigned int index, void *src) //writes an item to a certin index
{
    if(index > array->nel || index >= array->capacity) //if the index to be written at is out of bounds
    {
        fprintf(stderr, "Error: Index greater than array length/capacity.\n");
        exit(0);
    }

    memcpy((unsigned char *)array->data + (index * array->width), src, array->width); //uses pointer arithmetic to write the item to the proper index

    if(index == array->nel) //if it was written to the end of the array
        array->nel++; //increment the number that stores the length of the array

    performance->writes++; //increases the amount of writes by one
}

void contract(struct Performance *performance, struct Array *array) //contracts the array by one
{
    if(array->nel == 0)
    {
        fprintf(stderr, "Error: Array length is zero.\n");
        exit(0);
    }

    array->nel--; //reduces the length of the array by one

}

void freeArray(struct Performance *performance, struct Array *array) //frees all the malloc'd memory in the array struct
{
    free(array->data); //frees the data in the array
    free(array); //frees the array struct itself
    performance->frees++; //increases the amount of frees by one
}

void appendItem(struct Performance *performance, struct Array *array, void *src) //adds item to the end of the array
{
    writeItem(performance, array, array->nel, src); //uses the writeItem function to write to the end
}

void insertItem(struct Performance *performance, struct Array *array, unsigned int index, void *src) //inserts an item into the index of an array
{
    void *temp_ptr = malloc(array->width); //mallocs a temporary pointer
    int i;
    for(i = array->nel; i > index; i--) //goes through the array from the end to the index
    {
        readItem(performance, array, i-1, temp_ptr); //these two lines shift the items over by one
        writeItem(performance, array, i, temp_ptr);
    }
    free(temp_ptr); //frees the temporary pointer

    writeItem(performance, array, index, src); //writes the item to the index

}

void prependItem(struct Performance *performance, struct Array *array, void *src) //adds item to the beginning of an array
{
    insertItem(performance, array, 0, src); //uses the insert item function to add to front of the array (index 0)
}

void deleteItem(struct Performance *performance, struct Array *array, unsigned int index) //deletes an item at a given index
{
    void *temp_ptr = malloc(array->width); //mallocs a temporary pointer
    int i;
    for(i = index; i < array->nel - 1; i++) //goes through the array from index to the end
    {
        readItem(performance, array, i + 1, temp_ptr); //similar to insertItem's shifting except it shifts everything over to the left by one
        writeItem(performance, array, i, temp_ptr);
    }
    free(temp_ptr); //frees the temporary pointer
    contract(performance, array); //contracts to remove the wanted item
}

int findItem(struct Performance *performance, struct Array *array, int(*compar)(const void *, const void *), void *target) //finds a certain item in the array
{
    void *temp_ptr = malloc(array->width); //mallocs a temporary pointer
    int i;
    for(i = 0; i < array->nel; i++) //runs through the whole array
    {
        readItem(performance, array, i, temp_ptr); //reads in the item and stores in temp_ptr
        if(compar(temp_ptr, target) == 0) //if the item at the current index matches the target item
        {
            free(temp_ptr);//frees the temporary pointer
            return(i); //returns the index of the found item
        }
    }
    free(temp_ptr); //frees the temporary pointer
    return(-1); //returns -1 to show that the item wasn't found
}

int searchItem(struct Performance *performance, struct Array *array, int(*compar)(const void *, const void *), void *target)
{
    //uses a binary search in order to search the array for a certain item
    void *temp_ptr = malloc(array->width); //mallocs a temporary pointer
    unsigned int mid = 0;
    unsigned int low = 0; 
    unsigned int high = array->nel-1; //sets high to the highest index it could be 

        while(high>=low)
        {
            mid = (high + low) / 2; //picks the midpoint to start at

            readItem(performance, array, mid, temp_ptr); //reads the item from that midpoint and stores it in a temporary pointer

            if(compar(target,temp_ptr) > 0) //if the target is larger than the item at the current midpoint
            {
                low = mid + 1;
            }
            else if(compar(target, temp_ptr) < 0) //if the target is smaller than the item at the current midpoint
            {
                high = mid - 1;
            }
            else if(compar(target, temp_ptr) == 0) //if the item matches and has been found
            {
                free(temp_ptr); //frees the temporary pointer
                return mid; //returns the midpoint
            }
        }
    free(temp_ptr); //frees the temporary pointer
    return(-1); //if the number wasn't found returns -1

}