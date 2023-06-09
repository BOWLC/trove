#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lists.h"

char *my_strdup(char *string){
    int len;
    len = strlen(string) +1;
    char *sptr;
    sptr = (char*)malloc(len * sizeof(char));
    if(sptr == NULL){
        printf("\t Error: my_strdup :: String pointer output NULL!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(sptr, string);
    return sptr;
}

bool find_listword(LISTWORD *list, char *word){
    while(list != NULL){
        if(strcmp(list->word, word) == 0){
            return true;
        }
        list = list->next;
    }
    return false;
}

LISTWORD *new_listword(char *newword){
    LISTWORD *newnode = (LISTWORD *)calloc(1,sizeof(LISTWORD));
    if(!newnode){
        printf("calloc error new_listnode\n");
        exit(EXIT_FAILURE);
    }
    newnode->word = my_strdup(newword);
    if(!*newnode->word){
        printf("calloc error new_listnode word\n");
        exit(EXIT_FAILURE);
    }
    newnode->path = NULL; //Set pathlist pointer null, word just created
    return newnode;
}

LISTWORD *add_listword(LISTWORD *list, char *newword){
    if(find_listword(list, newword)) return list; //only add each word once in a list
    int id = 0;
    if(list != NULL){
        id = list->id + 1;
    } 
    LISTWORD *new = new_listword(newword);
    new->next = list;
    new->id = id;
    return new;
}

void delete_listword(LISTWORD *list){
    while(list != NULL){
        LISTWORD *current = list;
        list = list->next;
        free(current);
    }
    return;
}

bool find_listpath(LISTPATH *list, LISTFILE *path){
    while(list != NULL){
        if( list->path == path) return true;
        list = list->next;
    }
    return false;
}

LISTPATH *new_listpath(LISTFILE *path){
    LISTPATH *newnode = (LISTPATH *)calloc(1,sizeof(LISTPATH));
    if(!newnode){
        printf("calloc error new_listpath\n");
        exit(EXIT_FAILURE);
    }
    newnode->path = path;
    if(newnode->path == NULL){
        printf("\t calloc error new_listpath ->path.  Should be no NULL paths.\n");
        exit(EXIT_FAILURE);
    }
    return newnode;
}


LISTPATH *add_listpath(LISTPATH *list, LISTFILE *path){
    if(find_listpath(list, path)) return list; //only add each word once in a list
    if(path == NULL){
        printf("Error in add_listpath :: LISTFILE->path should never be null\n");
    } 
    LISTPATH *new = new_listpath(path);
    new->next = list;
    return new;
}

void delete_listpath(LISTPATH *list){
    while(list != NULL){
        LISTPATH *current = list;
        list = list->next;
        free(current);
    }
    return;
}

bool find_listfile(LISTFILE *list, char *path){
    while(list != NULL){
        if(strcmp(list->path, path) == 0){
            return true;
        }
        list = list->next;
    }
    return false;
}

LISTFILE *new_listfile(char *newpath){
    LISTFILE *newnode = ( LISTFILE *)calloc(1,sizeof(LISTFILE));
    if(!newnode){
        printf("calloc error new_listfile\n");
        exit(EXIT_FAILURE);
    }
    newnode->path = my_strdup(newpath);
    if(!*newnode->path){
        printf("calloc error new_listfile word\n");
        exit(EXIT_FAILURE);
    }
    return newnode;
}

LISTFILE *add_listfile(LISTFILE *list, char *newpath){
    if(find_listfile(list, newpath)) return list; //only add each word once in a list
    int id = 0;
    if(list != NULL){
        id = list->id +1;
    }
    LISTFILE *new = new_listfile(newpath);
    new->next = list;
    new->id = id;
    return new;
}

void delete_listfile(LISTFILE *list){
    while(list != NULL){
        LISTFILE *current = list;
        list = list->next;
        free(current);
    }
    return;
}

//returns list that points to each path using the hashtable location
LISTPATH *get_paths_from_list(LISTWORD *list, char *word){
    while(list != NULL){
        if(strcmp(list->word, word) == 0){
            return list->path;
        }
        list = list->next;
    }
    return NULL;
}


