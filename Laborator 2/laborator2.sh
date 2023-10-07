#!/bin/bash

echo "$1 $2 $3"

file="$1"

dir="$2"

nr_argumente=$#

if [ $nr_argumente -lt 3 ]
    then echo "Usage : $0 file dir n1 n2 n3 n4 n5 . . ."
fi

count=0;
sum=0;

shift 2

for arg in "$@"
do 
    if [ $arg -gt 10 ]
        then count=`expr $count + 1`
    fi
done

for arg in "$@"
do
    sum=`expr $sum + $arg`
done

echo "Nr de numere mai mari ca 10 este: $count si suma tuturor numerelor este: $sum"

echo "Numarul de cifre al sumei este:" 
echo $sum | wc -L  


if [ -f "$file" ]
    then echo "$sum" > "$file"
         echo "$count" >> "$file" 
fi


for file in "$dir"/*.txt; do
    if [ -f "$file" ]; then
        echo "Continutul fisierului '$file':"
        cat "$file"
        echo "-------------------"
    fi
done