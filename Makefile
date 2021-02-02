flags = -lncurses
all:
	cc -o jet *.c $(flags)
test:
	cc -o jet *.c $(flags) && ./jet

clean:
	rm jet
