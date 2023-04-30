SOURCES := $(wildcard src/*.rf)
ASM_FILE := $(patsubst src/%.rf, out/%.s, $(SOURCES))
OBJECTS := $(patsubst out/%.s, out/%.o, $(ASM_FILE))

default:
	@echo "Default make target"

.PHONY: always clean build
build: always out/main
always:
	mkdir -p out/

#clean:
#	rm -r out

out/main: $(OBJECTS)
	ld $^ -o $@

out/%.o: out/%.s
	nasm -g -f elf64 -F dwarf $^ -o $@

out/%.s: src/%.rf
	rfc -f $^ -o $@ -p x86_64-linux
