matadd.out: matadd.o main.o
	gcc -g -Wall -m32 -o matadd.out main.o matadd.o

main.o: main.c
	gcc -c -g -Wall -m32 -o main.o main.c

matadd.o: matadd.s
	gcc -c -g -Wall -m32 -o matadd.o matadd.s

clean:
	rm -fr *.o *.out
