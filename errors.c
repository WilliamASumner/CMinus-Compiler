#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtable.h"
#include "errors.h"
#include "yacc_header.h"


static const char* RED = "\033[1;31m";
static const char* GRN = "\033[0;32m";
static const char* RST = "\033[0m";

void yyerror(const char* s) { // what to do on an error in parser
#ifdef DEBUG
    fprintf(stderr,"Parse error: %s on line: %d\n",s,yylineno);
#endif
    cleanup();
    exit(1);
}

void print_lexical_error(enum lexical_error_type e, char* s)
{
    fprintf(stderr,"%slexical error: ",RED);
    switch (e) {
        case INTEGER_OVERFLOW:
            fprintf(stderr,"Too large integer input");
            break;
        case UNRECOGNIZED_TOKEN:
            fprintf(stderr,"Unrecognized token %s",s);
            break;
        default:
            fprintf(stderr,"undefined ast error type encountered\n");
            return;
    }
    printf("%s\n",RST);
}

void print_semantic_error(enum semantic_error_type e,struct ast_node* node) {
    fprintf(stderr,"%ssemantic error: ",RED);
    switch (e) {
        case USE_BEFORE_DEC:
            fprintf(stderr,"Use of variable or function before declaration");
            break;
        case LAST_DEC_NOT_VALID_MAIN:
            fprintf(stderr,"Function \"void main(void)\" is not last declaration");
            break;
        case REDECLARATION_OR_BAD_SCOPE:
            fprintf(stderr,"Redeclaration of variable or function");
            break;
        case PARAMS_MISMATCH:
            fprintf(stderr,"Parameter mismatch");
            break;
        case INVALID_ARR_ACCESS:
            fprintf(stderr,"Invalid array access");
            break;
        case TYPE_MISMATCH:
            fprintf(stderr,"Mismatch of types");
            break;
        case CALL_MISMATCH:
            fprintf(stderr,"Mismatch of call to function");
            break;
        case ID_TYPE_MISMATCH:
            fprintf(stderr,"Mismatch of id types");
            break;
        case NON_INT_VAR:
            fprintf(stderr,"Non-integer type of variable");
            break;
        case NON_INT_SMP_EXPR:
            fprintf(stderr,"Non-integer type of simple-expression");
            break;
        case NON_INT_ADD_EXPR:
            fprintf(stderr,"Non-integer type of addition-expression");
            break;
        case NON_INT_TERM:
            fprintf(stderr,"Non-integer type of term");
            break;
        case NON_INT_FACT:
            fprintf(stderr,"Non-integer type of factor");
            break;
        case BAD_RETURN_TYPE:
            fprintf(stderr,"Bad return type");
            break;
        case MISSING_RETURN:
            fprintf(stderr,"Missing return in function");
            break;
        case NON_TERMINATING_VOID_RETURN:
            fprintf(stderr,"Non terminating return in function");
            break;
        default:
            fprintf(stderr,"undefined semantic error type encountered\n");
            return;
    }
    printf(" at node %p%s\n",node,RST);
}

void print_ast_error(enum ast_error_type e, struct ast_node* node)
{
    fprintf(stderr,"%sast error: ",RED);
    switch (e) {
        case EMPTY_VAR_DEC:
            fprintf(stderr,"Empty variable declaration in AST");
            break;
        case EMPTY_FUNC_DEC:
            fprintf(stderr,"Empty function declaration in AST");
            break;
        case EMPTY_DECLIST:
            fprintf(stderr,"Empty declaration list in AST");
            break;
        case VAR_AND_FUNC_DEC:
            fprintf(stderr,"Function and Variable declaration in AST");
            break;
        case TYPE_CHECK_ON_NULL_NODE:
            fprintf(stderr,"Type check performed on null node in AST");
            break;
        case INVALID_EXPR_TYPE:
            fprintf(stderr,"Invalid expr_type found in AST");
            break;
        case IMPROPER_NODE_TYPE:
            fprintf(stderr,"Improper node_type in AST");
            break;
        case INVALID_FACTOR_TYPE:
            fprintf(stderr,"Improper factor_type in AST");
            break;
        case INCOMPLETE_EXPR_NODE:
            fprintf(stderr,"Incomplete expression node found in AST");
            break;
        case INCOMPLETE_SELE_NODE:
            fprintf(stderr,"Incomplete selection node found in AST");
            break;
        case INCOMPLETE_ITER_NODE:
            fprintf(stderr,"Incomplete iteration node found in AST");
            break;
        default:
            fprintf(stderr,"undefined ast error type encountered\n");
            return;
    }
    printf(" at node %p %s\n",node,RST);
}

void throw_lexical_error(enum lexical_error_type err,char* s) {
#ifdef DEBUG
    print_lexical_error(err,s); // show what error we got
#endif
    cleanup(); // clean up table and ast if need be
    exit(1); // and exit
}

void throw_semantic_error(enum semantic_error_type err,struct ast_node* node) {
#ifdef DEBUG
    print_semantic_error(err,node); // show what error we got
#endif
    cleanup(); // clean up table and ast
    exit(1); // and exit
}

void throw_ast_error(enum ast_error_type err, struct ast_node* node) {
#ifdef DEBUG
    print_ast_error(err,node); // show what error we got
#endif
    cleanup(); // clean up table and ast
    exit(1); // and exit
}

void cleanup() {
#ifdef DEBUG
    fprintf(stderr,"%sRunning cleanup...%s\n",GRN,RST);
    free_ast_tree(ast); // free the ast
    fprintf(stderr,"%sTree successfully freed%s\n",GRN,RST);
    free_table(table); // free the sym table
    fprintf(stderr,"%sSymbol table successfully freed%s\n",GRN,RST);
    fclose(yyin); // close the files
    fprintf(stderr,"%syyin successfully closed%s\n",GRN,RST);
    fclose(yyout);
    fprintf(stderr,"%syyout successfully closed%s\n",GRN,RST);
#else
    free_ast_tree(ast); // free the ast
    free_table(table); // free the sym table
    fclose(yyin); // close the files
    fclose(yyout);
#endif 

}
