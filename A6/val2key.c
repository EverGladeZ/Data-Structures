#include <string.h>
#include <stdlib.h>
#include "hashfn.h"
#include "util.h"

int main( int argc, char **argv )
{
    if (argc > 3 || argc < 3) { //check valid number of command line args
        fprintf(stderr, "Usage: %s filename.kv 'search term'\n", argv[0]);
        exit(1);
    }

    int i = 0;
    char filename_vhs[STRLEN];  //will store the filename of the .vhs file
    char *search_term = argv[2];  //stores the search term given by the third cmd line arg

    for (i = 0; i < strlen(argv[1]) - 3; i++) //gets the name of the .vhs file
    {
        filename_vhs[i] = argv[1][i]; 
    }
    filename_vhs[i] = '\0';

    strcat(filename_vhs, ".vhs");

    FILE *kv = fopen(argv[1], "rb"); //opens the .kv file for reading in binary
    FILE *vhs = fopen(filename_vhs, "rb"); //opens the .vhs file for reading in binary

    int hash_val = hashfn(search_term, get_capacity(vhs)); //computes starting hash value
    int id = 0; //will store index
    int iterations = 0; //stores the number of times reset back to zero
    char val[STRLEN]; //will store the value
    int num = 1; //runs the loop
    int found = 0; //set if the search term is found 
    
    while (num) {
        read_index(vhs,hash_val,&id); //reads in the index at the computed hash value
        read_val(kv, id, val); //reads in the value at the given index
        if(strcmp(val,search_term) == 0) // if the value matches the search term
        {
            read_key(kv,id,val); //read the key at the index
            num = 0; //stop the loop
            found = 1; //update dound 
        }
        else { //if value does not match search term
            if(hash_val == get_capacity(vhs) - 1) //if at the end of hash table
            {
                hash_val = 0; //reset to the beginning
                iterations++; //increment iteration counter
            }
            else if(iterations < 2) //if hashtable was not fully looped
            {
                hash_val++; //increment the hash value
            }
            else //if the table has been fully looped
            {
                found = 0; //nothing has been found
                num = 0; //stop the loop
            }
        }
    }

    if(found == 1) //if something was found
    {
        printf("%s\n", val); //print the key of the value
    }
    else
    {
        printf("NOT FOUND\n");
    }

    fclose(kv); //close files to prevent leaks
    fclose(vhs);

}