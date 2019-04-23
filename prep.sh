#!/bin/bash
tar -cvf parser.tar oldmakefile \
    run.sh compile.sh \
    parser.y lexer.l \
    ast.{c,h}  \
    symtable.{c,h} \
    errors.{c,h} \
    id.h \
    yacc_header.h \
    codegen.{c,h} \
    stack.{c,h}
