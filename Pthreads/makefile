
all : p320 p420 p421

p320 : p320.c p320.h
	gcc -c p320.c -o p320

p420 : p420.o p320.o
	gcc p420.o p320.o -o p420 -lpthread

p420.o : p420.c p320.h
	gcc -c p420.c

p421: p421.c
	gcc p421.c -o p421 -lpthread

clean:
	rm -f p320.o p420.o p421.o
	rm -f p320 p420 p421