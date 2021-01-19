#include "ttt.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int numArgs = argc; //stores number of command line arguments
    int nums[numArgs-1]; //nums array that will store all the hash values passed in
    for(int i = 1; i < numArgs; i++) //goes through each command line argument excluding the exec
    {
        nums[i] = atoi(argv[i]); //stores the hash value in the array
    }

    init_boards(); //initializes the boards
    init_board(START_BOARD); //initializes the first start board
    join_graph(START_BOARD); //joins the graph starting with a blank board
    compute_score(); //computes the scores

    for(int i = 1; i < numArgs; i++) //loops through all the values
    {
        print_node(htable[nums[i]]); //prints the node of each one
    }

    if(numArgs == 0){
        print_node(htable[0]); //if no node value was passed in, prints the start board node
    }

}