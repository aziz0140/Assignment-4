# Do not edit the contents of this file.
CC = gcc
CFLAGS = -Werror -Wall -g -std=gnu99
LDFLAGS = -lrt -lpthread -lm
TARGET = Question1 Question2
OBJFILES = Question1.o Question2.o
all: $(TARGET)
Question1: 180801040_18086050_a04_q1.c
	$(CC) $(CFLAGS) -o Question1 180801040_18086050_a04_q1.c $(LDFLAGS)
Question2: 180801040_18086050_a04_q2.c
	$(CC) $(CFLAGS) -o Question2 180801040_18086050_a04_q2.c $(LDFLAGS)

runq1: Question1
	./Question1 10 5 7 8
runq2: Question2
	./Question2 1000000
clean:
	rm -f $(OBJFILES) $(TARGET) *~ 