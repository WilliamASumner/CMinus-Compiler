#Written by Will Sumner

default: yaccprog.out

#Create parsing program
yaccprog.out: parser.tab.c lex.yy.c ast.c ast.h symtable.c symtable.h
	gcc -o yaccprog.out parser.tab.c lex.yy.c ast.c symtable.c -ll -ly

debug-yacc: parser.tab.c lex.yy.c ast.c ast.h symtable.c symtable.h
	gcc -g -DDEBUG -o yaccprog.out parser.tab.c lex.yy.c ast.c symtable.c -ll -ly

#Create parser.tab.c and parser.tab.h
parser.tab.c: parser.y lexer.l
	yacc -dv parser.y -o parser.tab.c

#Create lex.yy.c
lex.yy.c: lexer.l
	lex lexer.l

clean:
	-@rm lex.yy.c *.lex *.out 2>/dev/null || true # remove lex files
	-@rm parser.tab.h parser.tab.c 2>/dev/null || true # remove parser files
	-@rm -r parser.output *.dSYM/ 2>/dev/null || true # remove parser output files as well as debug folders

# rules for testing components
debug-symtable: sym.out
	lldb ./sym.out
sym.out: test-table.c symtable.c symtable.h
	gcc -g -DDEBUG test-table.c symtable.c -o sym.out

