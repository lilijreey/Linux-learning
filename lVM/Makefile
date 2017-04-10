all: vm ast

ast: src/ast.cpp
	@clang++-3.6  -Wall -g -std=c++11 -o $@ $^

vm: src/vm.cpp src/main.cpp src/ins.cpp
	@clang++-3.6  -Wall -g -std=c++11 -o $@ $^

clean:
	@rm -rf vm ast *.o
