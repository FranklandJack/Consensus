for i in `seq 1 10`;
do
	for j in `seq 1 10`;
	do
		p2=$(python -c "print(($i * 0.05) + 0.5)")
		./consensus -p 1 -q $p2  -s 5000
		sleep 1
	done
done
