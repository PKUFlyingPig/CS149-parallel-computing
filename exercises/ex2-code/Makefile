CC=gcc
CFLAGS=-g -O0 -std=gnu99

## Comment out the following line to have the programs print less information
DFLAGS=-DVERBOSE

LIBS=-lpthread -lm

TARGETS = rconverge1 rconverge2 rconverge3

all: $(TARGETS)

rconverge1: rconverge1.c
	$(CC) $(CFLAGS) $(DFLAGS) rconverge1.c -o rconverge1 $(LIBS)

rconverge2: rconverge2.c
	$(CC) $(CFLAGS) $(DFLAGS) rconverge2.c -o rconverge2 $(LIBS)

rconverge3: rconverge3.c
	$(CC) $(CFLAGS) $(DFLAGS) rconverge3.c -o rconverge3 $(LIBS)

clean:
	rm -f *~ *.o
	rm -f $(TARGETS)
