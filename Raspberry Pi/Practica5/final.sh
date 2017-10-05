#!/bin/bash

while true; do
	./final > aux.txt
	cp aux.txt final.txt
	#echo "It"
	sleep 1
done
