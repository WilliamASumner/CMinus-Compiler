#!/bin/bash
if [[ -f "./cminusdebug" ]]; then
    if [ $# -eq 2 ]; then
        lldb -s .lldbstart -f ./cminusdebug $1 $2
    else
        file=$(basename $1) # get file
        lldb -s .lldbstart -f ./cminusdebug $1 ${file%.*}.asm
    fi

elif [[ -f "./cminus" ]]; then
    ./cminus $1 $2
fi
