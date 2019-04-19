%{
// Written by Will Sumner
// CS 1622: Intro to Compilers
// University of Pittsburgh, Spring 2019
// C- minus yacc grammar with AST generation

#include <stdio.h> // output funcs
#include <stdlib.h>
#include <string.h> // convenience funcs
#include "yacc_header.h" // header for yyparse, yylineno...
#include "ast.h" // ast nodes
#include "symtable.h" // for finding funcs/vars
#include "parser.tab.h" // symbols/defines for tokens
#include "errors.h" // error functions/definitions
#define YYDEBUG 1 // setting for debug


struct ast_node* ast = NULL; // main pointer to the AST
struct sym_table* table = NULL; // setup the symbol table

struct dec_list_node* decListStart = NULL;
struct params_node* paramListStart = NULL;
struct args_node* argsListStart = NULL;

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
%type <type> typeSpec
%type <varDec> varDeclaration
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

program:                        {ast = NULL; /*no program*/}
       | decList                {ast = (struct ast_node*)ast_new_program_node(decListStart);}
;

decList: decList declaration    {$$ = ast_link_dec_list_node($1,$2);}
       | declaration            {$$ = $1; decListStart = $1; }
;

declaration: varDeclaration     {$$ = ast_new_dec_list_node($1,NULL,NULL);}
           | funcDeclaration    {$$ = ast_new_dec_list_node(NULL,$1,NULL);}
;

varDeclaration: typeSpec ID  SEMICOLON  {$$ =
              ast_new_var_dec_node(VARIABLE,$1,$2,-1);}
              | typeSpec ID LBRAC NUM RBRAC SEMICOLON {$$ =
              ast_new_var_dec_node(ARRAY,$1,$2,$4);}
;

typeSpec: INT_TOK               {$$ = $1;}
        | VOID_TOK              {$$ = $1;}
;

funcDeclaration: typeSpec ID LPAREN params RPAREN compStatement {$$ =
               ast_new_func_dec_node(FUNCTION,$1,$2,$4,$6);}
;

params: paramList  {$$ = paramListStart;}
      | VOID_TOK   {$$ = NULL;}
;

paramList: paramList COMMA param {$$ = ast_link_params_node($1,$3);}
         | param                 {$$ = $1; paramListStart = $1;}
;

param: typeSpec ID               {$$ = ast_new_params_node(VARIABLE,$1,$2,NULL);}
     | typeSpec ID LBRAC RBRAC   {$$ = ast_new_params_node(ARRAY,$1,$2,NULL);}
;

compStatement: LCURL localDecs statementList RCURL {$$ = ast_new_cmp_stmt_node(reverse_local_dec_list($2),reverse_stmt_list($3));}
;

localDecs:                           {$$ = NULL; /* TODO inspect this */}
         | localDecs  varDeclaration {$$ = ($1 == NULL) ? ast_new_local_decs_node($2,NULL) : ast_new_local_decs_node($2,$1);}
;

statementList:  {$$ = NULL; /* no more statements*/}
             | statementList statement {$$ = ($1 == NULL) ? $2 : ast_link_stmt_node($2,$1);}
;

statement: exprStatement {$$ = ast_new_stmt_node(EXPRST,NULL,(union sub_stmt)$1);}
         | compStatement {$$ = ast_new_stmt_node(CMP,NULL,(union sub_stmt)$1);}
         | seleStatement {$$ = ast_new_stmt_node(SEL,NULL,(union sub_stmt)$1);}
         | iterStatement {$$ = ast_new_stmt_node(ITER,NULL,(union sub_stmt)$1);}
         | retStatement  {$$ = ast_new_stmt_node(RET,NULL,(union sub_stmt)$1);}
;

exprStatement: expr SEMICOLON {$$ = ast_new_expr_stmt_node($1);}
             | SEMICOLON {$$ = ast_new_expr_stmt_node(NULL);}
;

seleStatement: IF LPAREN expr RPAREN statement %prec LOWER_THAN_ELSE {$$ = ast_new_sel_stmt_node($3,$5,NULL);}
             | IF LPAREN expr RPAREN statement ELSE statement {$$ = ast_new_sel_stmt_node($3,$5,$7);}
;

iterStatement: WHILE LPAREN expr RPAREN statement {$$ = ast_new_iter_stmt_node($3,$5); }

;

retStatement: RETURN SEMICOLON      {$$ = ast_new_ret_stmt_node(NULL);}
            | RETURN expr SEMICOLON {$$ = ast_new_ret_stmt_node($2);}
;

expr: var EQUALS expr        {$$ = ast_new_expr_node($1,$3,NULL);}
    | sexpr                  {$$ = ast_new_expr_node(NULL,NULL,$1);}
;

var: ID                      {$$ = ast_new_var_node(VARIABLE,INT,$1,NULL); }
   | ID LBRAC expr RBRAC     {$$ = ast_new_var_node(ARRAY,INT,$1,$3);}
;

sexpr: addexpr relop addexpr {$$ = ast_new_smp_expr_node($2,$1,$3);}
     | addexpr               {$$ = ast_new_smp_expr_node(UNARYREL,$1,NULL);}
;

relop: LT_TOK               {$$ = $1;}
     | GT_TOK               {$$ = $1;}
     | GTE_TOK              {$$ = $1;}
     | LTE_TOK              {$$ = $1;}
     | EQ_TOK               {$$ = $1;}
     | NEQ_TOK              {$$ = $1;}
;

addexpr: addexpr addop term {$$ = ast_new_add_expr_node($2,$1,$3);}
       | term               {$$ = ast_new_add_expr_node(UNARYADD,NULL,$1);}
;

addop: PLUS_TOK             {$$ = $1;}
     | MINUS_TOK            {$$ = $1;}
;

term: term mulop fact       {$$ = ast_new_term_node($2,$1,$3); /*TODO Linking*/ }
    | fact                  {$$ = ast_new_term_node(UNARYMULT,NULL,$1); /*TODO linking*/ }
;

mulop: MULT_TOK             {$$ = $1;}
     | DIV_TOK              {$$ = $1;}
;

fact: LPAREN expr RPAREN    {$$ = ast_new_factor_node(EXPR,(union fact_union)$2);}
    | var                   {$$ = ast_new_factor_node(VAR,(union fact_union)$1);}
    | call                  {$$ = ast_new_factor_node(CALL,(union fact_union)$1);}
    | NUM                   {$$ = ast_new_factor_node(NUMFACT,(union fact_union)$1);}
;

call: ID LPAREN args RPAREN {$$ = ast_new_call_node($1,$3);}
;

args:                       {$$ = NULL;}
    | argList               {$$ = argsListStart; }
;

argList: argList COMMA expr {$$ = ast_link_args_node($1,ast_new_args_node($3,NULL));}
       | expr               {argsListStart = ast_new_args_node($1,NULL);
                             $$ = argsListStart;}
;

%%

int yywrap() { return 1; } // compiles only one file at a time, reports that processing is done

int main(int argc,char **argv)
{
    if (argc == 3) { // using ./lexerProg infile outfile
        yyin = fopen(argv[1],"r");
        yyout = fopen(argv[2],"w");
    }
    else if (argc ==  2) { // using ./lexerProg infile
        yyin = fopen(argv[1],"r");
        yyout = stdout;
    }
    else if (argc == 1) { // using ./lexerProg
        yyin = stdin;
        yyout = stdout;
    }
    else { // print usage
        fprintf(stderr,"usage: ./parser [infile] [outfile]\n");
        return 0;
    }

    if (yyin == NULL) {
        fprintf(stderr,"Error: file %s could not be found.\n",argv[1]);
        return -1;
    }

    /*#ifdef DEBUG
    yydebug = 1; // set debug flag
    #endif */

    table = new_table(); // initialize the table
    yyparse(); // run that parser, baby.
    // add in the input and output functions, throws errors on redefinition
    ast_add_io(ast);
    analyze_ast_tree(ast);

    #ifdef DEBUG
    fprintf(yyout,"\n\n----- PRINTING AST -----\n\n");
    print_ast_tree((struct ast_node*)ast,stdout);
    fprintf(yyout,"\n\n----- FINISH PRINTING AST -----\n\n");
    #endif

    free_table(table); // clear the table
    table = new_table(); // reinitialize the table

    gencode_ast_tree(ast); // generate the MIPS!


    // cleanup
    free_table(table);
    free_ast_tree((struct ast_node*)ast);

    if (yyout != NULL)
        fclose(yyout); // close files
    if (yyin != NULL)
        fclose(yyin);
}
