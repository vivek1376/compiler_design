#ifndef __PARSER_H__
#define __PARSER_H__

#include "token.hpp"
#include "lexicalAnalyzer.hpp"

#define PARSER  Parser::getInstance()

class Parser {
    public:
        bool scanAssume(tokenType ttype);
        static Parser* getInstance();

    private:
        static Parser* instance_;
        Parser() = default;
        LexicalAnalyzer *lexer;
};


#endif
