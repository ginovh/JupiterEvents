all:
	g++ -std=c++11 main.cpp -Iaaplus/ -Laaplus/lib -laaplus -Wl,-rpath,"aaplus/lib" -pthread

run:
	./a.out
	
clean:
	rm -f a.out
