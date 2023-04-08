main: main.o io.o
	ld main.o io.o -o main
main.o: main.s
	nasm -g -f elf64 -F dwarf main.s
io.o: io.s
	nasm -g -f elf64 -F dwarf io.s
