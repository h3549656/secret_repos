FLAG = -pedantic-errors -std=c++11
current_dir = $(shell pwd)
INC = -I ~/mylocal/include -I ~/mylocal/include/ncursestw -L ~/mylocal/lib
LINK = -lncursestw -ldl -pthread
.ONESHELL:

build:
	mkdir ~/mylocal
	git clone https://github.com/mirror/ncurses.git
	cd ncurses
	./configure --prefix ~/mylocal --enable-widec --with-pthread
	make -j
	make -j install
	
main: main.cpp
	g++ ${FLAG} ${INC} -o main main.cpp ${LINK}

clean:
	rm main 

.PHONY: clean main build
