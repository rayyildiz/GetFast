ODIR = obj
CC = gcc
CFLAGS = -g -Wall -W -pedantic
LDFLAGS = -pthread

SOURCES = main.c head.c misc.c url.c file.c download.c
OBJS = main.o head.o misc.o url.o file.o download.o

all: $(OBJS)
	mkdir -p bin
	$(CC) -c $(SOURCES) $(CFLAGS)
	$(CC) -o bin/GetFast $(OBJS) $(LDFLAGS)
	rm -f *.o

clean: 
	rm -f GetFast *.o core.* *~ bin/*

c:
	rm -f *core* *~ *log