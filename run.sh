#!/bin/bash
if [[ -f "./yaccdebug.out" ]]; then
    lldb ./yaccdebug.out $1 $2
elif [[ -f "./yaccprog.out" ]]; then
    ./yaccprog.out $1 $2
fi
