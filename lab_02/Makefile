all: lab_02

obj/:
	mkdir obj

obj/str.o: obj/ include/str.h src/str.c
	gcc -c src/str.c -o obj/str.o -I include/

obj/test_str.o: obj/ include/str.h include/test_str.h src/test_str.c
	gcc -c src/test_str.c -o obj/test_str.o -I include/

obj/test.o: obj/ include/str.h include/test_str.h src/test.c
	gcc -c src/test.c -o obj/test.o -I include/

lab_02: obj/str.o obj/test.o obj/test_str.o
	gcc obj/*.o -o lab_02

run: lab_02
	./lab_02

clean:
	rm obj/*.o

.PHONY: all clean run