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
    char filename_khs[STRLEN]; //will store the filename of the .khs file
    char *search_term = argv[2];  //stores the search term given by the third cmd line arg

    for (i = 0; i < strlen(argv[1]) - 3; i++) //gets the name of the .khs file
    {
        filename_khs[i] = argv[1][i];
    }
    filename_khs[i] = '\0';

    strcat(filename_khs, ".khs");

    FILE *kv = fopen(argv[1], "rb"); //opens the .kv file for reading in binary
    FILE *khs = fopen(filename_khs, "rb"); //opens the .khs file for reading in binary

    int hash_val = hashfn(search_term, get_capacity(khs)); //computes starting hash value

    int id = 0; //will store the index
    int iterations = 0; //stores number of times reset back to 0
    char val[STRLEN]; //will store the value of the key
    int num = 1; //runs the loop
    int found = 0; //set if the search term is found
    
    while (num) {
        read_index(khs,hash_val,&id); //reads in the index at the computed hash value
        read_key(kv, id, val); //reads the key at the given index

        if(strcmp(val,search_term) == 0) //if the key matches the search_term
        {
            read_val(kv,id,val); //read the value at the index
            num = 0; //stop the loop
            found = 1; //update found to signify a found term
        }
        else { //if key does not match the serach term
            if(hash_val == get_capacity(khs) - 1) //if the end of the hash table has been reached
            {
                hash_val = 0; //reset to the top of hash table
                iterations++; //increment iteration counter
            }
            else if(iterations < 2) //if the hashtable was not fully looped yet
            {
                hash_val++; //increment the hash value
            }
            else //if the hash table has been fully looped
            {
                found = 0; //nothing has been found
                num = 0; //stop the loop
            }
        }
    }

    if(found == 1) //if there was something found
    {
        printf("%s\n", val); //print the value of the key
    }
    else
    {
        printf("NOT FOUND\n");
    }

    fclose(kv); //close files to prevent leaks
    fclose(khs);

}