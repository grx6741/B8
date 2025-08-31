SRC = src/lexer.c \
      src/token.c \
      src/ast.c \
      src/parser.c \
      src/codegen.c \
      src/ht.c

OBJ = $(SRC:src/%.c=bin/%.o)
TARGET = b8

OUT = output.asm
IN = input.b8

CFLAGS   = -ggdb
LD_FLAGS = -lm

test: $(OBJ) src/test.c
	gcc -c -o bin/test.o src/test.c
	gcc -o bin/test $(OBJ) bin/test.o $(CFLAGS) $(LD_FLAGS)
	./bin/test

$(TARGET): $(OBJ) src/main.c
	gcc -c -o bin/main.o src/main.c
	gcc -o $@ $(OBJ) bin/main.o $(CFLAGS) $(LD_FLAGS)

$(OUT): $(TARGET)
	./$(TARGET) $(IN) $(OUT)

run: $(OUT)
	./8bit-computer/asm/asm.py $(OUT) > 8bit-computer/memory.list
	make run -C 8bit-computer

all: clean run

bin/%.o: src/%.c | bin
	gcc -c -o $@ $< $(CFLAGS)

bin:
	mkdir -p bin

clean:
	rm -rf $(OBJ) $(TARGET)
	rm -f $(OUT) 8bit-computer/memory.list
