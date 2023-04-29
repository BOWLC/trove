#define _XOPEN_SOURCE 500 //required for opendir() and readdir()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>

#include "trovefile.h"


LISTFILE *filesindir(LISTFILE *path, char *folder, HASHFILE *ptable);

LISTFILE *findFiles(LISTFILE *filelist, LISTFILE *folders, HASHFILE *ptable, int wordmin, char *trovename){

    //add all files within folders to the list
    while(folders != NULL){
    printf("folders->path is : %s\n", folders->path);
    filelist = filesindir(filelist, folders->path, ptable);
    LISTFILE *temp = filelist;
    printf("=== PRINTING FILELIST FROM LIST ===\n");
        while(temp != NULL){
            printf(" %s\n", temp->path);
            temp = temp->next;
        }
    folders = folders->next;

    }
    /*
    while(filelist!= NULL){
        FILE *spTrove;
        char *trovename = (*filelist)->path;
        spTrove = fopen(trovename, "w");
        if(spTrove == NULL){
            printf("buildTrove :: Could not open new file %s\n", trovename);
        }
        printf("Build trove.\n");

        if(fclose(spTrove) == EOF){
            printf("Error closing Trovefile built in buildTrove.\n");
        }
        folders = folders->next;
    }
    */
    return filelist;
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


    printf("filesindir ::  currdir: %s\n", currdir);

    if((dirp = opendir(currdir)) == NULL){
        printf("filesindir :: directory : %s\n", currdir);
        perror("Could not open directory ");
        exit(EXIT_FAILURE);
    }

    do{
        nextpath[0] = '\0';
        strcpy(nextpath, currdir);
        errno =0;
        printf("dirn not NULL\n");
        if((dirn =  readdir(dirp)) != NULL){

            strcat(nextpath, dirn->d_name);
            if((strcmp(dirn->d_name, ".") == 0) || (strcmp(dirn->d_name, "..") == 0)) continue;

            strcat(nextpath, "/");
            strcat(nextpath, dirn->d_name);

            DIR *dirpn;//just to test if it is directory or file

            if((dirpn = opendir(nextpath)) != NULL){
                //don't add directory onto the list (it's a folder)
                //recursively search folders for files and add to the START of the list
                filesindir(path, nextpath, ptable);
                closedir(dirpn);
                continue;
            }

            if(!hashfile_add(ptable, nextpath)){
                //File not already in the list
                path = add_listfile(path, nextpath);//add to list
                closedir(dirpn);
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
