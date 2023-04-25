#include <stdio.h>
#include <stdint.h> //for uint32_t
#include <stdlib.h> //calloc
#include <stdbool.h>

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
    printf("GetIndex, index: %d\n", i);
    return find_listword(table[i], word);
}

void hashtword_add( HASHWORD *table, char *newword){
    if(hashword_find(table, newword)){
        printf("It's already in the hash table!\n");
        return;
    }
    int index = getIndex(newword);
    table[index] = add_listword(table[index], newword);
    printf(" hashword_add :: added word to hash table: %s\n", table[index]->word);
    return;
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
    printf("GetIndex, index: %d\n", i);
    return find_listfile(table[i], word);
}

void hashfile_add(HASHFILE *table, char *newword){
    if(hashfile_find(table, newword)){
        printf("It's already in the hash table!\n");
        return;
    }
    int index = getIndex(newword);
    table[index] = add_listfile(table[index], newword);
    printf(" hashfile_add :: added path to hash table: %s\n", table[index]->path);
    return;
}
