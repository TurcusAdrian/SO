#!/bin/bash


while read -r linie
do
    text=$(echo "$linie" | grep -E "^[A-Z]{1,1}[A-Za-z0-9]+[ ,]*[\.]$" | grep -v "si[\ ]*," | grep -v "n[pb]")

    echo "$text"

done


# 1 grep -E "^[A-Z]{1,1}[A-Za-z0-9]+[ ,]*[\.]$"
# 2 grep -v "si[\ ]*,"
# 3 grep -v "n[pb]"