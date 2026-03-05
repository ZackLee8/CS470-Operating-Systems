CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = Lab5
SRC = Lab5.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)