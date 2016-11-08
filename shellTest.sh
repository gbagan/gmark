for((i=0;i<=500000; i+=500000/10))
do
  ./src/test -c use-cases/test.xml -n ${i}
done
