
CC=gcc
OPT=0
CFLAGS=-O$(OPT) -Wall -DOPT=\"$(OPT)\" -DCC=\"$(CC)\"
LDFLAGS=-lrt

OBJ=main.o stride.o info.o

all : memlab

memlab : $(OBJ)
	$(CC) $(LDFLAGS) -o memlab $^

$(OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : all