
#Create yacc program
yaccprog.out: parser.tab.c lex.yy.c ast.c ast.h symtable.c symtable.h
	gcc -o yaccprog.out parser.tab.c lex.yy.c ast.c symtable.c -ll -ly

debug-yacc: parser.tab.c lex.yy.c ast.c ast.h symtable.c symtable.h
	gcc -g -o yaccprog.out parser.tab.c lex.yy.c ast.c symtable.c -ll -ly

#Create y.tab.c
parser.tab.c: parser.y lexer.l
	yacc -dv parser.y -o parser.tab.c

# Create lex.yy.c
lex.yy.c: lexer.l
	lex lexer.l

clean:
	-@rm lex.yy.c *.lex *.out 2>/dev/null || true
	-@rm parser.tab.h parser.tab.c || true

# rules for testing against binary output
run: lex-out.lex lexico-out.lex
	diff -yb lex-out.lex lexico-out.lex
lex-out.lex: lexprog.out main.c
	./lexprog.out main.c lex-out.lex
lexico-out.lex: main.c
	noah ./lexico main.c lexico-out.lex

debug-symtable: sym.out
	lldb ./sym.out
sym.out: test-table.c symtable.c symtable.h
	gcc -g -DDEBUG test-table.c symtable.c -o sym.out

