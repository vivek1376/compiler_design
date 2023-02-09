#include <fstream>
#include <iostream>


void runLexer(std::string);
void readCharacterFromStream(std::ifstream&, char&);

enum tokenType {
	PLUS, MINUS, IF_RW, LOOP_RW, END_RW, L_PAREN, R_PAREN,
	L_BRACKET, R_BRACKET, NUMBER, IDENTIFIER,
	// EOF? add more
};

class Token {

};


class inFile {
	private:
		std::ifstream srcFile;
		std::string fileName;
		int lineCnt = 0;  // the line count; initialized to zero

	public:
		inFile() = delete;
/*
		inFile(std::string fileName) {
			this.fileName = fileName;
			srcFile.open(fileName, std::ios_base::in);
		}

		// bool attachFile(std::string);  // open the named file

		inline char getChar() {

			return srcFile.get();
		}
*/
		// void ungetChar(char);  // push character back to the
							   // input file std::string
		// void incLineCnt();
		// void getLineCnt();
};
