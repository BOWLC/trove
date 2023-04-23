#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <linux/limits.h>

#include "trove.h"

#define OPTLIST "f::brul::"
//#define _POSIX_SOURCE
#define DEFAULT_TROVEFILE "/tmp/trove"
#define DEFAULT_WORDLEN 4

void searchTrove(char *trovename, char *word);
int buildTrove(char *trovename, char *paths[]);
int trimTrove(char *trovename, char *word[]);
int updateTrove(char *trovename, char *word[]);
void usage(int ecode);
void arg_validate_l(int arg);
int arg_validate_f(char *dir);

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
                f_access = arg_validate_f(fdir);
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

    //mutually exclusive options combined
    if(( bflag && rflag) || (bflag && uflag )|| (rflag && uflag )) usage(1);

    //optind is index of first non option argument
    //check_files(FILE LIST);  //check that the files in file list exist
    //import_trove(trovefile); //load trove into memory
    //check_trove(trovefile in memry); //check that files indexed still exist
    searchTrove(fdir, fdir);
    buildTrove(fdir, argv);
    trimTrove(fdir, argv);
    updateTrove(fdir, argv);
    compress(fdir);
    extract(fdir);

    return 0;
}

void arg_validate_l(int arg){
    if( arg <= 0 ){
        usage(1);
        exit(EXIT_FAILURE);
    }
}

int arg_validate_f(char *dir){
    if(access(dir, F_OK) != 0){
        //file does not exist
        usage(2);
        return 2;
    }    
    else
        printf("File exists.\n");
    if(access(dir, R_OK) != 0){
        usage(3);
        return 3;
    }
    printf("Read access OK.\n");
    if(access(dir, W_OK) != 0){
        usage(4);
        return 4;
    }
    printf("Write access OK.\n");
    return 0;

}

void searchTrove(char *trovename, char *word){
    printf("Search trove.\n");
}

int buildTrove(char *trovename, char *paths[]){
    return 0;
    printf("Build trove.\n");
}

int trimTrove(char *trovename, char *word[]){
    printf("Trim trove.\n");
    return 0;
}

int updateTrove(char *trovename, char *word[]){
    printf("Update trove.\n");
    return 0;
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
        default:
            printf("SOME SYSTEM ERROR OR SOMETHING\n");
            break;
    }
    exit(EXIT_FAILURE);
}
