# CMinus-Compiler
A basic cminus compiler for the MIPS instruction set

This project can generates code for the SPIM simulator, but with some slight modifications will work with the MARS simulator. 

## To Use:
- run `make`
- use `./run.sh src-file output-file` to generate the assembly file
- use your favorite spim simulator to run the generated file 

## Supports:
- `int` and `void` function types
- `int` and `int[]` variable types
- recursive function calls
- the `+`, `-`, `/`, and `*` operators
- the functions `output(int x)` and `input()` provide basic I/O 

## Known Limitations:
- No `float`/`double` support
- No negative number support
- No code optimizations have been implemented
- AST generation could be simplified, a lot of extra nodes are generated
- Errors are only provided after running `make debug`
- Variables must be declared before use

## Example File:

```
int fib(int x) { /* return the xth fibonacci number */
    if (x <= 2)
        return 1;
    return fib(x-1) + fib(x-2);
}

void main(void) {
    int x;
    x = input();
    while (x > 0) {
        output(fib(x));
        x = input();
    }
    return;
}
```
  
