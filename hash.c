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

HASHTABLE *make_hashtable(void){
    HASHTABLE *newtable = (HASHTABLE*)calloc(HASHTABLE_SIZE, sizeof(LISTWORD));
    if(!newtable){
        printf("Failed to calloc hash table.\n");
        exit(EXIT_FAILURE);
    }
    return newtable;
}

bool hashtable_find(HASHTABLE *table, char *word){
    int i = getIndex(word);
    printf("GetIndex, index: %d\n", i);
    return find_listword(table[i], word);
}

void hashtable_add(HASHTABLE *table, char *newword){
    if(hashtable_find(table, newword)){
        printf("It's already in the hash table!\n");
        return;
    }
    int index = getIndex(newword);
    if(table[index] == NULL){
        table[index] = new_listword(newword);
    }
    else{
        table[index] = add_listword(table[index], newword);
    }
    printf(" hashtable_add :: added word to hash table: %s\n", table[index]->word);
    if(table[index]->next) printf("table[index] is not null\n");
    return;
}

void testlist(HASHTABLE *table){
    char testword[] = "Test";
    printf("Adding %s to list table[0]\n", testword);
    table[0] = add_listword(table[0], testword);
    bool isin = find_listword(table[0], testword);
    if(isin){
        printf("Found %s in list table[0]\n", testword);
    }
    else{
        printf("Did not find %s in list table[0]\n", testword);
    }

}
