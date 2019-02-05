lexprog: lex.yy.c
	gcc -ll lex.yy.c -o lexprog
lex.yy.c: lexer.l
	lex lexer.l
clean:
	rm lex.yy.c lexprog *.lex
run: lex-out.lex lexico-out.lex
	diff -yb lex-out.lex lexico-out.lex
lex-out.lex: lexprog main.c
	./lexprog main.c lex-out.lex
lexico-out.lex: main.c
	noah ./lexico main.c lexico-out.lex
