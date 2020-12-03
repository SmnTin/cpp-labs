# https://stackoverflow.com/questions/8946797/gcc-options-for-strictest-c-code
# https://stackoverflow.com/questions/154630/recommended-gcc-warning-options-for-c
FLAGS = -Wall -Wextra -std=c99 -pedantic -Wno-gnu -Wmissing-prototypes -Wpointer-arith\
                 -Wshadow -Wcast-qual -Wstrict-prototypes -Wold-style-definition\
                 -Wno-unused-parameter -O0 -g

all: lab_04

obj/:
		if [ ! -d "obj" ]; then mkdir obj; fi

obj/clist.o: obj/ include/clist.h src/clist.c
		gcc $(FLAGS) -c src/clist.c -o $@ -I include/

obj/main.o: obj/ include/clist.h src/main.c
		gcc $(FLAGS) -c src/main.c -o $@ -I include/

lab_04: obj/clist.o obj/main.o
		gcc $(FLAGS) obj/*.o -o $@

run: lab_04
		./lab_04 $(ARGS)

clean:
		if [ -d "obj" ]; then rm -rf obj; fi

.PHONY: all clean run