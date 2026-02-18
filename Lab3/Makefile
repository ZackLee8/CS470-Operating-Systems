CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = myshell
SRC = myshell.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)