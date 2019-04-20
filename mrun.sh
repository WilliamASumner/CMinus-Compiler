#!/bin/bash

if [ $# -eq 1 ]; then
    java -jar mars.jar $s0 $s1 0x10010000-0x10010040 $1
else
    java -jar mars.jar 0x10010000-0x10010010 output.asm
fi


