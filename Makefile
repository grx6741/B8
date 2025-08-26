SRC = src/lexer.c \
      src/token.c \
      src/ast.c \
      src/parser.c \
      src/codegen.c \
      src/ht.c

OBJ = $(SRC:src/%.c=bin/%.o)
TARGET = b8

CFLAGS   = -ggdb
LD_FLAGS = -lm

run: $(TARGET)
	./$(TARGET)

test: $(OBJ) src/test.c
	gcc -c -o bin/test.o src/test.c
	gcc -o bin/test $(OBJ) bin/test.o $(CFLAGS) $(LD_FLAGS)
	./bin/test

all: clean run

$(TARGET): $(OBJ) src/main.c
	gcc -c -o bin/main.o src/main.c
	gcc -o $@ $(OBJ) bin/main.o $(CFLAGS) $(LD_FLAGS)

bin/%.o: src/%.c | bin
	gcc -c -o $@ $< $(CFLAGS)

bin:
	mkdir -p bin

clean:
	rm -rf $(OBJ) $(TARGET)
