#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

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

void push(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width)
{
    struct Node *node = malloc(sizeof(struct Node)); //mallocs enough memory for a node
    node->data = malloc(width); //mallocs enough memory for the data type that will be stored in the node

    if (node == NULL || node->data == NULL) //if the malloc fails.
    {
        fprintf(stderr, "Error: Malloc failed.\n");
        exit(0);
    }

    memcpy(node->data,src,width); //copies the data into the node

    node->next = *list_ptr; //sets the list's head to the new nodes next node
    *list_ptr = node; //sets the new node as the head

    performance->mallocs++;
    performance->writes++;
}

void readHead(struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width)
{
    if (isEmpty(performance,list_ptr)) //if the list is empty.
    {
        fprintf(stderr, "Error: List is empty, cannot read head.\n");
        exit(0);
    }
    struct Node *head = *list_ptr;
    memcpy(dest,head->data,width); //copies the data of the lists head to the destination
    performance->reads++;
}

void pop(struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width)
{
    if (isEmpty(performance,list_ptr)) //if the list is empty.
    {
        fprintf(stderr, "Error: List is empty, cannot pop head.\n");
        exit(0);
    }
    struct Node *head = *list_ptr;
    memcpy(dest,head->data,width); //copies the data of the lists head to the destination
    *list_ptr = head->next; //sets the second node in the list as the new head
    free(head->data); //frees the data stored in the head
    free(head); //frees the head node itself

    performance->frees++;
    performance->reads++;
}

struct Node **next(struct Performance *performance, struct Node **list_ptr)
{
    if (isEmpty(performance,list_ptr)) //if the list is empty.
    {
        fprintf(stderr, "Error: List is empty, cannot get next node.\n");
        exit(0);
    }
    struct Node *head = *list_ptr;
    struct Node **temp = &head->next; //makes the next node in the list the head of the list
    performance->reads++;
    return(temp); //returns the shifted list
}

int isEmpty(struct Performance *performance, struct Node **list_ptr)
{
    if(*list_ptr == NULL) //if there is no head
    {
        return 1; //list is empty
    }
    else
    {
        return 0; //list is not empty
    }
}

void freeList(struct Performance *performance, struct Node **list_ptr)
{
    void *dest = malloc(1); //mallocs a temporary destination
    struct Node **currentNode = list_ptr;
    while(!isEmpty(performance,currentNode)) //while the list isn't empty
    {
        pop(performance,currentNode,dest,1); //keeps removing and freeing the head of the list until it is empty
    }
    free(dest); //frees the temporary destination
}

void readItem(struct Performance *performance, struct Node **list_ptr, unsigned int index, void *dest, unsigned int width)
{
    void *read = malloc(width); //mallocs enough memory for the size of the data in the nodes
    int i = 0;
    struct Node **currentNode = list_ptr;
    for(i = 0; i < index; i++) //runs through the list until it hits the correct index
    {
        currentNode = next(performance, currentNode);
    }
    readHead(performance,currentNode,read,width); //reads the node at the wanted index
    memcpy(dest,read,width); //copies the data from the proper index to the destination pointer
    free(read); //frees the temporary read destination
}

void appendItem(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width)
{
    struct Node **currentNode = list_ptr;
    struct Node **temp;
    int run = 1; //used to run the while loop
    if(!isEmpty(performance,list_ptr))
    {
        while(run) //runs until the next node is empty in order to add to the end of the list
        {
            currentNode = next(performance,currentNode);
            if(isEmpty(performance,currentNode))
            {
                run = 0;
            }
        }
    }
    push(performance,currentNode,src,width); //adds the node to the end of the list
    
}

void insertItem(struct Performance *performance, struct Node **list_ptr, unsigned int index, void *src, unsigned int width)
{
    int i = 0;
    struct Node **currentNode = list_ptr;
    for(i = 0; i < index; i++) //runs through the list until it hits the correct index
    {
        currentNode = next(performance, currentNode);
    }
    push(performance,currentNode,src,width); //pushes the node to the desired index in the list
}

void prependItem(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width)
{
    insertItem(performance,list_ptr,0,src,width); //uses insert item to add to the very front of the list
}

void deleteItem(struct Performance *performance, struct Node **list_ptr, unsigned int index)
{
    int i = 0;
    void *dest = malloc(1); //mallocs a temporary destination
    struct Node **currentNode = list_ptr;
    struct Node **temp;
    for(i = 0; i < index; i++) //runs through the list until it hits the correct index
    {
        currentNode = next(performance, currentNode);
    }
    pop(performance,currentNode,dest,1); //deletes the node at the desired index
    free(dest);
}

int findItem(struct Performance *performance, struct Node **list_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width)
{
    int i = 0;
    struct Node **currentNode = list_ptr;
    struct Node **temp;
    struct Node *temp_ptr;
    void *read = malloc(width); //mallocs a temporary destination with the size of the data being read
    while(!isEmpty(performance,currentNode)) //runs through the whole list one by one
    {
        readHead(performance,currentNode,read,width);
        if(compar(read, target) == 0) //if the item at the current index matches the target item
        {
            free(read);
            return(i); //returns the index of the found item
        }
        currentNode = next(performance,currentNode);
        i++;
    }
    free(read);
    return(-1); //returns -1 to show that the item wasn't found
}