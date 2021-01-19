#include <string.h>
#include <stdlib.h>
#include "hashfn.h"
#include "util.h"

void valToKey (char *search_term, FILE *kv, FILE *vhs, char *val); //helper function declaration
void keyToVal (char *search_term, FILE *kv, FILE *khs, char *val); //helper function declaration

int main( int argc, char **argv )
{
    FILE *nmkv = fopen("name.basics.kv", "rb"); //opens all necessary files for reading binary
    FILE *nmvhs = fopen("name.basics.vhs", "rb");
    FILE *ttkv = fopen("title.basics.kv", "rb");
    FILE *ttkhs = fopen("title.basics.khs", "rb");
    FILE *tpkv = fopen("title.principals.kv", "rb");
    FILE *tpkhs = fopen("title.principals.khs", "rb");
    char val[STRLEN]; //declares strings that will store info from helper functions
    char movie_name[STRLEN];
    char bacon_key[STRLEN];
    char name_key[STRLEN];
    char movie_key[STRLEN];
    char new_key[STRLEN];
    char actor[STRLEN];

    valToKey(argv[1], nmkv, nmvhs,name_key); //stores the key of the name that was passed in via the second cmd line argument
    valToKey("Kevin Bacon", nmkv, nmvhs,bacon_key); //stores the key of Kevin Bacon

    for(int i = 0; i < get_capacity(tpkhs); i++) //loops through the whole file
    {
        read_val(tpkv, i, val); //reads in the value at the given index
        if(strcmp(val, bacon_key) == 0) //if the value is Kevin Bacon's id, the key will be a Kevin Bacon movie
        {
            read_key(tpkv, i, movie_key); //store the name of the movie
            for(int j = 0; j < get_capacity(tpkhs); j++) //loop through the whole file again
            {
                read_val(tpkv, j, actor); //read the value at the given index
                if(strcmp(actor, name_key) == 0) //if the actor matches the target actor
                {
                    read_key(tpkv, j, new_key); //get the key of the movie the target actor is in
                    if(strcmp(new_key, movie_key) == 0) //if the movie is the same movie that Kevin Bacon is in
                    {
                        keyToVal(movie_key, ttkv, ttkhs, movie_name); //store the name of the movie
                        printf("%s\n", movie_name); //print the movie name out
                        i = get_capacity(tpkhs); //stop the outer for loop
                        j = get_capacity(tpkhs); //stop the inner for loop
                    }
                }
            }
        }
    }
    fclose(nmkv); //close all files to prevent leaks
    fclose(nmvhs);
    fclose(ttkv);
    fclose(ttkhs);
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