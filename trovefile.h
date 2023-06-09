#ifndef _TROVEFILE
#define _TROVEFILE
#include <stdio.h>

#include "trove.h"
#include "hash.h"
#include "lists.h"

void searchTrove(char *trovename, char *word);
LISTFILE *findFiles(LISTFILE *list,LISTFILE *folderlist, HASHFILE *ptable, int wordmin);
int trimTrove(char *trovename, char *word[]);
int updateTrove(char *trovename, char *word[]);
int path_validate(char *dir);
LISTWORD *buildTrove(LISTFILE *filelist, LISTFILE *folderlist, HASHFILE *ptable,  HASHWORD *wordhash, char *trovename, int wordmin);
#endif
