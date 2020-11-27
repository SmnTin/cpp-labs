# https://stackoverflow.com/questions/8946797/gcc-options-for-strictest-c-code
# https://stackoverflow.com/questions/154630/recommended-gcc-warning-options-for-c
FLAGS = -Wall -Wextra -std=c99 -pedantic -Wno-gnu -Wmissing-prototypes -Wpointer-arith\
		 -Wshadow -Wcast-qual -Wstrict-prototypes -Wold-style-definition -O -g

all: lab_03

obj/:
	if [ ! -d "obj" ]; then mkdir obj; fi

obj/mergesort.o: obj/ include/mergesort.h src/mergesort.c
	gcc $(FLAGS) -c src/mergesort.c -o $@ -I include/

obj/main.o: obj/ include/mergesort.h src/main.c
	gcc $(FLAGS) -c src/main.c -o $@ -I include/

lab_03: obj/mergesort.o obj/main.o
	gcc $(FLAGS) obj/*.o -o $@

run: lab_03
	./lab_03 $(ARGS)

clean:
	if [ -d "obj" ]; then rm -rf obj; fi

.PHONY: all clean run
