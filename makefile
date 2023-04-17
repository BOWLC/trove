
PROJECT = trove
HEADERS = $(PROJECT).h
OBJ = trove.o hash.o

C11 = cc -std=c11
CFLAGS = -Wall -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm

trove.o : trove.c $(HEADERS)
	$(C11) $(CFLAGS) -c trove.c

hash.o : hash.c $(HEADERS)
	$(C11) $(CFLAGS) -c hash.c

clean:
	rm -f $(PROJECT) $(OBJ)
