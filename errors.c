#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtable.h"
#include "errors.h"
#include "yacc_header.h"

void yyerror(const char* s) { // what to do on an error in parser
#ifdef DEBUG
    fprintf(stderr,"Parse error: %s on line: %d\n",s,yylineno);
#endif
    cleanup();
    exit(1);
}

void print_semantic_error(enum error_type e) {
    fprintf(stderr,"error: ");
    switch (e) {
        case USE_BEFORE_DEC:
            fprintf(stderr,"Use of variable or function before declaration");
            break;
        case LAST_DEC_NOT_MAIN:
            fprintf(stderr,"Function \"void main(void)\" is not last declaration");
            break;
        case REDECLARATION:
            fprintf(stderr,"Redeclaration of variable or function");
            break;
        case PARAMS_MISMATCH:
            fprintf(stderr,"Parameter mismatch");
            break;
        case INVALID_ARR_ACCESS:
            fprintf(stderr,"Invalid array access");
            break;
        case INCORRECT_TYPE_ASSG:
            fprintf(stderr,"Incorrect type assignment");
            break;
        case IMPROPER_RET_TYPE:
            fprintf(stderr,"Mismatched return type");
            break;
        case IMPROPER_EXPR_TYPE:
            fprintf(stderr,"Improper expression type");
            break;
        case ALL_GOOD:
            fprintf(stderr,"All is good");
            break;
        default:
            fprintf(stderr,"undefined error type encountered\n");
            return;
    }
    printf("\n");
}

void throw_semantic_error(enum error_type err) {
#ifdef DEBUG
    print_semantic_error(err); // show what error we got
#endif
    cleanup(); // clean up table and ast
    exit(1); // and exit
}


void cleanup() {
    free_ast_tree(ast); // free the ast
    free_table(table); // free the sym table
    fclose(yyin); // close the files
    fclose(yyout);
}
