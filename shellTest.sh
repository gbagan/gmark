for((i=0;i<=1000000; i+=1000000/10))
do
	./src/test -c use-cases/test.xml -n ${i}
done
