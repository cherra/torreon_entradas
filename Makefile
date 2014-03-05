CC	= gcc
CFLAGS	= `pkg-config --cflags gtk+-2.0 gmodule-2.0`
LIBS	= `pkg-config --libs   gtk+-2.0 gmodule-2.0` -L/usr/lib/mysql -lmysqlclient -lz
DEBUG	= -Wall -g 

OBJECTS	= entradas.o callbacks.o

.PHONY: clean

all: entradas

entradas: $(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o $@ $(LIBS)  $(CFLAGS)

entradas.o: entradas.c support.h
	$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@

callbacks.o: callbacks.o support.h
	$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o principal
