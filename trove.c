#define _XOPEN_SOURCE 500 //required for opendir()
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>

#include "trove.h"

#define OPTLIST "f::brul::"
void usage(int ecode);
void arg_validate_l(int arg);
int path_validate(char *dir);

int main(int argc, char *argv[]){

    int opt;
    int f_access; //status of trovefile accessibility
    bool bflag = false;
    bool rflag = false;
    bool uflag = false;
    char fdir[PATH_MAX] = DEFAULT_TROVEFILE;
    int lmin = DEFAULT_WORDLEN;
    int opterr = 0; //do not print errors

    if(argc == 1) usage(1); //too few program arguments

    while((opt = getopt(argc, argv, OPTLIST)) !=-1){
        switch(opt){
            case 'f':
                if(optarg && *optarg){
                    printf("optarg && *optarg, changing from default trovefile\n");
                    strcpy(fdir, optarg); //only change from default path if arg provided
                }
                else printf("Using default trovefile path: %s\n", DEFAULT_TROVEFILE);
                f_access = path_validate(fdir);
                break;
            case 'b':
                bflag = !bflag;
                break;
            case 'r':
                rflag = !rflag;
                break;
            case 'u':
                uflag = !uflag;
                break;
            case 'l':
                if(optarg && *optarg){
                    lmin = atoi(optarg);
                }
                else printf("Using default word length: %d\n", lmin);
                arg_validate_l(lmin);
                break;
            default:
                usage(1);
                break;
                //ERROR
        }
        if(opterr == '?') usage(1); //found unknown option
    }
    if( bflag || rflag || uflag ){
        //read access requred
        if(f_access != 0){
            usage(5);
        }
    }

    int onum = bflag + rflag + uflag;
    //mutually exclusive options combined
    if(onum > 1) usage(1);
    

    if(!onum){
        //Search trovefile since other options not provided
        if(argc - optind != 1) {
            //Program only accepts a single word at a time
            usage(1);
        }

        extract(fdir);//placeholder

        char *word = argv[optind];
        searchTrove(fdir, word);
    }
    else{

        printf("b/r/y trove.\n");
        //make hash table for path list
        HASHFILE *ptable;
        ptable = make_hashfile();

        //make lists of files/folderes for parsing
        LISTFILE *filelist =  NULL;
        LISTFILE *folderlist = NULL;

        DIR *dirp;
 
        for(;optind < argc; optind++){
            //parse remaining arguments (file list) and change to absolute path
            char path[PATH_MAX];
            realpath(argv[optind], path);
            if(path == NULL){
                perror("main, realpath Error");
                usage(1);
                exit(EXIT_FAILURE);
            }
            if(path_validate(path)){
                //path invalid
                usage(6);
                exit(EXIT_FAILURE);
            }
            //all dirs OK
            if((dirp = opendir( path)) != NULL){
            printf("main  ::  found a directory in args: %s.\n", path);
            folderlist = add_listfile(folderlist, path);
            } 
            else{
            hashfile_add(ptable, path); 
            printf("main  ::  found a file in args: %s\n", path);
            filelist = add_listfile(filelist, path);

            }

        }

        if(bflag){
            
            filelist = findFiles(filelist, folderlist, ptable, lmin, fdir );//find files in directories
        }
        if(rflag){
            trimTrove(fdir, argv);//placeholder
        }
        if(uflag){
            updateTrove(fdir, argv);//placeholder
        }

        printf("=== PRINTING FILELIST FROM LIST ===\n");
        while(filelist != NULL){
            printf(" %s\n", filelist->path);
            filelist = filelist->next;
        }
        compress(fdir);//placeholder
    }

    //optind is index of first non option argument
    //check_files(FILE LIST);  //check that the files in file list exist
    //import_trove(trovefile); //load trove into memory
    //check_trove(trovefile in memry); //check that files indexed still exist
    return 0;
}

void arg_validate_l(int arg){
    if( arg <= 0 ){
        usage(1);
        exit(EXIT_FAILURE);
    }
}

void usage(int ecode){
    switch(ecode){
        case 1:
            printf("Error.  Program command line options/arguments incorrect.\n"
                    "Usage: ./trove [-f trovefile] [-b | -r | -u] [-l length] filelist\n"
                    "or   : ./trove [-f trovefile] word\n");
            break;
        case 2:
            printf("Error: [-f trovefile], path argument inaccessible.\n");
            return;
            break;
        case 3:
            printf("Error: [-f trovefile], do not have read access to file.\n");
            return;
            break;
        case 4:
            printf("Error: [-f trovefile], do not have write access to file.\n");
            return;
            break;
        case 5:
            printf("Error: [-b | -r | -u] options selected, but no write access to trovefile.\n");
            break;
        case 6:
            printf("Error: filelist.  Invalid path provided.\n");
            break;
        default:
            printf("SOME SYSTEM ERROR OR SOMETHING\n");
            break;
    }
    exit(EXIT_FAILURE);
}
