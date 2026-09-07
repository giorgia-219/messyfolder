CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = messyfolder

SRC = src/main.c src/scanner.c src/utils.c src/organizer.c src/duplicates.c
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean