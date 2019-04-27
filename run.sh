#!/bin/bash
if [[ -f "./cminusdebug" ]]; then
    if [ $# -eq 2 ]; then
        lldb -s .lldbstart -f ./cminusdebug $1 $2
    elif [ $# -eq 1 ]; then
        file=$(basename $1) # get file
        lldb -s .lldbstart -f ./cminusdebug $1 ${file%.*}.asm
    else
        echo "invalid number of args"
    fi

elif [[ -f "./cminus" ]]; then
    if [ $# -eq 2 ]; then
        ./cminus $1 $2
    elif [ $# -eq 1 ]; then
        ./cminus $1
    else
        echo "invalid number of args"
    fi
else
    echo "no compiler binary found"
fi
