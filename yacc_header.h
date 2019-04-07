#ifndef YACC_HEADER_H
#define YACC_HEADER_H
extern int yylex(void); // lex lexer function
extern int yyparse(void); // yacc parser function
extern int yylineno; // current line number
extern FILE* yyin; // infile pointer
extern FILE* yyout; // outfile pointer

int yywrap(); // what to do on next file
#endif
