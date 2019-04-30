#Written by Will Sumner
#Create parsing program
#yaccprog.out: parser.tab.c lex.yy.c ast.c ast.h symtable.c symtable.h
#	$(CC) -o yaccprog.out parser.tab.c lex.yy.c ast.c symtable.c -ll -ly

CC        := gcc # compiler
MKDIR_P   := mkdir -p # mkdir command
CFLAGS    := -Wall # cflags
DFLAGS    := -g -DDEBUG -O0 # debug flags
LDFLAGS   := -ll -ly # lex and yacc flags

# dir names
RELDIR    := release
DEBDIR    := debugging

# For building project
REGFILES  := symtable errors ast codegen stack # all handwritten src files
ALLFILES  := $(REGFILES) parser.tab lex.yy # all needed files
OBJFILES  := $(addprefix $(RELDIR)/, $(addsuffix .o,$(ALLFILES)))
DOBJFILES := $(addprefix $(DEBDIR)/, $(addsuffix .debug.o,$(ALLFILES)))

# For making a tar
CFILES    := $(addsuffix .c,$(REGFILES))
HFILES    := $(addsuffix .h,$(REGFILES)) yacc_header.h id.h
SHFILES   := compile.sh run.sh
TARFILES  := $(CFILES) $(HFILES) $(SHFILES) parser.y lexer.l oldmakefile
PREPOUT   := cminus.tar

TARGET    := cminus
DTARGET   := cminusdebug
.PHONY    := clean debug-symtable tar

all: $(TARGET)
debug: $(DTARGET)
tar: $(PREPOUT)

# Tar when any src files change or prep script changes
$(PREPOUT): $(TARFILES)
	tar -cvf $(PREPOUT) $(TARFILES)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJFILES)

$(DTARGET): $(DOBJFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DFLAGS) -o $(DTARGET) $(DOBJFILES)

$(OBJFILES):  | $(RELDIR)
$(DOBJFILES): | $(DEBDIR)

$(RELDIR):
	${MKDIR_P} $(RELDIR)

$(DEBDIR):
	${MKDIR_P} $(DEBDIR)


###################################
#########    RELEASE    ###########
###################################

$(RELDIR)/parser.tab.o: parser.tab.c yacc_header.h
	$(CC) $(CFLAGS) -c parser.tab.c -o $@

$(RELDIR)/lex.yy.o: lex.yy.c
	$(CC) -o $@ -c lex.yy.c

$(RELDIR)/symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c -o $@

$(RELDIR)/errors.o: errors.c errors.h yacc_header.h
	$(CC) $(CFLAGS) -c errors.c -o $@

$(RELDIR)/ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c -o $@

$(RELDIR)/codegen.o: codegen.c codegen.h ast.c ast.h
	$(CC) $(CFLAGS) -c codegen.c -o $@

$(RELDIR)/stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c -o $@

###################################
#########     DEBUG     ###########
###################################

$(DEBDIR)/parser.tab.debug.o: parser.tab.c yacc_header.h
	$(CC) $(CFLAGS) $(DFLAGS) -c parser.tab.c -o $@

$(DEBDIR)/lex.yy.debug.o: lex.yy.c
	$(CC) $(DFLAGS) -c lex.yy.c -o $@ # no cflags due to warnings

$(DEBDIR)/symtable.debug.o: symtable.c symtable.h
	$(CC) $(CFLAGS) $(DFLAGS) -c symtable.c -o $@

$(DEBDIR)/errors.debug.o: errors.c errors.h yacc_header.h
	$(CC) $(CFLAGS) $(DFLAGS) -c errors.c -o $@

$(DEBDIR)/ast.debug.o: ast.c ast.h
	$(CC) $(CFLAGS) $(DFLAGS) -c ast.c -o $@

$(DEBDIR)/codegen.debug.o: codegen.c codegen.h
	$(CC) $(CFLAGS) $(DFLAGS) -c codegen.c -o $@

$(DEBDIR)/stack.debug.o: stack.c stack.h
	$(CC) $(CFLAGS) $(DFLAGS) -c stack.c -o $@

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
	-@rm *.ast *.out *.asm 2>/dev/null || true # remove output files
	-@rm $(PREPOUT) 2>/dev/null || true # remove tar file
