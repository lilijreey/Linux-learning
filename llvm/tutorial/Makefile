ff: main.cpp ast.hpp lexer.cpp parser.cpp 
	@g++ -Wall -std=c++14 -o $@ *.cpp

ir: ir.cpp
	@clang++ -g -O3 ir.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o ir
