%{
#include <stdio.h>
#include "ast.h"
#include "parser.tab.h" // grammer token list
extern YYSTYPE yylval;
void yyerror(const char* msg);
int lastline = 1; // val for returning comment error

// the %x COMMT sets it as a state once a OCOMM is seen
%}

%x COMMT

DIG [0-9]
CHAR [a-zA-Z]
INT_TOK {DIG}{DIG}*
IDSTR {CHAR}({DIG}|{CHAR})*
OCOMM \/\*
CCOMM \*\/
WHITESPACE [ \t]+
ERROR .

%%
{OCOMM}           {BEGIN(COMMT); lastline = yylineno;}
<COMMT>[^*\n]*    {/* capture anything that is not a newline */}
<COMMT>\*[^*/\n]* {/* capture anything after * that is not a comment close */}
<COMMT,INITIAL>\n {yylineno++; /*clear newline and update lineno */}
<COMMT>{CCOMM}    {BEGIN(INITIAL); /* start comment mode */}
<COMMT><<EOF>>    {yylineno = lastline; /* if EOF, report error at correct lineno */
                   fprintf(stderr,"Error: unclosed comment on line %d\n",yylineno);
                   yyterminate(); /* end the lexing */
                  }

"<="              {yylval.relOperator = LTE; return LTE_TOK;}
">="              {yylval.relOperator = GTE; return GTE_TOK;}
"=="              {yylval.relOperator = EQ; return EQ_TOK;}
"!="              {yylval.relOperator = NEQ; return NEQ_TOK;}

"+"               {yylval.addOperator = PLUS; return PLUS_TOK;}
"-"               {yylval.addOperator = MINUS;return MINUS_TOK;}

"*"               {yylval.mulOperator = MULT;return MULT_TOK;}
"/"               {yylval.mulOperator = DIV;return DIV_TOK;}

"="               {return EQUALS;}
"<"               {yylval.relOperator = LT; return LT_TOK;}
">"               {yylval.relOperator = GT; return GT_TOK;}

";"               {return SEMICOLON;}
","               {return COMMA;}

"("               {return LPAREN;}
")"               {return RPAREN;}
"["               {return LBRAC;}
"]"               {return RBRAC;}
"{"               {return LCURL;}
"}"               {return RCURL;}

"if"              {return IF;}
"else"            {return ELSE;}

"int"             {yylval.type = INT; return INT_TOK;}
"void"            {yylval.type = VOID; return VOID_TOK;}

"return"          {return RETURN;}

"while"           {return WHILE;}


{INT_TOK}         {yylval.integer = atoi(yytext); return NUM;}
{IDSTR}           {yylval.string = strdup(yytext); return ID;}
{WHITESPACE}      {/*clear whitespace, do nothing*/}
{ERROR}           {fprintf(stderr, "error: unrecognized token %s\n",yytext);yyerror("");}
%%
