#include "ttt.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char **argv)
{
    int counter = 0; //will store number of nodes
    init_boards(); //initializes boards
    init_board(START_BOARD); //initializes the node of empty start board
    join_graph(START_BOARD); //joins the graph beginning with the start board
    for(int i = 0; i < HSIZE; i++) //loops through the whole hash table
    {
        if(htable[i].init == 1) //if the board is not empty
        {
            counter++; //increments counter of non-empty nodes
        }
    }
    printf("%d\n", counter); //prints the number out
}