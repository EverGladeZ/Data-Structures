#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

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

void attachNode(struct Performance *performance, struct Node **node_ptr, void *src, unsigned int width)
{
    struct Node *node = malloc(sizeof(struct Node)); //mallocs enough memory for a node
    node->data = malloc(width); //mallocs enough memory for the data type that will be stored in the node

    if (node == NULL || node->data == NULL) //if the malloc fails.
    {
        fprintf(stderr, "Error: Malloc failed.\n");
        exit(0);
    }

    memcpy(node->data,src,width); //copies the data into the node
    node->lt = NULL;
    node->gte = NULL;
    *node_ptr = node;
    performance->mallocs++;
    performance->writes++;
}

int comparNode(struct Performance *performance, struct Node **node_ptr, int(*compar)(const void *, const void *), void *target)
{
    performance->reads++;
    return (compar(target,(*node_ptr)->data)); //returns the result of the compar function
}

struct Node **next(struct Performance *performance, struct Node **node_ptr, int direction)
{
        struct Node *node = *node_ptr;
        if (node == NULL || node->data == NULL) //if the tree is empty
        {
            fprintf(stderr, "Error: Malloc failed.\n");
            exit(0);
        }
        performance->reads++;
        if(direction < 0) //if less than
        {
            return(&node->lt); //go to left side
        }
        else
        {
            return(&node->gte); //go to right side
        }
}

void readNode(struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width)
{
    if (isEmpty(performance,node_ptr)) //if the tree is empty.
    {
        fprintf(stderr, "Error: Tree is empty, cannot read node.\n");
        exit(0);
    }
    struct Node *node = *node_ptr;
    memcpy(dest,node->data,width); //copies the data of the tree's node to the destination
    performance->reads++;

}

void detachNode(struct Performance *performance, struct Node **node_ptr)
{
    if (isEmpty(performance,node_ptr)) //if the tree is empty.
    {
        fprintf(stderr, "Error: Tree is empty, cannot detach node.\n");
        exit(0);
    }
    struct Node *node = *node_ptr;
    free(node->data); //frees the data stored in the node
    free(node); //frees the node itself
    node_ptr = NULL;
    performance->frees++;
}

int isEmpty(struct Performance *performance, struct Node **node_ptr)
{
    if(*node_ptr == NULL) //if there is no node
    {
        return 1; //tree is empty
    }
    else
    {
        return 0; //tree is not empty
    }
}

void addItem(struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *src, unsigned int width)
{
    struct Node **temp = node_ptr;
    while(!isEmpty(performance,temp)) //goes through the tree to an empty spot following BST rules
    {
        temp = next(performance, temp, comparNode(performance, temp, compar, src));
    }
    attachNode(performance,temp,src,width); //attaches node to the proper place
}

void freeTree(struct Performance *performance, struct Node **node_ptr)
{
    if (isEmpty(performance,node_ptr))
    {
        return;
    }

    freeTree(performance,next(performance,node_ptr,-1)); //uses recurrsion to go down all necessary left branches
    freeTree(performance,next(performance,node_ptr,1)); //uses recurrsion to go down all necessary right branches
    detachNode(performance, node_ptr); //deataches (and frees) the proper node

}

int searchItem(struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width)
{
    struct Node **currentNode = node_ptr;
    while(!isEmpty(performance,currentNode)) //runs through the whole Node path 
    {
        int temp = comparNode(performance, currentNode, compar, target);
        if(temp == 0) //if the current node the target node
        {
            readNode(performance,currentNode,target,width);
            return(1); //returns 1 to show the node was found
        }
            currentNode = next(performance,currentNode, temp);
    }
    return(0); //returns 0 to show that the node wasn't found
}