#!/bin/bash

while true; do
	sudo ./ad_spi_acc > aux.txt
	cp aux.txt acc.txt
	#echo "It"
	sleep 1
done
