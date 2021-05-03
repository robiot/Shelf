exec = shelf
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g


$(exec): $(objects)
	gcc $(wildcard build/src/*.o) $(flags) -o $(exec)

%.o: %.c include/%.h
	mkdir -p build/src
	gcc -c $(flags) $< -o build/$@


install:
	make

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
