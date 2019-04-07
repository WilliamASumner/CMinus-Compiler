#ifndef COMPILER_ERRORS_H
#define COMPILER_ERRORS_H

extern struct ast_node* ast;
extern struct sym_table* table;

enum error_type {
    USE_BEFORE_DEC,
    LAST_DEC_NOT_MAIN,
    REDECLARATION,
    PARAMS_MISMATCH,
    INVALID_ARR_ACCESS,
    INCORRECT_TYPE_ASSG,
    IMPROPER_RET_TYPE,
    IMPROPER_EXPR_TYPE,
    ALL_GOOD
};

void yyerror(const char* s);
void print_parser_error(enum error_type e);
void cleanup();

#endif
