#!/bin/bash
wc -l Makefile \
    run.sh compile.sh \
    parser.y lexer.l \
    ast.*  \
    symtable.*


