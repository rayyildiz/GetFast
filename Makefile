SOURCES = GetFast.c Head.c Misc.c Url.c File.c Download.c
OBJS = GetFast.o Head.o Misc.o Url.o File.o Download.o
CFLAGS = -g -Wall -W -pedantic 
LDFLAGS = -pthread
CC = gcc
STRIP = strip
	

all: $(OBJS)
	$(CC) -c $(SOURCES)
	$(CC) -o GetFast $(OBJS) $(LDFLAGS)

strip: $(all)
	$(STRIP) fastget

clean: 
	rm -f GetFast *.o core.* *~

c:
	rm -f *core* *~ *log
