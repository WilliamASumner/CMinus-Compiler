#ifndef COMPILER_ERRORS_H
#define COMPILER_ERRORS_H

extern struct ast_node* ast;
extern struct sym_table* table;

enum lexical_error_type {
    UNRECOGNIZED_TOKEN,
    INTEGER_OVERFLOW,
    TOO_BIG_ID
};

enum semantic_error_type {
    USE_BEFORE_DEC,
    LAST_DEC_NOT_VALID_MAIN,
    REDECLARATION_OR_BAD_SCOPE,
    PARAMS_MISMATCH,
    INVALID_ARR_ACCESS,
    TYPE_MISMATCH,
    CALL_MISMATCH,
    ID_TYPE_MISMATCH,
    NON_INT_VAR,
    NON_INT_SMP_EXPR,
    NON_INT_ADD_EXPR,
    NON_INT_TERM,
    NON_INT_FACT,
    BAD_RETURN_TYPE,
    MISSING_RETURN,
    ALL_GOOD
};

enum ast_error_type {
    EMPTY_VAR_DEC,
    EMPTY_FUNC_DEC,
    EMPTY_DECLIST,
    VAR_AND_FUNC_DEC,
    TYPE_CHECK_ON_NULL_NODE,
    INVALID_EXPR_TYPE,
    INVALID_FACTOR_TYPE,
    IMPROPER_NODE_TYPE,
    INCOMPLETE_EXPR_NODE,
    INCOMPLETE_SELE_NODE,
    INCOMPLETE_ITER_NODE
};

void yyerror(const char* s);
void throw_lexical_error(enum lexical_error_type e,void* s);
void throw_semantic_error(enum semantic_error_type e,struct ast_node* node);
void throw_ast_error(enum ast_error_type e, struct ast_node* node);
void cleanup();

#endif
