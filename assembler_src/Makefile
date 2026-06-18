DEBUG=#-DDEBUG
GDB=#-g
STD=-std=c++17
all:
#	bison -d parser.yy -o parser.tab.cpp
#	flex -o lexer.cpp lexer.l
	g++ $(DEBUG) $(GDB) $(STD) lexer.cpp -c -o lexer.o
	g++ $(DEBUG) $(GDB) $(STD) parser.tab.cpp -c -o parser.o
# 	g++ $(DEBUG) $(GDB) $(STD) lexer_test.cpp lexer.o -o lexer_test
	g++ $(DEBUG) $(GDB) $(STD) ast.cpp -c -o ast.o
	g++ $(DEBUG) $(GDB) $(STD) instructions.cpp -c -o instructions.o
	g++ $(DEBUG) $(GDB) $(STD) assembler.cpp -c -o assembler.o
	g++ $(DEBUG) $(GDB) $(STD) main.cpp lexer.o parser.o assembler.o ast.o instructions.o -o assembler

