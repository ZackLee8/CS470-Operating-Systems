CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = lab2
SRC = Lab2.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
