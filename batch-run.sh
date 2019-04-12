#!/bin/bash

if [ -d $1 ]; then
    dir="$1"
    if [[ "${dir: -1}" != "/" ]]; then # add a slash if needed
        dir+="/"
    fi
    files=( $( ls $1 ) )
    files=( "${files[@]/#/$dir}" )
else 
    echo "invalid dir supplied"
    exit
fi


if [[ -f "./yaccdebug.out" ]]; then
    for file in ${files[@]}; do
        echo "testing on $file" > $2 # annotate files
        lldb ./yaccdebug.out $file $2
    done
elif [[ -f "./yaccprog.out" ]]; then
    for file in ${files[@]}; do
        echo "testing on $file" > $2 # annotate files
        ./yaccprog.out $file $2 &> $2 # put all info into file
    done
else
    echo "No file to run"
fi
