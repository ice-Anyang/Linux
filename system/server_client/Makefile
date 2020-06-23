.PHONY:all
all:clientPipe serverPipe
serverPipe:serverPipe.c
	gcc -o $@ $^
clientPipe:clientPipe.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f clientPipe serverPipe
