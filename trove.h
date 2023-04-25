#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h> //for uint32_t unsigned integer

#include "hash.h"
#include "trovefile.h"

#define DEFAULT_TROVEFILE "/tmp/trove"
#define DEFAULT_WORDLEN 4

//declared here, defined in zipcat.c
extern int compress(char *trovename);

extern int extract(char *trovename);


