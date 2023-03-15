#ifndef __LEXICALANALYZER_H__
#define __LEXICALANALYZER_H__


#include <fstream>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

#include "token.hpp"
#include "symbolTable.hpp"
#include "helperFunctions.hpp"

#define LEXER   LexicalAnalyzer::getInstance()


void runLexer(std::string);
void readCharacterFromStream(std::ifstream&, char&);

class inFile {
    private:
        std::ifstream srcFile;
        std::string fileName;
        int lineCnt = 0;  // the line count; initialized to zero
                          //

    public:
        inFile() = delete;

        inFile(std::string fileName);

        // bool attachFile(std::string);  // open the named file

        // NOTE place fn defn here in order for it to be inlined. 
        // see https://stackoverflow.com/a/7883046/9894266
        inline char getChar() {
            return srcFile.get();
        }

        bool isgood();

        void incLineCnt() {
        }

        void ungetCh();

        bool isEOF();

        // void incLineCnt();
        // void getLineCnt();
};



class LexicalAnalyzer {
    public:
        static LexicalAnalyzer* getInstance();

        void removeWhitespace(inFile& srcFile);
        bool isWhitespace(inFile& srcFile);
        void consumeLineComment(inFile& srcFile);
        bool ifStartBlockComment(inFile& srcFile);
        bool ifEndBlockComment(inFile& srcFile);
        void processComments(inFile& srcFile);
        bool isComment(inFile& srcFile);
        Token* buildToken(inFile& srcFile, SymbolTable& symTab);
        void initSymbolTable();
        Token* scan(inFile& srcFile);
        bool ifThisChar(char ch);
        void runLexer(std::string filename);
        SymbolTable& getSymbolTable();

    private:
        static LexicalAnalyzer* instance_;
        LexicalAnalyzer() = default;  // TODO or delete ?
        SymbolTable symTab;
};

#endif
