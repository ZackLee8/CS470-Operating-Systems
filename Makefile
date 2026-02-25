CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = Lab4
SRC = Lab4.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)