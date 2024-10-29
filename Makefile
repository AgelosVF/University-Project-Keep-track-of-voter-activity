CC=gcc
CFLAGS= -Wall -g

All: ScanMenu.o LinkedList.o HashTable.o my_free.o
	$(CC) $(CFLAGS) ScanMenu.o LinkedList.o HashTable.o my_free.o MVote.h -o mvote

ScanMenu.o: MVote.h ScanMenu.c
	$(CC) $(CFLAGS) -c ScanMenu.c 

LinkedList.o: LinkedList.c MVote.h
	$(CC) $(CFLAGS) -c LinkedList.c

HashTable.o: HashTable.c MVote.h 
	$(CC) $(CFLAGS) -c HashTable.c 

my_free.o: my_free.c
	$(CC) $(CFLAGS) -c my_free.c 

clean:
	rm mvote my_free.o HashTable.o LinkedList.o ScanMenu.o
