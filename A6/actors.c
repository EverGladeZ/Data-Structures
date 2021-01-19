#include <string.h>
#include <stdlib.h>
#include "hashfn.h"
#include "util.h"

void valToKey (char *search_term, FILE *kv, FILE *vhs, char *val); //helper function declaration
void keyToVal (char *search_term, FILE *kv, FILE *khs, char *val); //helper function declaration

int main( int argc, char **argv )
{
    FILE *nmkv = fopen("name.basics.kv", "rb"); //opens all necessary files for reading in binary
    FILE *nmkhs = fopen("name.basics.khs", "rb");
    FILE *ttkv = fopen("title.basics.kv", "rb");
    FILE *ttvhs = fopen("title.basics.vhs", "rb");
    FILE *tpkv = fopen("title.principals.kv", "rb");
    FILE *tpkhs = fopen("title.principals.khs", "rb");
    char val[STRLEN]; //declares strings that will store info from the helper functions
    char name[STRLEN];
    char movie_key[STRLEN];
    char new_key[STRLEN];

    valToKey(argv[1], ttkv, ttvhs,movie_key); //get the key of the movie passed in via the second cmd line arguement
    while(read_keyval(tpkv, new_key, val) == 2) //while there is still something to read in .kv
    {
        if(strcmp(movie_key, new_key) == 0) //if the key of the movie in the current line is equal to the key of the movie given in the cmd line arguement
        {
            keyToVal(val,nmkv,nmkhs,name); //get the value of the movie's key (which is the actor's name)
            printf("%s\n",name); //print the actors name
        }
    }
    fclose(nmkv); //close all files to prevent leaks
    fclose(nmkhs);
    fclose(ttkv);
    fclose(ttvhs);
    fclose(tpkv);
    fclose(tpkhs);
}

void valToKey (char *search_term, FILE *kv, FILE *vhs, char *val) { //a function version of the val2key.c program (see that code for comments)
    int hash_val = hashfn(search_term, get_capacity(vhs));
    int id = 0;
    int iterations = 0;
    int num = 1;
    int found = 0;
    
    while (num) {
        read_index(vhs,hash_val,&id);
        read_val(kv, id, val);
        if(strcmp(val,search_term) == 0)
        {
            read_key(kv,id,val);
            num = 0;
        }
        else {
            if(hash_val == get_capacity(vhs) - 1)
            {
                hash_val = 0;
                iterations++;
            }
            else if(iterations < 2)
            {
                hash_val++;
            }
            else
            {
                val = NULL;
                found = 0;
                num = 0;
                
            }
        }
    }
    
}

void keyToVal (char *search_term, FILE *kv, FILE *khs, char *val) { //a function version of the key2val.c program (see that code for comments)
    int hash_val = hashfn(search_term, get_capacity(khs));
    int id = 0;
    int iterations = 0;
    int num = 1;
    int found = 0;
    
    while (num) {
        read_index(khs,hash_val,&id);
        read_key(kv, id, val);
        if(strcmp(val,search_term) == 0)
        {
            read_val(kv,id,val);
            num = 0;
        }
        else {
            if(hash_val == get_capacity(khs) - 1)
            {
                hash_val = 0;
                iterations++;
            }
            else if(iterations < 2)
            {
                hash_val++;
            }
            else
            {
                found = 0;
                num = 0;
                val = NULL;
            }
        }
    }
    
}
