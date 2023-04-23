#include <stdio.h> 
#include "lists.h"

typedef LISTWORD *HASHTABLE;

//Declared here, defined in hash.c
extern uint32_t hashFunction(char *hashme);

extern uint32_t getIndex(char *hashme);

extern HASHTABLE *make_hashtable(void);

extern void hashtable_add(HASHTABLE *table, char *newword);

extern bool hashtable_find(HASHTABLE *table, char *word);

extern void testlist(HASHTABLE *table);
