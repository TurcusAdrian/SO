##!/bin/bash


dir="$1"
mod="$2"

for file in "$dir"/*; do
    if [ -f "$file" ] && [[ "$file" == *.txt ]]; then
        if [ "$mod" == "x" ]; then
            chmod +x "$file"
        elif [ "$mod" == "r" ]; then
            chmod +r "$file"
        elif [ "$mod" == "w" ]; then
            chmod +w "$file"
        fi
    elif [ -d "$file" ]; then
        bash "$0" "$file" "$mod"
    fi
done