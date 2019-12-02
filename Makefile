all: maxTweeter.c
	gcc -Wall -o output.x maxTweeter.c
clean:
	rm output.x