#include "lexical_analyzer.hpp"
#include <cstdio>

int main() {

	std::string filename = "testfile";

	runLexer(filename);




	return 0;
}

void runLexer(std::string filename) {

	inFile srcfile("testfile");

    std::cout << std::hex << srcfile.getChar();
	std::cout << std::hex << srcfile.getChar();
	std::cout << std::hex << srcfile.getChar();
	std::cout << std::hex << srcfile.getChar();
	std::cout << std::hex << srcfile.getChar();
	std::cout << std::hex << srcfile.getChar();
	std::cout << std::hex << srcfile.getChar();
	std::cout << std::hex << srcfile.getChar();


	std::cout << "good? " << srcfile.isgood() << std::endl;
/* 	std::ifstream infile(filename); */

/* 	char nextCh; */

/* 	while (infile.good()) { */
/* 		readCharacterFromStream(infile, nextCh); */

/* 		std::cout << nextCh; */
/* 		// std::cout << nextCh << std::endl; */
/* 	} */

}

/* inline void readCharacterFromStream(std::ifstream& infile, char &ch) { */

/* 	infile.get(ch); */
/* } */
