#Written by Will Sumner
#Create parsing program
#yaccprog.out: parser.tab.c lex.yy.c ast.c ast.h symtable.c symtable.h
#	gcc -o yaccprog.out parser.tab.c lex.yy.c ast.c symtable.c -ll -ly

CC       = gcc
CFLAGS   = -Wall
DFLAGS   = -g -DDEBUG
LDFLAGS  = -ll -ly
FILES    = parser.tab.c lex.yy.c symtable.c errors.c ast.c
OBJFILES = parser.tab.o lex.yy.o symtable.o errors.o ast.o
DOBJFILES = parser.tab.debug.o lex.yy.debug.o symtable.debug.o errors.debug.o ast.debug.o
TARGET   = yaccprog.out
DTARGET   = yaccdebug.out
.PHONY   = clean debug-symtable

all: $(TARGET)
debug: $(DTARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

$(DTARGET): $(DOBJFILES)
	$(CC) $(DFLAGS) $(CFLAGS) -o $(DTARGET) $(OBJFILES) $(LDFLAGS)

parser.tab.o: parser.tab.c parser.tab.h yacc_header.h
	gcc -c parser.tab.c -o parser.tab.o

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c -o lex.yy.o

symtable.o: symtable.c symtable.h
	gcc -c symtable.c -o symtable.o

errors.o: errors.c errors.h yacc_header.h
	gcc -c errors.c -o errors.o

ast.o: ast.c ast.h
	gcc -c ast.c -o ast.o

parser.tab.debug.o: parser.tab.c parser.tab.h yacc_header.h
	gcc $(DFLAGS) -c parser.tab.c -o parser.tab.debug.o

lex.yy.debug.o: lex.yy.c
	gcc $(DFLAGS) -c lex.yy.c -o lex.yy.debug.o

symtable.debug.o: symtable.c symtable.h
	gcc $(DFLAGS) -c symtable.c -o symtable.debug.o

errors.debug.o: errors.c errors.h yacc_header.h
	gcc $(DFLAGS) -c errors.c -o errors.debug.o

ast.debug.o: ast.c ast.h
	gcc $(DFLAGS) -c ast.c -o ast.debug.o


#Create parser.tab.c and parser.tab.h
parser.tab.c: parser.y lexer.l
	yacc -dv parser.y -o parser.tab.c

#Create lex.yy.c
lex.yy.c: lexer.l
	lex lexer.l

clean:
	-@rm -f $(OBJFILES) $(TARGET) 2>/dev/null || true
	-@rm -f $(DOBJFILES) $(DTARGET) 2>/dev/null || true
	-@rm lex.yy.c *.lex 2>/dev/null || true # remove lex files
	-@rm parser.tab.h parser.tab.c 2>/dev/null || true # remove parser files
	-@rm -r parser.output *.dSYM/ 2>/dev/null || true # remove parser output files as well as debug folders

# rules for testing components
debug-symtable: sym.out
	lldb ./sym.out
sym.out: test-table.c symtable.c symtable.h
	gcc -g -DDEBUG test-table.c symtable.c -o sym.out

