# https://stackoverflow.com/questions/8946797/gcc-options-for-strictest-c-code
# https://stackoverflow.com/questions/154630/recommended-gcc-warning-options-for-c
FLAGS = -Wall -Wextra -std=c99 -pedantic -Wno-gnu -Wmissing-prototypes -Wpointer-arith\
                 -Wshadow -Wcast-qual -Wstrict-prototypes -Wold-style-definition\
                 -Wno-unused-parameter -O2 -g

all: lab_05 obj obj/clist.o obj/main.o

obj:
		if [ ! -d "obj" ]; then mkdir obj; fi

obj/clist.o: include/clist.h src/clist.c
		gcc $(FLAGS) -c src/clist.c -o $@ -I include/

obj/point_list.o: include/point_list.h include/clist.h src/point_list.c
		gcc $(FLAGS) -c src/point_list.c -o $@ -I include/

obj/main.o: include/clist.h include/point_list.h src/main.c
		gcc $(FLAGS) -c src/main.c -o $@ -I include/

lab_05: obj obj/clist.o obj/point_list.o obj/main.o
		gcc $(FLAGS) obj/*.o -o $@

run: lab_05
		./lab_05 $(ARGS)

clean:
		if [ -d "obj" ]; then rm -rf obj; fi

.PHONY: all clean run