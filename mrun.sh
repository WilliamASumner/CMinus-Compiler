#!/bin/bash
#$sp, $ra print values of these bad boois
#$1 - filename
if [ $# -eq 1 ]; then
    java -jar mars.jar $1
else
    java -jar mars.jar 0x10010000-0x10010010 output.asm
fi


