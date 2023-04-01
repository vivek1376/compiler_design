main: main.o lexicalAnalyzer.o helperFunctions.o symbolTable.o token.o parser.o inFile.o prodRetType.o
	g++ -std=c++14 -Wall -o main main.o lexicalAnalyzer.o helperFunctions.o symbolTable.o token.o parser.o inFile.o prodRetType.o

main.o: main.cpp lexicalAnalyzer.hpp
	g++ -std=c++14 -Wall -o main.o -c main.cpp

lexicalAnalyzer.o: lexicalAnalyzer.cpp lexicalAnalyzer.hpp token.hpp symbolTable.hpp helperFunctions.hpp inFile.hpp
	g++ -std=c++14 -o lexicalAnalyzer.o -c lexicalAnalyzer.cpp

parser.o: parser.cpp parser.hpp token.hpp lexicalAnalyzer.cpp
	g++ -std=c++14 -o parser.o -c parser.cpp

symbolTable.o: symbolTable.cpp symbolTable.hpp
	g++ -std=c++14 -o symbolTable.o -c symbolTable.cpp

token.o: token.cpp token.hpp
	g++ -std=c++14 -o token.o -c token.cpp

inFile.o: inFile.cpp inFile.hpp
	g++ -std=c++14 -o inFile.o -c inFile.cpp

helperFunctions.o: helperFunctions.cpp helperFunctions.hpp
	g++ -std=c++14 -o helperFunctions.o -c helperFunctions.cpp

prodRetType.o: prodRetType.cpp prodRetType.hpp
	g++ -std=c++14 -o prodRetType.o -c prodRetType.cpp

.PHONY: clean

clean:
	-rm main *.o


