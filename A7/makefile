CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: show_node count_nodes alphatoe

show_node: ttt1.o ttt2.o show_node.o
	$(CC) $(GCC) -o show_node ttt1.o ttt2.o show_node.o

count_nodes: ttt1.o ttt2.o count_nodes.o
	$(CC) $(GCC) -o count_nodes ttt1.o ttt2.o count_nodes.o

alphatoe: ttt1.o ttt2.o alphatoe.o
	$(CC) $(GCC) -o alphatoe ttt1.o ttt2.o alphatoe.o

ttt1.o: ttt1.c ttt.h
	$(CC) $(GCC) -c ttt1.c

ttt2.o: ttt2.c ttt.h
	$(CC) $(GCC) -c ttt2.c

show_node.o: show_node.c ttt.h
	$(CC) $(GCC) -c show_node.c

count_nodes.o: count_nodes.c ttt.h
	$(CC) $(GCC) -c count_nodes.c

alphatoe.o: alphatoe.c ttt.h
	$(CC) $(GCC) -c alphatoe.c

clean:
	rm *.o show_node count_nodes alphatoe