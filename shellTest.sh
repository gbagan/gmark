for((i=10000;i<5000000; i+=5000000/10))
do
	./src/test -c use-cases/test.xml -n ${i}/3-2*${i}/3-${i}
done
