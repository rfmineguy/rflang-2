out/main: main.o io.o
	ld main.o io.o -o main
out/main.o: main.s
	nasm -g -f elf64 -F dwarf main.s -o $@
out/io.o: io.s
	nasm -g -f elf64 -F dwarf io.s -o $@
