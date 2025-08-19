SRC = src/main.c \
      src/lexer.c \
      src/token.c

OBJ = $(SRC:src/%.c=bin/%.o)
TARGET = b8

CFLAGS   = -ggdb
LD_FLAGS = -lm

run: $(TARGET)
	./$(TARGET)

all: clean run

$(TARGET): $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LD_FLAGS)

bin/%.o: src/%.c | bin
	gcc -c -o $@ $< $(CFLAGS)

bin:
	mkdir -p bin

clean:
	rm -rf $(OBJ) $(TARGET)
