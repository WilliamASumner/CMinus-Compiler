%{
#include <stdio.h> // output funcs
#include <stdlib.h>
#include <string.h> // convenience funcs
#include "ast.h" // ast nodes
#include "symtable.h" // for finding funcs/vars
#include "parser.tab.h"
#define YYDEBUG 1 // setting for debug

extern int yylex(void);
extern int yyparse(void);
extern int yylineno;
extern FILE* yyin;
extern FILE* yyout;

void yyerror(const char* s);
int yywrap();

struct program_node* ast = NULL; // main pointer to the AST
struct sym_table* table = NULL; // setup the symbol table

struct dec_list_node* decListStart = NULL;
struct params_node* paramListStart = NULL;


//TODO look into %parse-param

%}

%union { // yyval can be a string or a integer, depending
    int integer;
    char* string;
    struct ast_node* ast;
    struct program_node* program;
    struct dec_list_node* decList;
    struct var_dec_node* varDec;
    struct func_dec_node* funcDec;
    struct params_node* params;
    struct cmp_stmt_node* cmpStmt;
    struct local_decs_node* localDecs;
    struct stmt_node* stmt;
    struct expr_stmt_node* exprStmt;
    struct sel_stmt_node* selStmt;
    struct iter_stmt_node* iterStmt;
    struct ret_stmt_node* retStmt;
    struct expr_node* expr;
    struct var_node* var;
    struct smp_expr_node* smpExpr;
    struct add_expr_node* addExpr;
    struct term_node* term;
    struct factor_node* fact;
    struct call_node* call;
    struct args_node* args;
    enum mulop mulOperator;
    enum addop addOperator;
    enum relop relOperator;
    enum type_spec type;
}

%token <integer> NUM
%token <string> ID
%token LPAREN RPAREN LCURL RCURL LBRAC RBRAC
%token <addOperator> PLUS_TOK MINUS_TOK
%token <mulOperator> MULT_TOK DIV_TOK
%token SEMICOLON COMMA EQUALS
%token <relOperator> GT_TOK LT_TOK LTE_TOK GTE_TOK EQ_TOK NEQ_TOK
%token <type> TYPE
%token IF ELSE WHILE RETURN 
%token <type> VOID_TOK INT_TOK

%type <decList> decList declaration
%type <varDec> varDeclaration
%type <type> typeSpec
%type <funcDec> funcDeclaration
%type <params> params paramList param
%type <cmpStmt> compStatement
%type <localDecs> localDecs
%type <stmt> statementList statement
%type <exprStmt> exprStatement
%type <selStmt> seleStatement
%type <iterStmt> iterStatement
%type <retStmt> retStatement
%type <expr> expr
%type <var> var
%type <smpExpr> sexpr
%type <relOperator> relop
%type <addExpr> addexpr
%type <addOperator> addop
%type <term> term
%type <mulOperator> mulop
%type <fact> fact
%type <call> call
%type <args> args argList

%left PLUS_TOK MINUS_TOK
%left MULT_TOK DIV_TOK

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

program:                         {;ast = ast_new_program_node(NULL); YYACCEPT; /*no program*/}
       | decList                 {ast = ast_new_program_node(decListStart); YYACCEPT;}
;

decList: decList declaration     {$$ = ast_link_dec_list_node($1,$2);}
       | declaration             {$$ = $1; decListStart = $1; }
;

declaration: varDeclaration      {$$ = ast_new_dec_list_node($1,NULL,NULL);}
           | funcDeclaration     {$$ = ast_new_dec_list_node(NULL,$1,NULL);}
;

varDeclaration: typeSpec ID  SEMICOLON   {$$ = ast_new_var_dec_node($1,table_add(table,$2,VARIABLE),0);}
              | typeSpec ID LBRAC NUM RBRAC SEMICOLON {$$ = ast_new_var_dec_node($1,table_add(table,$2,ARRAY),$4);}
;

typeSpec: INT_TOK          {$$ = $1;}
        | VOID_TOK         {$$ = $1;}
;

funcDeclaration: typeSpec ID LPAREN params RPAREN compStatement { $$ = ast_new_func_dec_node($1,table_add(table,$2,FUNCTION),$4,$6);}
;

params: paramList  {$$ = paramListStart;}
      | VOID_TOK   {$$ = NULL;}
;

paramList: paramList COMMA param { $$ = ast_link_params_node($1,$3);}
         | param { $$ = $1; paramListStart = $1;}
;

param: typeSpec ID {$$ = ast_new_params_node($1,table_add(table,$2,VARIABLE),NULL);}
     | typeSpec ID LBRAC RBRAC { $$ = ast_new_params_node($1,table_add(table,$2,VARIABLE),NULL); }
;

compStatement: LCURL localDecs statementList RCURL {$$ = ast_new_cmp_stmt_node($2,$3);}
;

localDecs:                  {$$ = NULL; /* TODO inspect this */}
         | localDecs varDeclaration {$$ = ast_new_local_decs_node($2,$1);}
;

statementList:  {$$ = NULL; /* TODO inspect this as well, look out for proper linking */}
             | statementList statement {$$ = ast_link_stmt_node($2,$1);}
;

statement: exprStatement { $$ = ast_new_stmt_node(EXPRST,NULL,(union sub_stmt)$1);}
         | compStatement { $$ = ast_new_stmt_node(CMP,NULL,(union sub_stmt)$1);}
         | seleStatement { $$ = ast_new_stmt_node(SEL,NULL,(union sub_stmt)$1);}
         | iterStatement { $$ = ast_new_stmt_node(ITER,NULL,(union sub_stmt)$1);}
         | retStatement  { $$ = ast_new_stmt_node(RET,NULL,(union sub_stmt)$1);}
;

exprStatement: expr SEMICOLON { $$ = ast_new_expr_stmt_node($1);}
             | SEMICOLON { $$ = ast_new_expr_stmt_node(NULL);}
;

seleStatement: IF LPAREN expr RPAREN statement %prec LOWER_THAN_ELSE { $$ = ast_new_sel_stmt_node($3,$5,NULL);}
             | IF LPAREN expr RPAREN statement ELSE statement { $$ = ast_new_sel_stmt_node($3,$5,$7);}
;

iterStatement: WHILE LPAREN expr RPAREN statement { $$ = ast_new_iter_stmt_node($3,$5); }

;

retStatement: RETURN SEMICOLON      { $$ = ast_new_ret_stmt_node(NULL);}
            | RETURN expr SEMICOLON { $$ = ast_new_ret_stmt_node($2);}
;

expr: var EQUALS expr { $$ = ast_new_expr_node($1,$3,NULL);}
    | sexpr           { $$ = ast_new_expr_node(NULL,NULL,$1);}
;

var: ID                  { $$ = ast_new_var_node(table_add(table,$1,VARIABLE),NULL); }
   | ID LBRAC expr RBRAC { $$ = ast_new_var_node(table_add(table,$1,ARRAY),$3);}
;

sexpr: addexpr relop addexpr { $$ = ast_new_smp_expr_node($2,$1,$3);}
     | addexpr               {$$ = ast_new_smp_expr_node(0,$1,NULL);}
;

relop: LT_TOK {$$ = $1;}
     | GT_TOK {$$ = $1;}
     | GTE_TOK {$$ = $1;}
     | LTE_TOK {$$ = $1;}
     | EQ_TOK {$$ = $1;}
     | NEQ_TOK {$$ = $1;}
;

addexpr: addexpr addop term     {$$ = ast_new_add_expr_node($2,$1,$3);}
       | term                   {$$ = ast_new_add_expr_node(0,NULL,$1);}
;

addop: PLUS_TOK                 {$$ = $1;}
     | MINUS_TOK                {$$ = $1;}
;

term: term mulop fact           {$$ = ast_new_term_node($2,$1,$3); /*TODO Linking*/ }
    | fact                      {$$ = ast_new_term_node(0,NULL,$1); /*TODO linking*/ }
;

mulop: MULT_TOK     {$$ = $1;}
     | DIV_TOK      {$$ = $1;}
;

fact: LPAREN expr RPAREN {$$ = ast_new_factor_node(EXPR,(union fact_union)$2);}
    | var  {$$ = ast_new_factor_node(VAR,(union fact_union)$1);}
    | call {$$ = ast_new_factor_node(CALL,(union fact_union)$1);}
    | NUM {$$ = ast_new_factor_node(NUMFACT,(union fact_union)$1);}
;

call: ID LPAREN args RPAREN { $$ = ast_new_call_node(table_add(table,$1,FUNCTION),$3);}
;

args:         { $$ = NULL;}
    | argList { $$ = $1; }
;

argList: argList COMMA expr {$$ = ast_new_args_node($3,$1);}
       | expr {$$ = ast_new_args_node($1,NULL);}
;

%%

int yywrap() { return 1; } // compiles only one file at a time, reports that processing is done

void yyerror(const char* s) { // what to do on an error
    fprintf(stderr,"Parse error: %s on line: %d\n",s,yylineno);
    exit(1);
}

int main(int argc,char **argv)
{
    if (argc == 3) { // using ./lexerProg infile outfile
        yyin = fopen(argv[1],"r");
        yyout = fopen(argv[2],"w");
    }
    else if (argc ==  2) { // using ./lexerProg infile
        yyin = fopen(argv[1],"r");
        yyout = fopen("lex-out.lex","w");
    }
    else if (argc == 1) { // using ./lexerProg
        yyin = stdin;
        yyout = stdout;
    }
    else { // print usage
        printf("usage: ./parser [infile] [outfile]\n");
        return 0;
    }

    if (yyin == NULL) {
        printf("Error: file %s could not be found.\n",argv[1]);
        return -1;
    }

    table = newTable(); // initialize the table
    //yydebug = 1;

    yyparse(); // run that parser baby
    printf("Now printing tree\n");
    print_ast_tree((struct ast_node*)ast);
    printf("tree printed!\n");
    free_table(table); // cleanup
    //free_ast_tree((struct ast_node*)ast);



    if (yyout != NULL)
        fclose(yyout); // close files
    if (yyin != NULL)
        fclose(yyin);
}
