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
                    closedir(dirpn);
                }
            }                
        }
    }while(dirn != NULL); 

    if(errno != 0){
        perror("reddir failed in buildTrove");
        exit(EXIT_FAILURE);
    }
    closedir(dirp);
    return path;
 
}

LISTFILE *findFiles(LISTFILE *filelist, LISTFILE *folders, HASHFILE *ptable, int wordmin){
    //add all files within folders to the list
    while(folders != NULL){
        filelist = filesindir(filelist, folders->path, ptable);
        folders = folders->next;
    }
    return filelist;
}

void insertTrove(HASHWORD *wordhash, HASHFILE *pathhash, char *pathname, char *word){
    LISTPATH *pathlist;
    LISTWORD *wordinhash;
    LISTFILE *pathinhash;


    hashword_add(wordhash, word);

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

LISTWORD  *buildTrove(LISTFILE *filelist, LISTFILE *folderlist, HASHFILE *pathhash, HASHWORD *wordhash, char *trovename, int wordmin){
    printf("Build trove\n");

    LISTWORD *allwords = NULL;//list of all words of min length found in files
    LISTFILE *filelist_start = filelist;
    printf("in buildTrove, initial filelist id: %d\n", filelist->id);

    //file we are writing (trovefile)
    FILE *trovefilefp;

    //number of files/words found
    int numfiles;
    int numwords;

    //read each file by character, adding words to list and hash table
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

            while((cbuff = fgetc(spTrove)) != EOF){
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
                        }
                        insertTrove(wordhash, pathhash, pathname, wordbuff);
                        wordlen = 0;//added complete word
                    }
                    else wordlen = 0;//word complete but too short
                }
                else{
                    wordlen = 0;//word too long
                }
                
            }

            if(fclose(spTrove) == EOF){
                printf("Error closing Trovefile built in buildTrove.\n");
            }
        }
        filelist = filelist->next;
    }

    //Now save trove file as txt file

      if((trovefilefp = fopen(trovename, "w")) == NULL){
        perror("buildTrove writing trovefile");
        exit(EXIT_FAILURE);
    }

    LISTWORD *allwords_start = allwords;
    filelist = filelist_start;
    numfiles = filelist->id + 1;
    numwords = allwords->id +1;
    printf("before writing trovefile, numwords: %d \t numfiles: %d\n", numwords, numfiles);

    //write numfiles to new line
    fputc(numfiles, trovefilefp);
    fputc('\n', trovefilefp);
    //write numwords to new line
    fputc(numwords, trovefilefp);
    fputc('\n', trovefilefp);
    //write filelist to new lines
    while(filelist != NULL){
        fputs(filelist->path, trovefilefp);
        fputc('\n', trovefilefp);
        filelist = filelist->next;
    }
    //write words to new lines
    while(allwords != NULL){
        fputs(allwords->word, trovefilefp);
        fputc('\n', trovefilefp);
        allwords = allwords->next;
    }
    allwords = allwords_start;

    /*write relation of each file to folder as a relation (1 for linked, 0 for not)
     * for each char y on line x where char = '1', word of id x is related to file id y
     * */

    int infile[numfiles];//which files the word occurs in

    while(allwords != NULL){
        LISTWORD *wordp = get_listword(wordhash, allwords->word);
        LISTPATH *pathlist = wordp->path; // first path list item related to this word
        memset(infile, 0, numfiles);
        while(pathlist != NULL){
            int pathid = pathlist->path->id;
            infile[pathid] = 1;//record that we found the file
            pathlist = pathlist->next;
        }

        for(int i = 0 ; i < numwords; i++){
            fputc(infile[i], trovefilefp);
        }

        fputc('\n', trovefilefp);
        allwords = allwords->next;
    }


    return allwords_start;
}
void searchTrove(char *trovename, char *word){
    printf("Searching trove -%s- for \"%s\"...\n", trovename, word);
    FILE *trovefilefp;

    //HASHWORD *wordhash;
    //HASHFILE *filehash;

    int numfiles;
    int numwords;


    if((trovefilefp = fopen(trovename, "r")) == NULL){
        perror("Opening trovefile to be searched");
        exit(EXIT_FAILURE);
    }
    numfiles = fgetc(trovefilefp);
    printf("found number of files: %d\n", numfiles);
    if(fgetc(trovefilefp) != '\n'){
        printf("Error in trovefile format.  exiting...\n");
        exit(EXIT_FAILURE);
    }

    numwords = fgetc(trovefilefp);
    printf("found number of words: %d\n", numwords);
    if(fgetc(trovefilefp) != '\n'){
        printf("Error in trovefile format.  exiting...\n");
        exit(EXIT_FAILURE);
    }
    char paths[numfiles][PATH_MAX];
    char words[numwords][WORDLEN_MAX];
    for(int i = 0 ; i < numfiles ; i++){
        fgets(paths[i], PATH_MAX, trovefilefp);
        printf("found file path after loading trovefile: %s\n", paths[i]);
    }

    for(int i = 0 ; i < numwords ; i++){
        fgets(words[i], WORDLEN_MAX, trovefilefp);
        if(ferror(trovefilefp)){
            printf("error reading file stream.\n");
        }
        printf("found word after loading trovefile: %s\n", words[i]);
    }

    if((fclose(trovefilefp)) == EOF){
        perror("Closing trovefile after search");
        exit(EXIT_FAILURE);
    }
    

    //check if files in trovefile still exist
    //search given words in trovefile and list their locations
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
