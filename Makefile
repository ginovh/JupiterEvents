all:
	g++ main.cpp -Iaaplus/ -Laaplus/lib -laaplus -Wl,-rpath,"aaplus/lib" -pthread -o jupevent

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
	./jupevent 2014 2014
	
clean:
	rm -f jupevent *~

.PHONY: aaplus
