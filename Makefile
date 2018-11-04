SOURCES = main.c head.c misc.c url.c file.c download.c
OBJS = main.o head.o misc.o url.o file.o download.o
CFLAGS = -g -Wall -W -pedantic 
LDFLAGS = -pthread
CC = gcc

all: $(OBJS)
	$(CC) -c $(SOURCES)
	$(CC) -o GetFast $(OBJS) $(LDFLAGS)

clean: 
	rm -f GetFast *.o core.* *~

c:
	rm -f *core* *~ *log