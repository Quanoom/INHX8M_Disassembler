#!/bin/bash



# A BASH SCRIPT : 


gcc sourceCode.c -o executable      # GENERATE THE EXECUTABLE FILE NAMED AS executable

if [ $? -eq 0 ]; then               # CHECK IF LAST COMMAND GIVES 0 (success)
    echo "executable gerenated"
    ./executable                    # RUN THE EXECUTABLE LOCATED IN THE CURRENT DIRECTORY
else
    echo "executable did not generated"

fi

exec bash                   