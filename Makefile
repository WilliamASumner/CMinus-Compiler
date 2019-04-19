#Written by Will Sumner
#Create parsing program
#yaccprog.out: parser.tab.c lex.yy.c ast.c ast.h symtable.c symtable.h
#	gcc -o yaccprog.out parser.tab.c lex.yy.c ast.c symtable.c -ll -ly

CC        := gcc
MKDIR_P   := mkdir -p
CFLAGS    := -Wall
DFLAGS    := -g -DDEBUG -O0
LDFLAGS   := -ll -ly

RELDIR    := release
DEBDIR    := debugging

FILES     := parser.tab lex.yy symtable errors ast codegen stack
OBJFILES  := $(addprefix $(RELDIR)/, $(addsuffix .o,$(FILES)))
DOBJFILES := $(addprefix $(DEBDIR)/, $(addsuffix .debug.o,$(FILES)))

TARGET    := yaccprog.out
DTARGET   := yaccdebug.out
.PHONY    := clean debug-symtable directories

all: $(TARGET)
debug: $(DTARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

$(DTARGET): $(DOBJFILES)
	$(CC) $(DFLAGS) $(CFLAGS) -o $(DTARGET) $(DOBJFILES) $(LDFLAGS)

$(OBJFILES):  | $(RELDIR)
$(DOBJFILES): | $(DEBDIR)

$(RELDIR):
	${MKDIR_P} $(RELDIR)

$(DEBDIR):
	${MKDIR_P} $(DEBDIR)


###################################
#########    RELEASE    ###########
###################################

$(RELDIR)/parser.tab.o: parser.tab.c parser.tab.h yacc_header.h
	gcc $(CFLAGS) -c parser.tab.c -o $@

$(RELDIR)/lex.yy.o: lex.yy.c
	gcc -o $@ -c lex.yy.c

$(RELDIR)/symtable.o: symtable.c symtable.h
	gcc $(CFLAGS) -c symtable.c -o $@

$(RELDIR)/errors.o: errors.c errors.h yacc_header.h
	gcc $(CFLAGS) -c errors.c -o $@

$(RELDIR)/ast.o: ast.c ast.h
	gcc $(CFLAGS) -c ast.c -o $@

$(RELDIR)/codegen.o: codegen.c codegen.h ast.c ast.h
	gcc $(CFLAGS) -c codegen.c -o $@

$(RELDIR)/stack.o: stack.c stack.h
	gcc $(CFLAGS) -c stack.c -o $@

###################################
#########     DEBUG     ###########
###################################

$(DEBDIR)/parser.tab.debug.o: parser.tab.c parser.tab.h yacc_header.h
	gcc $(CFLAGS) $(DFLAGS) -c parser.tab.c -o $@

$(DEBDIR)/lex.yy.debug.o: lex.yy.c
	gcc $(DFLAGS) -c lex.yy.c -o $@

$(DEBDIR)/symtable.debug.o: symtable.c symtable.h
	gcc $(CFLAGS) $(DFLAGS) -c symtable.c -o $@

$(DEBDIR)/errors.debug.o: errors.c errors.h yacc_header.h
	gcc $(CFLAGS) $(DFLAGS) -c errors.c -o $@

$(DEBDIR)/ast.debug.o: ast.c ast.h
	gcc $(CFLAGS) $(DFLAGS) -c ast.c -o $@

$(DEBDIR)/codegen.debug.o: codegen.c codegen.h
	gcc $(CFLAGS) $(DFLAGS) -c codegen.c -o $@

$(DEBDIR)/stack.debug.o: stack.c stack.h
	gcc $(CFLAGS) $(DFLAGS) -c stack.c -o $@


#Create parser.tab.c and parser.tab.h
parser.tab.c: parser.y lexer.l
	yacc -dv parser.y -o $@

#Create lex.yy.c
lex.yy.c: lexer.l
	lex -o $@ lexer.l 

clean:
	-@rm -rf $(RELDIR)/ $(DEBDIR)/ 2>/dev/null || true
	-@rm lex.yy.c *.lex 2>/dev/null || true # remove lex files
	-@rm parser.{output,tab.{h,c}}  2>/dev/null || true # remove parser files
	-@rm $(TARGET) $(DTARGET)  2>/dev/null || true # remove targets
	-@rm *.ast *.out 2>/dev/null || true # remove .ast output files
