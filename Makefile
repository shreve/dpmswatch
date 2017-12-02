all: main.c
	g++ main.c -o dpmswatch -lX11 -lXext -Wall -O2
