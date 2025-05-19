#!/bin/bash

gcc sourceCode.c -o code

if [ $? -eq 0 ]; then
    echo "COMPILATION DONE"
    ./code
else
    echo "COMPILATION FAILS"
fi

if [ -e "code" ]; then  
    rm code
else
    echo "file does not exist"
fi

exec bash