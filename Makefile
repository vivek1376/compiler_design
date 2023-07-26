all: main

main: main.o lexicalAnalyzer.o helperFunctions.o symbolTable.o token.o parser.o inFile.o prodRetType.o reporting.o
	g++ -std=c++14 -Wall -o main main.o lexicalAnalyzer.o helperFunctions.o symbolTable.o token.o parser.o inFile.o prodRetType.o reporting.o

main.o: main.cpp lexicalAnalyzer.hpp
	g++ -std=c++14 -Wall -g -o main.o -c main.cpp

lexicalAnalyzer.o: lexicalAnalyzer.cpp lexicalAnalyzer.hpp symbolTable.hpp helperFunctions.hpp inFile.hpp reporting.hpp
	g++ -std=c++14 -Wall -g -o lexicalAnalyzer.o -c lexicalAnalyzer.cpp

parser.o: parser.cpp parser.hpp lexicalAnalyzer.cpp symbolTable.hpp
	g++ -std=c++14 -Wall -g -o parser.o -c parser.cpp

symbolTable.o: symbolTable.cpp symbolTable.hpp token.hpp
	g++ -std=c++14 -Wall -g -o symbolTable.o -c symbolTable.cpp

token.o: token.cpp token.hpp
	g++ -std=c++14 -Wall -g -o token.o -c token.cpp

inFile.o: inFile.cpp inFile.hpp
	g++ -std=c++14 -Wall -g -o inFile.o -c inFile.cpp

helperFunctions.o: helperFunctions.cpp helperFunctions.hpp
	g++ -std=c++14 -Wall -g -o helperFunctions.o -c helperFunctions.cpp

prodRetType.o: prodRetType.cpp prodRetType.hpp
	g++ -std=c++14 -Wall -g -o prodRetType.o -c prodRetType.cpp

reporting.o: reporting.cpp reporting.hpp
	g++ -std=c++14 -Wall -g -o reporting.o -c reporting.cpp

.PHONY: clean

clean:
	-rm main *.o


