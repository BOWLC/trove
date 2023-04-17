#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <linux/limits.h>

#include "trove.h"

#define OPTLIST "f:brul:"

void searchTrove(char *trovename, char *word);
int buildTrove(char *trovename, char *paths[]);
int trimTrove(char *trovename, char *word[]);
int updateTrove(char *trovename, char *word[]);
int compress(char *trovename);
int extract(char *trovename);
void Usage(int ecode);

int main(int argc, char *argv[]){
    int opt;
    bool bflag = false;
    bool rflag = false;
    bool uflag = false;
    char fdir[PATH_MAX] =  "/tmp/trove";
    int lmin = 4;
    if(argc == 1){
        Usage(1);
        //Too few arguments
    }
    while((opt = getopt(argc, argv, OPTLIST)) !=-1){
        switch(opt){
            case 'f':
                strcpy(fdir, optarg);
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
                lmin = atoi(optarg);
                if(lmin == 0);//ERROR, MIN LENGTH OF 0 OR ARG STRING
                break;
            default:
                Usage(1);
                break;
                //ERROR
        }
    }
    searchTrove(fdir, fdir);
    buildTrove(fdir, argv);
    trimTrove(fdir, argv);
    updateTrove(fdir, argv);
    compress(fdir);
    extract(fdir);

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

int compress(char *trovename){
    printf("Compress trove file.\n");
    return 0;
}

int extract(char *trovename){
    printf("Extract trove file.\n");
    return 0;
}

void Usage(int ecode){
    switch(ecode){
        case 1:
            printf("Error.  Program command line options/arguments incorrect.\n"
                    "Usage: ./trove [-f trovefile] [-b | -r | -u] [-l length] filelist\n"
                    "or   : ./trove [-f trovefile] word\n");
            break;
        default:
            printf("SOME SYSTEM ERROR OR SOMETHING\n");
            break;
    }
    exit(EXIT_FAILURE);
}
