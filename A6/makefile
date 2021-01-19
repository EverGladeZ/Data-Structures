all: buildidx key2val val2key actors bacon1

actors: hashfn.o util.o actors.o
	$(CC) $(GCC) -o actors hashfn.o util.o actors.o

bacon1: hashfn.o util.o bacon1.o
	$(CC) $(GCC) -o bacon1 hashfn.o util.o bacon1.o

buildidx: hashfn.o util.o buildidx.o
	$(CC) $(GCC) -o buildidx hashfn.o util.o buildidx.o

key2val: hashfn.o util.o key2val.o
	$(CC) $(GCC) -o key2val hashfn.o util.o key2val.o

val2key: hashfn.o util.o val2key.o
	$(CC) $(GCC) -o val2key hashfn.o util.o val2key.o

bacon1.o: bacon1.c hashfn.h util.h
	$(CC) $(GCC) -c bacon1.c

actors.o: actors.c hashfn.h util.h
	$(CC) $(GCC) -c actors.c
	
buildidx.o: buildidx.c hashfn.h util.h
	$(CC) $(GCC) -c buildidx.c

key2val.o: key2val.c hashfn.h util.h
	$(CC) $(GCC) -c key2val.c

val2key.o: val2key.c hashfn.h util.h
	$(CC) $(GCC) -c val2key.c

hashfn.o: hashfn.c hashfn.h
	$(CC) $(GCC) -c hashfn.c

util.o: util.c util.h
	$(CC) $(GCC) -c util.c

clean: 
	rm *.o buildidx key2val val2key actors bacon1