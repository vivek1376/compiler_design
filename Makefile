lexer: lexer.o
	g++ -Wall -o lexer lexer.o


lexer.o: lexical_analyzer.cpp lexical_analyzer.hpp
	g++ -Wall -o lexer.o -c lexical_analyzer.cpp


clean:
	-rm lexer lexer.o
