while true; do
	sudo python p1.py > aux.txt
	cp aux.txt dades.txt
	echo "It"
	sleep 1
done
