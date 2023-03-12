main: main.o lexicalAnalyzer.o helperFunctions.o
	g++ -Wall -o main main.o lexicalAnalyzer.o

main.o: main.cpp lexicalAnalyzer.hpp
	g++ -Wall -o main.o -c main.cpp

lexicalAnalyzer.o: lexicalAnalyzer.cpp lexicalAnalyzer.hpp token.hpp symbolTable.hpp helperFunctions.hpp
	g++ -o lexicalAnalyzer.o -c lexicalAnalyzer.cpp

helperFunctions.o: helperFunctions.cpp helperFunctions.hpp
	g++ -o helperFunctions.o -c helperFunctions.cpp

clean:
	-rm main main.o lexicalAnalyzer.o helperFunctions.o
