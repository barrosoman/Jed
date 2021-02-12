all:
	cc -o jed *.c $(flags)
test:
	cc -o jed *.c $(flags) && ./jed

clean:
	rm jed
