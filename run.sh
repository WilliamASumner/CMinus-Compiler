#!/bin/bash
if [[ -f "./yaccdebug.out" ]]; then
    if [ $# -eq 2 ]; then
        lldb -s .lldbstart -f ./yaccdebug.out $1 $2
    else
        file=$(basename $1) # get file
        lldb -s .lldbstart -f ./yaccdebug.out $1 ${file%.*}.asm
    fi

elif [[ -f "./yaccprog.out" ]]; then
    ./yaccprog.out $1 $2
fi
