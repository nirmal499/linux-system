#!/bin/bash

#######################################################################################
# Usage: ./exists file_name [arguments_comma_separated] [dependencies_comma_separated]  
#######################################################################################

# Check if the user supplied file_name
if [[ $# -eq 0 || $# -gt 4 ]]; then
    echo "You must supply file_name."
    echo "Example: $0 /etc.c arg1,arg2 lm,lprime"
    echo "Example: $0 /etc.c "" lm,lprime"
    exit 1 # Return with value 1
fi

deps_str=""
args_str=""
# The special shell variable IFS determines how Bash recognizes word boundaries while splitting a sequence of character strings.
# The default value of IFS is a three-character string comprising a space, tab, and newline
IFS=','
for arg in $2; do args_str+="$arg "; done
for dep in $3; do deps_str+="$dep "; done
unset IFS

# echo "Dep is ${deps_str}"
# echo "Args is ${args_str}"

# Check if the file/directory exists
if [[ -e $1 ]]; then
    # echo "File or directory exists"

    file_name_without_extension=`basename $1 .c`

    flag='-Wall -Wextra -pedantic'
    # str1="gcc ${flag} $1 -o bin/${file_name_without_extension}.out ${deps_str}"
    gcc ${flag} $1 -o bin/${file_name_without_extension}.out ${deps_str}

    # shift 1
    # ./bin/${file_name_without_extension}.out "$@"

    # str2="./bin/${file_name_without_extension}.out ${args_str}"
    ./bin/${file_name_without_extension}.out ${args_str}

    # echo "${str1}"
    # echo "${str2}"

else
    echo "File or directory does not exist"
    exit 3 # Return with a special code so other
    # programs can use the value to see if a
    # file dosen't exist

fi