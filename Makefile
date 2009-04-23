
CC=gcc
OPT=0
CFLAGS=-O$(OPT) -Wall -DOPT=\"$(OPT)\" -DCC=\"$(CC)\"
LDFLAGS=-lrt

CFILES=$(wildcard *.c)
OFILES=$(CFILES:.c=.o)

all : memlab

memlab : $(OFILES)
	$(CC) $(LDFLAGS) -o memlab $^

depend : .depend
.depend : $(CFILES)
	$(CC) -MM $(CFLAGS) $^ > $@

-include .depend

$(OFILES): %.o: %.c .depend
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm -rf *.o .depend

.PHONY : all depend clean
