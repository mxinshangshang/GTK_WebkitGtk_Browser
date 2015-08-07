CC=gcc
FLAGS= -g `pkg-config --libs --cflags webkitgtk-3.0`

AwayBrowser: main.o ui.o callback.o browser.o ui.h callback.h browser.h
	$(CC) -o AwayBrowser main.o ui.o callback.o browser.o $(FLAGS)

main.o: main.c ui.h callback.h browser.h main.ui
	$(CC) -c main.c $(FLAGS)

ui.o: ui.c ui.h
	$(CC) -c ui.c $(FLAGS)

callback.o: callback.c callback.h
	$(CC) -c callback.c $(FLAGS)

browser.o: browser.c browser.h
	$(CC) -c browser.c $(FLAGS)

PHONY: check-syntax

check-syntax:
	$(CC) -o /dev/null -S $(CHK_SOURCES) $(FLAGS)
