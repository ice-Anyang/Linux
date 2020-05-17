world:world.c
	gcc $^ -o $@
hello:hello.o
	gcc hello.o -o hello
hello.o:hello.s
	gcc -c hello.s -o hello.o
hello.s:hello.i
	gcc -S hello.i -o hello.s
hello.i:hello.c
	gcc -E hello.c -o hello.i
test:test.c
	gcc $^ -o $@
.PHOUY:clean
	clean:
	rm -f hello.i hello.s hello.o hello
