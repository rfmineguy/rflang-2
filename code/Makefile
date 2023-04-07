main: main.o
	ld main.o -o main
main.o: main.s
	nasm -g -f elf64 -F dwarf main.s
