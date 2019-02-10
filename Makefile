# Create lexer program
lexprog.out: lex.yy.c
	gcc lex.yy.c -o lexprog.out

# Create lex.yy.c
lex.yy.c: lexer.l
	lex lexer.l

clean:
	rm lex.yy.c *.lex *.out

# rules for testing against binary output
run: lex-out.lex lexico-out.lex
	diff -yb lex-out.lex lexico-out.lex
lex-out.lex: lexprog.out main.c
	./lexprog.out main.c lex-out.lex
lexico-out.lex: main.c
	noah ./lexico main.c lexico-out.lex
