#!/bin/bash
tar -cvf parser.tar Makefile \
    run.sh compile.sh \
    parser.y lexer.l \
    ast.*  \
    symtable.* \
    errors.* \
    id.h \
    yacc_header.h
