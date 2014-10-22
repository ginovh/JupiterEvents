all:
	g++ -std=c++11 main.cpp -Iaaplus/ -Laaplus/lib -laaplus -Wl,-rpath,"aaplus/lib" -pthread

aaplus:
	rm -rf aaplus
	mkdir aaplus
	make -C aaplus -f ../Makefile inside_aaplus

inside_aaplus:
	curl http://www.naughter.com/download/aaplus.zip -o aaplus.zip
	unzip aaplus.zip
	cmake . 
	make -j4

run:
	./a.out
	
clean:
	rm -f a.out *~

.PHONY: aaplus