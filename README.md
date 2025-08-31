# B8

B8 is a very minimal Programming language for this ![8 bit Computer](https://github.com/lightcode/8bit-computer).
To Build this Compiler see the ![BUILDING](##Building) section

## Example

A small program that prints all fibbonacci numbers below 100

```rust
int a;
int b;

a = 0;
b = 1;

loop {
	out(a);

	int c;
	c = a + b;
	b = a;
	a = c;

	if (a > 100) {
		break;
	} else {}
}
```

## Syntax

1. Declare variables in the Current Scope

```cpp
int a;
int b;
```

2. Assign Values to declared variables in current stack or the previous stacks. Only supports positive 8 bit integers `uint8_t`

```cpp
a = 20;
```

3. Only Binary Operations

```cpp
a = 20 + b;
b = a + 16;
a = a - b;
```

4. If Statements with mandatory else sections

```cpp
if ( a > b ) {
    a = a + 10;
} else {
    b = b - 20;
}
```

5. Basic Loops with support for break statement

```rust
loop {
    a = a + 1;
    if ( a >= 20 ) {
        break;
    }
}
```

6. Basic Logging with `out()`

```cpp
int a;
a = 20;
out(a);
```

## Limitations

1. Identifiers can only have a name of max length 255 (`MAX_IDENTIFIER_NAME_LEN`).
2. A New scope is only created inside an if-else statement or a loop statement. So this is invalid.

```cpp
int a;
{
    int b;
    b = 20;
}
```

3. A B8 program can only have a max scope depth of 255 (`MAX_SCOPE_DEPTH`).
4. No support for user defined functions. A program can only call `out()` function with only one arguement.
5. No Optimized Register Allocation is done. Each access to a variable is done by reading from the RAM.
6. Only Binary Expressions are supported. So this is invalid.

```cpp
int a;
a = 2 * 3 + 5;
```

## Building

The 8-bit computer repo requires python2, so make sure you have python2 available on your PATH.

To install the required dependencies on ubuntu (should work on any debian derivatives, I've only tested on Debian and Ubuntu), run this command.

```bash
sudo apt install iverilog gcc make
```

Clone this repo along with the submodules

```bash
git clone https://github.com/grx6741/B8.git --recursive
```

Running `make` should compile `b8` binary in the project's root directory and also compile the example fibbonacci code and run it.

```bash
make
```

Now you can use the `b8` compiler with any custom input file

```bash
echo "int a; a = 10; out(a);" > custom_input.b8
./b8 custom_input.b8 output.asm
./8bit-computer/asm/asm.py output.asm > 8bit-computer/memory.list
make run -C 8bit-computer
```
