#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <caracter>"
    exit 1
fi

caracter=$1
count=0

while IFS= read -r line; do
    if echo "$line" | grep -E "^[A-Z]{1}[A-Za-z0-9]+[ ,]*[\.]$" | grep -v "si[\ ]*" | grep -v "n[pb]" | grep -q "$caracter"; then
        (( count++ ))
    fi
done

echo "$count"