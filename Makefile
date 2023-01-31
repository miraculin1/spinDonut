all: donut.c
	gcc -o donut donut.c -lm -lncurses -g -fsanitize=address
