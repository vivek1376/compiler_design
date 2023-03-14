main: main.o lexicalAnalyzer.o helperFunctions.o
	g++ -std=c++14 -Wall -o main main.o lexicalAnalyzer.o helperFunctions.o 

main.o: main.cpp lexicalAnalyzer.hpp
	g++ -std=c++14 -Wall -o main.o -c main.cpp

lexicalAnalyzer.o: lexicalAnalyzer.cpp lexicalAnalyzer.hpp token.hpp symbolTable.hpp helperFunctions.hpp
	g++ -std=c++14 -o lexicalAnalyzer.o -c lexicalAnalyzer.cpp

helperFunctions.o: helperFunctions.cpp helperFunctions.hpp
	g++ -std=c++14 -o helperFunctions.o -c helperFunctions.cpp

clean:
	-rm main main.o lexicalAnalyzer.o helperFunctions.o
