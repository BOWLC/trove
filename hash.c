#include <stdio.h>
#include <stdlib.h>
#include "trove.h"

#define HASHTABLE_SIZE 1000

typedef struct HashTable
{
    int numElements;
    struct ListNode table[HASHTABLE_SIZE];
} HASHTABLE;

int hashFunction(char *input){
    int hash = 0;
    while(*input != '\0'){
        hash = hash*33 + *input;
        input++;
    }
    return hash;
}

int getIndex(char *input){
    return hashFunction(input) % 1000;
}
