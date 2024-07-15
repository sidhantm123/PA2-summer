# Compiler
CC = gcc

CFLAGS = -g -std=c11 -Wall -Wextra
EXECUTABLE = pa2
SRC_C = collision_dectector.c

default: $(EXECUTABLE)

$(EXECUTABLE): $(SRC_C)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRC_C)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean: 
	rm -f $(EXECUTABLE) *.o