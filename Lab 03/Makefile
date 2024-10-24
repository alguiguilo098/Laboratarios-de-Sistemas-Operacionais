CC=gcc
CFLAGS=-I.
DEPS = matriz.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

matriz: matriz.o test_matriz.o 
	$(CC) -o test_matriz matriz.o test_matriz.o

clean: 
	rm *.o test_matriz

questao1:
	gcc matriz.c Exercicio01.c -pthread -o main
	./main

questao2:
	gcc matriz.c Exercicio02.c -pthread -lm -o main
	./main
	rm main