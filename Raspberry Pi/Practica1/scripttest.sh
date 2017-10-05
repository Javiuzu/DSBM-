#!/bin/bash 
A=0
while true; do
	echo $A > aux.txt
	cp aux.txt dades.txt
	let A=$A+1
	sleep 1
done
