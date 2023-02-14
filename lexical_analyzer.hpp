#include <fstream>
#include <unordered_map>
#include <iostream>


void runLexer(std::string);
void readCharacterFromStream(std::ifstream&, char&);

enum tokenType {
    PLUS, MINUS, IF_RW, LOOP_RW, END_RW, L_PAREN, R_PAREN,
    L_BRACKET, R_BRACKET, NUMBER, IDENTIFIER,
    // EOF? add more
};

class Token {
    private:
        tokenType tt;
        std::string tokenStr;

    public:

        Token() = delete;

/*         setTokenType(tokenType tt) { */
/*         } */
};


class inFile {
    private:
        std::ifstream srcFile;
        std::string fileName;
        int lineCnt = 0;  // the line count; initialized to zero
                          //

    public:
        inFile() = delete;

        inFile(std::string fileName)
            : fileName{fileName} {
            /* this.fileName = fileName; */
            srcFile.open(fileName, std::ios_base::in);
        }

        // bool attachFile(std::string);  // open the named file

        inline char getChar() {

            return srcFile.get();
        }

        bool isgood() {
            // see https://stackoverflow.com/a/4533102/9894266
            return srcFile.good(); 
            /* return srcFile.good() && !this->isEOF(); */
        }

        void incLineCnt() {
        }

        void ungetCh() {
            srcFile.unget();
        }

        bool isEOF() {
            // see https://stackoverflow.com/a/6283787/9894266

            int c = srcFile.peek();
            if (c == EOF) {
                if (srcFile.eof())
                    return true;
                else
                    // error ?
                    // throw exception ?
                    return true;
            } else {
                return false;
            }
        }

        // void incLineCnt();
        // void getLineCnt();
};



class SymbolTable {
    private:
        std::unordered_map<std::string, Token> symTab;

    public:
        Token lookupTokenString(std::string) {

        }

};
