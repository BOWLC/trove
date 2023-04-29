#ifndef _HASH_TROVE
#define _HASH_TROVE
#include <stdio.h> 

#include "lists.h"

typedef LISTWORD *HASHWORD;
typedef LISTPATH *HASHPATH;
typedef LISTFILE *HASHFILE;

//Declared here, defined in hash.c

extern HASHWORD *make_hashword(void);
extern HASHFILE *make_hashfile(void);


extern bool hashword_add(HASHWORD *table, char *newword);

extern bool hashword_find(HASHWORD *table, char *word);


extern bool hashfile_add(HASHFILE *table, char *newword);

extern bool hashfile_find(HASHFILE *table, char *word);
#endif
