CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: test_getbytes test_getbits test_bits2ull test_bits2ll test_spff test_dpff

test_getbytes: bandb.o test_getbytes.o
	$(CC) $(GCC) -o test_getbytes bandb.o test_getbytes.o

test_getbits: bandb.o test_getbits.o
	$(CC) $(GCC) -o test_getbits bandb.o test_getbits.o

test_bits2ull: bandb.o test_bits2ull.o
	$(CC) $(GCC) -o test_bits2ull bandb.o test_bits2ull.o

test_bits2ll: bandb.o test_bits2ll.o
	$(CC) $(GCC) -o test_bits2ll bandb.o test_bits2ll.o

test_spff: bandb.o test_spff.o
	$(CC) $(GCC) -o test_spff bandb.o test_spff.o

test_dpff: bandb.o test_dpff.o
	$(CC) $(GCC) -o test_dpff bandb.o test_dpff.o

banb.o: bandb.c bandb.h
	$(CC) $(GCC) -c bandb.c

test_getbytes.o: test_getbytes.c bandb.h
	$(CC) $(GCC) -c test_getbytes.c

test_getbits.o: test_getbits.c bandb.h
	$(CC) $(GCC) -c test_getbits.c

test_bits2ull.o: test_bits2ull.c bandb.h
	$(CC) $(GCC) -c test_bits2ull.c

test_bits2ll.o: test_bits2ll.c bandb.h
	$(CC) $(GCC) -c test_bits2ll.c

test_spff.o: test_spff.c bandb.h
	$(CC) $(GCC) -c test_spff.c

test_dpff.o: test_dpff.c bandb.h
	$(CC) $(GCC) -c test_dpff.c

clean:
	rm *.o test_getbits test_bits2ull test_getbytes test_bits2ll test_spff test_dpff