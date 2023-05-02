#include <stdio.h>
#include <stdint.h> //for uint32_t
#include <stdlib.h> //calloc
#include <stdbool.h>
#include <string.h>

#include "hash.h"

//SIZE PRIME NUMBER TO IMPROVE PERFORMANCE
#define HASHTABLE_SIZE 997

uint32_t hashFunction(char *hashme){
    uint32_t hash = 0;
    while(*hashme!= '\0'){
        hash = hash*33 + *hashme;
        hashme++;
    }
    return hash;
}

uint32_t getIndex(char *hashval){
    return hashFunction(hashval) % HASHTABLE_SIZE;
}

HASHWORD *make_hashword(void){
    HASHWORD *newtable = ( HASHWORD* )calloc( HASHTABLE_SIZE , sizeof( LISTWORD ));
    if(!newtable){
        printf("Failed to calloc hash table.\n");
        exit(EXIT_FAILURE);
    }
    return newtable;
}

bool hashword_find( HASHWORD *table, char *word){
    int i = getIndex(word);
    return find_listword(table[i], word);
}

bool hashword_add( HASHWORD *table, char *newword){
    if(hashword_find(table, newword)){
        return true;
    }
    int index = getIndex(newword);
    table[index] = add_listword(table[index], newword);
    return false;
}

HASHFILE *make_hashfile(void){
    HASHFILE *newtable = (HASHFILE*)calloc(HASHTABLE_SIZE, sizeof(LISTFILE));
    if(!newtable){
        printf("Failed to calloc hash table.\n");
        exit(EXIT_FAILURE);
    }
    return newtable;
}

bool hashfile_find(HASHFILE *table, char *word){
    int i = getIndex(word);
    return find_listfile(table[i], word);
}



//returns true if word already in table
bool hashfile_add(HASHFILE *table, char *newword){
    if(hashfile_find(table, newword)){
        return true;
    }
    int index = getIndex(newword);
    table[index] = add_listfile(table[index], newword);
    return false;
}

//returns the word list structure from a hash table
LISTWORD *get_listword(HASHWORD *table, char *word){
    int i = getIndex( word );
    LISTWORD *list = table[i];
    while(list != NULL){
        if(strcmp(list->word, word) == 0){
            return list;
        }
        list = list->next;
    }
    return NULL;
}

//returns the word list structure from a hash table
LISTFILE *get_listfile(HASHFILE *table, char *path){
    int i = getIndex( path );
    LISTFILE *list = table[i];
    while(list != NULL){
        if(strcmp(list->path, path) == 0){
            return list;
        }
        list = list->next;
    }
    return NULL;
}
