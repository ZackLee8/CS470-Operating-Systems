CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = Zack_TestFC
SRC = Zack_TestFC.c Zack_LibFC.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)