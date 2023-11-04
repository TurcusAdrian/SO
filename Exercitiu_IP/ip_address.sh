#!/bin/bash

while read -r linie
do
    text=$(echo "$linie" | grep -E "^(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]).){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$")

    echo "$text"

done

#merge daca vreau sa folosesc formatul cu . dar nu se opreste pana la 255

# 34.89.234.1 - exemplu adresa IP - [0-255].[0-255].[0-255].[0-255]
#maxim 255.255.255.255
#minim 0.0.0.0