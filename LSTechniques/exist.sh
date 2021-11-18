#!/bin/bash

###############################################################################
# Usage: ./exists file_name [arguments]
##############################################################################

# Check if the user supplied file_name
if [[ $# -eq 0 ]]; then
    echo "You must supply file_name."
    echo "Example: $0 /etc"
    exit 1 # Return with value 1
fi

# Check if the file/directory exists
if [[ -e $1 ]]; then
    # echo "File or directory exists"

    file_name_without_extension=`basename $1 .c`

    flag='-Wall -Wextra -pedantic'
    gcc ${flag} $1 -o bin/${file_name_without_extension}.out

    shift 1

    ./bin/${file_name_without_extension}.out "$@"
else
    echo "File or directory does not exist"
    exit 3 # Return with a special code so other
    # programs can use the value to see if a
    # file dosen't exist

fi

exit 0 # If the file or directory exists, we exit with 0