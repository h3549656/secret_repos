FLAG = -pedantic-errors -std=c++11 
current_dir = ${shell pwd}
INC = -I ${current_dir}/local/include -I ${current_dir}/local/include/ncursest -L ${current_dir}/local/lib
LINK = -lncursest -ldl -pthread

main: main.cpp 
	g++ ${FLAG} ${INC} -o main main.cpp ${LINK}
	
clean:
	rm main

.PHONY: clean main
