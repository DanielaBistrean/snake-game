build:
	gcc -o snake src/main.c -lncurses -Wall
run:
	./snake
clean:
	rm snake
buildrun: build run


