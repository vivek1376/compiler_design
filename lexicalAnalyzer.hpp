#ifndef __LEXICALANALYZER_H__
#define __LEXICALANALYZER_H__


#include <fstream>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cctype>

#include "inFile.hpp"
#include "symbolTable.hpp"
#include "helperFunctions.hpp"
#include "reporting.hpp"

#define LEXER   LexicalAnalyzer::getInstance()




class LexicalAnalyzer {
    public:
        static LexicalAnalyzer* getInstance();

        void removeWhitespace();
        bool isWhitespace();
        void consumeLineComment();
        bool ifStartBlockComment();
        bool ifEndBlockComment();
        void processComments();
        bool isComment();
        Token* buildToken(bool*, SymInfo**);
        void initSymbolTable();
        void addSymbolTable();
        Token* scan(bool*, SymInfo**);
        bool ifThisChar(char ch);
        void runLexer();
        SymbolTable& getSymbolTable();

        LexicalAnalyzer(LexicalAnalyzer const&) = delete;
        LexicalAnalyzer& operator=(LexicalAnalyzer const&) = delete;

        void setinFile(inFile* infile);
        Token* getlookahead();
        int getPos();
        void setinFilepos(int pos);

    private:
        static LexicalAnalyzer* instance_;
        inFile* srcFile;
        LexicalAnalyzer() = default;  // TODO or delete ?
        // NOTE see https://stackoverflow.com/questions/3871429/class-members-that-are-objects-pointers-or-not-c
        // whether member variable should be pointer or value
        SymbolTable symTab;
};

#endif
