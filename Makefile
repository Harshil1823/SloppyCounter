CC = gcc
CFLAGS = -pthread

sloppySim: sloppyCounter.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f sloppySim
