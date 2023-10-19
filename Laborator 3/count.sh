#!/bin/bash

dir="$1"
fileout="$2"

for file in "$dir"/*; do
    if [ -f "$file" ] && [[ "$file" == *.txt ]]; then
        filename=$(basename "$file")
        wc -c < "$file" >> "$fileout"
        echo "File: $filename" >> "$fileout"
    fi
done