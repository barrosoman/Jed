flags = -lncurses
all:
	cc -o jet *.c $(flags)

clean:
	rm jet
