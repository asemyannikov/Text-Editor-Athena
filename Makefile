all: programm

programm-valgrind: list.o common.o screen.o control.o printing.o terminal.o main.o
	$(CC) list.o common.o screen.o control.o printing.o terminal.o main.o -o programm
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./programm
	rm -rf *.o programm

programm: list.o common.o screen.o control.o printing.o terminal.o main.o 
	$(CC) list.o common.o screen.o control.o printing.o terminal.o main.o -o programm
	rm -rf *.o programm

main.o: main.c
	$(CC) main.c -c main.c

list.o: list.c
	$(CC) list.c -c list.c
	
common.o: common.c
	$(CC) common.c -c common.c
	
screen.o: screen.c
	$(CC) screen.c -c screen.c
	
control.o: control.c
	$(CC) control.c -c control.c
	
printing.o: printing.c
	$(CC) printing.c -c printing.c
	
terminal.o: terminal.c
	$(CC) terminal.c -c terminal.c
	
clean:
	rm -rf *.o programm
