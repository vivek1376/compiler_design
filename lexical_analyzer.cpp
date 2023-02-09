#include "lexical_analyzer.hpp"

int main() {

	std::string filename = "testfile";

	runLexer(filename);
	// inFile insrcFile("dfsdf");


	return 0;
}

void runLexer(std::string filename) {
/*
	std::ifstream infile(filename, std::ios_base::in);

	char nextCh;

	while (infile.good()) {
		readCharacterFromStream(infile, nextCh);

		std::cout << nextCh;
		// std::cout << nextCh << std::endl;
	}
*/

}

inline void readCharacterFromStream(std::ifstream& infile, char &ch) {

	infile.get(ch);
}
