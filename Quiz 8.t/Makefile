CC : gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = Quiz8T
SRC = Quiz8T.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)