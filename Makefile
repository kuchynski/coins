cpp: coins.cpp
	g++ -o app $^
c: coins.c storage.c
	gcc -std=c99 -o app $^
clean: rm -f *.o
