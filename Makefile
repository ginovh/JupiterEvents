all:
	g++ -std=c++11 main.cpp -Iaaplus/ -Laaplus/lib -laaplus

run:
	./a.out
	
clean:
	rm -f a.out