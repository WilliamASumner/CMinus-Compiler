#ifndef CODEGEN_H
#define CODEGEN_H
#include "ast.h"

enum if_type {
    INT_IF,
    GT_IF,
    LT_IF,
    LTE_IF,
    GTE_IF,
    EQ_IF,
    NEQ_IF
};

void gencode(struct ast_node* node);
#endif
