#ifndef _LISTS_TROVE
#define _LISTS_TROVE
#include <stdio.h>
#include <stdbool.h>

//Pathname to be stored in hash table
typedef struct ListFile
{
    int id;
    char *path;//path name
    struct ListFile *next;//list for hashtable
} LISTFILE;

//List of *path associated with each file
typedef struct ListPath
{
    struct ListFile *path;
    struct ListPath *next;
} LISTPATH;

//Words to be stored in hash table
typedef struct ListWord
{
    int id;
    char *word;
    struct ListPath *path; //list of paths where word is found
    struct ListWord *next; //list for hashtable
} LISTWORD;

extern bool find_listword(LISTWORD *list, char *word);
extern LISTWORD *add_listword(LISTWORD *list, char *word);
extern void delete_listword(LISTWORD *list);

extern bool find_listpath(LISTPATH *list, LISTFILE *path);
extern LISTPATH *add_listpath(LISTPATH *list, LISTFILE *path);
extern void delete_listpath(LISTPATH *list);

extern bool find_listfile(LISTFILE *list, char *path);
extern LISTFILE *add_listfile(LISTFILE *list, char *path);
extern void delete_listfile(LISTFILE *list);

extern LISTPATH *get_paths_from_list(LISTWORD *list, char *word);
#endif
