#!/bin/bash

# usage: ./runlocaltests.sh [nome del codice sorgente] [numero primo test locale da provare] [numero ultimo test locale da provare]

gcc -Wall -Werror -fsanitize=address -std=gnu11 -o2 -pipe -g3 -o $1 $1.c -lm # argomento 1: nome codice sorgente (senza .c), deve trovarsi nella stessa cartella di questo script

if [ $? != 0 ]
then
	echo "Errore di compilazione!\n"
	return 1	
fi

for ((VARIABLE = $2; VARIABLE <= $3; VARIABLE++)) # intervallo dei test da provare (argomenti 2 e 3, estremi inclusi)
	do
		./$1 < archivio_test_aperti/open_$VARIABLE.txt > output.txt # attenzione ai percorsi dei file! la cartella dei test locali deve trovarsi assieme a questo file
		diff archivio_test_aperti/open_$VARIABLE.output.txt output.txt 
		
		if [ $? != 0 ]
		then
			echo "Differenza in file $VARIABLE!\n"
			break
		fi
		
	done