#!/bin/bash

echo "START COMPILATION"

gcc sourceCode.c -o program

if [ $? -eq 0 ]; then
    echo "COMPILATION DONE"
    ./program
else
    echo "COMPILATION FAILS"

fi

exec bash