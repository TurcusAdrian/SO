#!/bin/bash

echo "$1 $2"

dir=$1

mod=$2


for file in "$dir"/*.txt; do

if [ "$mod" == "x" ]
then chmod +$mod "$file"
fi

if [ "$mod" == "r" ]
then chmod +$mod "$file"
fi

if [ "$mod" == "w" ]
then chmod +$mod "$file"
fi
done


for director in "$dir"/*; do
if [ -d "$dir" ]
then bash $0 $dir $mod
fi
done