#include <string.h>
#include <stdlib.h>
#include "hashfn.h"
#include "util.h"

int main( int argc, char **argv )
{
    if (argc > 3 || argc < 3) { //if wrong number of  cmd line arguments
        fprintf(stderr, "Usage: %s filename.kv capacity\n", argv[0]);
        exit(1);
    }
    char filename_khs[STRLEN]; //will store the name of the .khs file
    char filename_vhs[STRLEN]; //will store the name of the .vhs file
    int i = 0;
    for (i = 0; i < strlen(argv[1]) - 3; i++) //loops through to get the filename without the .kv
    {
        filename_khs[i] = argv[1][i];
        filename_vhs[i] = argv[1][i];
    }
    filename_khs[i] = '\0'; //terminates string
    filename_vhs[i] = '\0'; //terminates string

    strcat(filename_khs, ".khs"); //connects the filename with the .khs suffix
    strcat(filename_vhs, ".vhs"); //connects the filename with the .vhs suffix

    FILE *kv = fopen(argv[1], "rb"); //opens the .kv file for reading in binary
    FILE *khs = fopen(filename_khs, "wb+"); //opens the .khs file for reading and writing in binary
    FILE *vhs = fopen(filename_vhs, "wb+"); //opens the .vhs file for reading and writing in binary

    write_empty(khs, atoi(argv[2])); //writes an empty hashtable to the .khs file using the capacity given in the third command line arg
    write_empty(vhs, atoi(argv[2])); //writes an empty hashtable to the .vhs file using the capacity given in the third command line arg

    int capacity = get_capacity(khs); //gets the capacity of the hashtables

    char key[STRLEN]; //will store the key
    char val[STRLEN]; //will store the value
    int index = 0; //index that will be used for seeing if the spot in the hashtable is empty
    i = 0; //will store index of the key-value pair in the .kv file
    int keyhash = 0; //stores the hash value for the key
    int valhash = 0; //stores the hash value for the value

    while(read_keyval(kv, key, val) == 2) //while there is still information to be read from .kv
    {
        keyhash = hashfn(key, capacity); //compute hash value for key
        valhash = hashfn(val, capacity); //compute hash value for value
        read_index(khs, keyhash, &index); //check the index at the given hash value for the key table
        while(index != -1) //if the spot in the hashtable is filled
        {
            if(keyhash == capacity-1) //if it is the end of the hash table
            {
                keyhash = 0; //loop back around to the beginning
            }
            else {
                keyhash++; //move to the next spot down
            }
            read_index(khs, keyhash, &index); //set the index to its new value after hash was changed
        }
        write_index(khs, i, keyhash); //write the key to the key hash
        read_index(vhs, valhash, &index); //check the index at the given hash value for the value table
        while(index != -1) //if the spot is filled
        {
            if(valhash == capacity-1) //if it is the end of the hash table
            {
                valhash = 0; //loop back around to the beginning
            }
            else {
                valhash++; //move to the next spot down 
            }
            read_index(vhs, valhash, &index); //set the index to its new value after hash was changed
        }
        write_index(vhs, i, valhash); //move to the next spot down
        i++; //increment the line being read from the .kv
    }
    fclose(kv); //close all files to prevent leaks
    fclose(vhs);
    fclose(khs);
    
    
    


}