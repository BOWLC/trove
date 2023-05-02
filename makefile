
PROJECT = trove
HEADERS = $(PROJECT).h
OBJ = trove.o trovefile.o hash.o zipcat.o lists.o

C11 = cc -std=c11 
CFLAGS = -Wall -Werror

$(PROJECT) : $(OBJ)
	$(C11) -g $(CFLAGS) -o $(PROJECT) trove.o trovefile.o hash.o zipcat.o lists.o -lm

trove.o : trove.c trove.h 
	$(C11) $(CFLAGS) -c trove.c

trovefile.o : trovefile.c trovefile.h
	$(C11) $(CFLAGS) -c trovefile.c

hash.o : hash.c hash.h
	$(C11) $(CFLAGS) -c hash.c

zipcat.o : zipcat.c
	$(C11) $(CFLAGS) -c zipcat.c

lists.o : lists.c lists.h
	$(C11) $(CFLAGS) -c lists.c

clean:
	rm -f $(PROJECT) $(OBJ)
