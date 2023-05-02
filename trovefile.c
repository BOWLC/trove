#define _XOPEN_SOURCE 500 //required for opendir() and readdir()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <linux/limits.h>

#include "trovefile.h"

//maximum length of a word considered valid
#define WORDLEN_MAX 1024


LISTFILE *filesindir(LISTFILE *path, char *folder, HASHFILE *ptable);//recursive search dir for files

void insertTrove(HASHWORD *wordhash, HASHFILE *pathhash, char *pathname, char *word);

LISTFILE *findFiles(LISTFILE *filelist, LISTFILE *folders, HASHFILE *ptable, int wordmin, char *trovename){

    //add all files within folders to the list
    while(folders != NULL){
        filelist = filesindir(filelist, folders->path, ptable);
        folders = folders->next;
    }
    return filelist;
}

LISTWORD  *buildTrove(LISTFILE *filelist, HASHFILE *pathhash, HASHWORD *wordhash, int wordmin){
    printf("Build trove\n");
    LISTWORD *allwords = NULL;

    while(filelist!= NULL){
        FILE *spTrove;
        char *pathname = filelist->path;
        char cbuff;
        char wordbuff[WORDLEN_MAX];
        int wordlen = 0;
        spTrove = fopen(pathname, "r");

        if(spTrove == NULL){
            printf("buildTrove :: Could not open new file %s\n", pathname);
            perror("buildtrove error: ");
            filelist = filelist->next;
        }
        else{

            printf("reading file: \n");
            while((cbuff = fgetc(spTrove)) != EOF){
                printf("%c", cbuff);
                if(wordlen +1 < WORDLEN_MAX){//word not too long
                    if(isalpha(cbuff) || isdigit(cbuff)){ 
                        //character is alphanumeric
                        wordbuff[wordlen] = cbuff;
                        wordlen++;
                    }
                    else if(wordlen >=  wordmin){//word not too short
                        wordbuff[wordlen] = '\0';
                        if(!hashword_find(wordhash, wordbuff)){//only add word if it's not in the hash table already
                            allwords = add_listword(allwords, wordbuff);//Add word to quick ref list
                            printf("allwords id on top: %d\n", allwords->id);
                        }
                        insertTrove(wordhash, pathhash, pathname, wordbuff);
                        wordlen = 0;//added complete word
                    }
                    else wordlen = 0;//word complete and too short
                }
                else{
                    wordlen = 0;//word too long
                }
                
            }
            printf("\n");

            if(fclose(spTrove) == EOF){
                printf("Error closing Trovefile built in buildTrove.\n");
            }
        }
        filelist = filelist->next;
    }
    return allwords;
}

void insertTrove(HASHWORD *wordhash, HASHFILE *pathhash, char *pathname, char *word){
    LISTPATH *pathlist;
    LISTWORD *wordinhash;
    LISTFILE *pathinhash;


    hashword_add(wordhash, word);
    printf("insertTrove word to be added: %s \n", word);

    wordinhash = get_listword(wordhash, word);
    pathinhash = get_listfile(pathhash, pathname);

    if(wordinhash != NULL){
        //word in hash table
        pathlist = add_listpath(wordinhash->path, pathinhash);
        wordinhash->path = pathlist;

    }
    else{
        printf("insertTrove Error ::  word not added to hash table correctly.\n");
    }
    return;
}

void searchTrove(char *trovename, char *word){
    printf("Searching trove -%s- for \"%s\"...\n", trovename, word);
}


LISTFILE *filesindir(LISTFILE *path, char *folder, HASHFILE *ptable){
    //LISTFILE *path = *listpath;
    struct dirent *dirn;
    DIR *dirp;
    char nextpath[PATH_MAX];
    char currdir[PATH_MAX];
    strcpy(currdir, folder);



    if((dirp = opendir(currdir)) == NULL){
        printf("filesindir :: directory : %s\n", currdir);
        perror("Could not open directory ");
        exit(EXIT_FAILURE);
    }

    do{
        nextpath[0] = '\0';
        strcpy(nextpath, currdir);
        errno =0;

        if((dirn =  readdir(dirp)) != NULL){

            if((strcmp(dirn->d_name, ".") == 0) || (strcmp(dirn->d_name, "..") == 0)) continue;

            strcat(nextpath, "/");
            strcat(nextpath, dirn->d_name);

            DIR *dirpn;//just to test if it is directory or file

            if((dirpn = opendir(nextpath)) != NULL){
                //don't add directory onto the list (it's a folder)
                //recursively search folders for files and add to the START of the list
                path = filesindir(path, nextpath, ptable);
                closedir(dirpn);
            }
            else{
                if(!hashfile_add(ptable, nextpath)){
                    //File not already in the list
                    path = add_listfile(path, nextpath);//add to list
                    printf("listfile top is: %d\n", path->id);
                    closedir(dirpn);
                }
            }                
        }
    } while(dirn != NULL); 

    if(errno != 0){
        perror("reddir failed in buildTrove");
        exit(EXIT_FAILURE);
    }
    closedir(dirp);
    return path;
 
}


int trimTrove(char *trovename, char *word[]){
    printf("Trim trove.\n");
    return 0;
}

int updateTrove(char *trovename, char *word[]){
    printf("Update trove.\n");
    return 0;
}


int path_validate(char *dir){
    if(access(dir, F_OK) != 0){
        //file does not exist
        usage(2);
        return 2;
    }    
    else
    if(access(dir, R_OK) != 0){
        //no read access
        usage(3);
        return 3;
    }
    if(access(dir, W_OK) != 0){
        //no write access
        usage(4);
        return 4;
    }
    return 0;

}
