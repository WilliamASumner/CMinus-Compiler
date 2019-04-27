#!/bin/bash
tar -cvf compiler.tar oldmakefile \
    run.sh compile.sh \
    parser.y lexer.l \
    id.h \
    yacc_header.h \
    ast.{c,h}  \
    symtable.{c,h} \
    errors.{c,h} \
    codegen.{c,h} \
    stack.{c,h}
