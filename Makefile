SRC = src/main.c \
      src/lexer.c \
      src/token.c

OBJ = $(SRC:src/%.c=bin/%.o)
TARGET = b8

CFLAGS = -ggdb

run: $(TARGET)
	./$(TARGET)

all: clean run

$(TARGET): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

bin/%.o: src/%.c | bin
	gcc -c -o $@ $< $(CFLAGS)

bin:
	mkdir -p bin

clean:
	rm -rf $(OBJ) $(TARGET)
