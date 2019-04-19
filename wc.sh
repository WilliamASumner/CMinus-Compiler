#!/bin/bash
wc -l Makefile \
    run.sh compile.sh \
    parser.y lexer.l \
    ast.{c,h}  \
    symtable.{c,h} \
    id.h \
    errors.{c,h} \
    yacc_header.h \
    codegen.{c,h} \
    stack.{c,h}


