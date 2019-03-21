%{
extern int yylval;
int lastline = 1; // val for returning comment error
%}

%x COMMT

DIG [0-9]
CHAR [a-z|A-Z]
NUM {DIG}{DIG}*
ID {CHAR}({DIG}|{CHAR})*
KEY if|else|int|return|void|while
DOUBLESYM [<|>|=|!]=
OCOMM \/\*
CCOMM \*\/
SYM [+|\-|*|/|=|<|>|;|,|\(|\)|\[|\]|\{|\}]
WHITESPACE [ \t]+
/*ERRORID {NUM}{ID} */
ERROR .

%%
{OCOMM}           {BEGIN(COMMT); lastline = yylineno;}
<COMMT>[^*\n]*    {/* capture anything that is not a newline */}
<COMMT>\*[^*/\n]* {/* capture anything after * that is not a comment close */}
<COMMT>\n         {yylineno++; /* eat up newlines */}
<COMMT>{CCOMM}    {BEGIN(INITIAL); /* start comment mode */}
<COMMT><<EOF>>    {yylineno = lastline; /* if comment and EOF, report error at correct lineno */
                   fprintf(yyout,"(%d,ERROR,\"/*\")\n",yylineno);
                   yyterminate(); /* end the lexing */
                  }
{KEY}             {fprintf(yyout,"(%d,KEY,\"%s\")\n",yylineno,yytext);}
{NUM}             {fprintf(yyout,"(%d,NUM,\"%s\")\n",yylineno,yytext);}
{ID}              {fprintf(yyout,"(%d,ID,\"%s\")\n",yylineno,yytext);}
{DOUBLESYM}       {fprintf(yyout,"(%d,SYM,\"%s\")\n",yylineno,yytext);}
{SYM}             {fprintf(yyout,"(%d,SYM,\"%s\")\n",yylineno,yytext);}
{WHITESPACE}      {/*clear whitespace */}
<COMMT,INITIAL>\n {yylineno++; /*clear newline and update lineno */}
{ERROR}           {fprintf(yyout,"(%d,ERROR,\"%s\")\n",yylineno,yytext);
                   return -1;}
%%

int yywrap() { return 1; } // compiles only one file at a time, reports that processing is done

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
    else { // print usage
        yyout = stdout;
        printf("usage: ./lexerProg [infile] [outfile]\n");
        return 0;
    }

    if (yyin == NULL) {
        printf("Error: file %s could not be found.\n",argv[1]);
        return -1;
    }

    yylex(); // run that lexer!

    fclose(yyout); // close files
    fclose(yyin);
}
