%{
extern int yylval;
int lastline = 1;
%}

%x COMMT

DIG [0-9]
CHAR [a-z|A-Z]
ID {CHAR}({DIG}|{CHAR})*
NUM {DIG}{DIG}*
KEY if|else|int|return|void|while
DOUBLESYM [<|>|=|!]=
OCOMM \/\*
CCOMM \*\/
SYM [+|\-|*|/|=|<|>|;|,|\(|\)|\[|\]|\{|\}]
WHITESPACE [ \t]+
ERRORID {NUM}{ID}
ERROR .

%%
{OCOMM}           {BEGIN(COMMT); lastline = yylineno;}
<COMMT>[^*\n]*    {/* capture anything that is not a newline */}
<COMMT>\*[^*/\n]* {/* capture anything after * that is not a comment close */}
<COMMT>\n         {yylineno++;}
<COMMT>{CCOMM}    {BEGIN(INITIAL);}
<COMMT><<EOF>>    {yylineno = lastline;
                   fprintf(yyout,"(%d,ERROR,\"/*\")\n",yylineno);
                   yyterminate();
                  }
{KEY}             {fprintf(yyout,"(%d,KEY,\"%s\")\n",yylineno,yytext);}
{NUM}             {fprintf(yyout,"(%d,NUM,\"%s\")\n",yylineno,yytext);}
{ID}              {fprintf(yyout,"(%d,ID,\"%s\")\n",yylineno,yytext);}
{DOUBLESYM}       {fprintf(yyout,"(%d,SYM,\"%s\")\n",yylineno,yytext);}
{SYM}             {fprintf(yyout,"(%d,SYM,\"%s\")\n",yylineno,yytext);}
{WHITESPACE}      {} /*clear whitespace */
<COMMT,INITIAL>\n {yylineno++;} /*clear whitespace */
{ERRORID}         {fprintf(yyout,"(%d,ERROR,\"%s\")\n",yylineno,yytext);
                   return -1;}
{ERROR}           {fprintf(yyout,"(%d,ERROR,\"%s\")\n",yylineno,yytext);
                   return -1;}

%%

/* no yywrap so link with -ll */

int main(int argc,char **argv)
{
    if (argc == 3) { // using ./lexerProg infile outfile
        yyin = fopen(argv[1],"r");
        yyset_out(fopen(argv[2],"w"));
    }
    else if (argc ==  2) { // using ./lexerProg infile
        yyin = fopen(argv[1],"r");
        yyset_out(fopen("lex-out.lex","w"));
    }
    else if (argc == 1) { // using ./lexerProg
        yyin = stdin;
        yyset_out(stdout);
    }
    else {
        yyout = stdout;
        printf("usage: ./lexerProg [infile] [outfile]\n");
        return 0;
    }

    if (yyin == NULL) {
        printf("Error: file %s could not be found.\n",argv[1]);
        return -1;
    }

    yylex(); // run that lexer!

    fclose(yyout);
    fclose(yyin);
}
