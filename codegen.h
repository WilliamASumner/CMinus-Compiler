// Code Generation
// Written by Will Sumner
// CS 1622: Intro to Compilers
// University of Pittsburgh, Spring 2019
// This file contains code for generating MIPS code

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
