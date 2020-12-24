# https://stackoverflow.com/questions/8946797/gcc-options-for-strictest-c-code
# https://stackoverflow.com/questions/154630/recommended-gcc-warning-options-for-c
FLAGS = -Wall -Wextra -std=c11 -pedantic -Wno-gnu -Wmissing-prototypes -Wpointer-arith\
                 -Wshadow -Wcast-qual -Wstrict-prototypes -Wold-style-definition\
                 -Wno-unused-parameter -O0 -g

all: hw_01 obj obj/bmp.o obj/stego.o obj/main.o

obj:
		if [ ! -d "obj" ]; then mkdir obj; fi

obj/bmp.o: include/bmp.h src/bmp.c
		gcc $(FLAGS) -c src/bmp.c -o $@ -I include/

obj/stego.o: include/bmp.h include/stego.h src/stego.c
		gcc $(FLAGS) -c src/stego.c -o $@ -I include/

obj/main.o: include/bmp.h include/stego.h src/main.c
		gcc $(FLAGS) -c src/main.c -o $@ -I include/

hw_01: obj obj/bmp.o obj/main.o obj/stego.o
		gcc $(FLAGS) obj/*.o -o $@

run: hw_01
		./hw_01 $(ARGS)

clean:
		if [ -d "obj" ]; then rm -rf obj; fi

.PHONY: all clean run