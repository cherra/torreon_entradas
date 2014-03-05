#!/bin/sh

gcc -o callbacks.o -c callbacks.c $(pkg-config --cflags --libs gtk+-2.0 gmodule-2.0)
gcc -o entradas.o -c entradas.c $(pkg-config --cflags --libs gtk+-2.0 gmodule-2.0)
make
