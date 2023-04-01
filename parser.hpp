#ifndef __PARSER_H__
#define __PARSER_H__

#include "token.hpp"
#include "lexicalAnalyzer.hpp"
#include "prodRetType.hpp"

#define PARSER  Parser::getInstance()

class Parser {
    public:
        bool scanAssume(tokenType ttype);
        static Parser* getInstance();
        ProdRetType* parseNT_program();
        ProdRetType* parseNT_bound();
        // TODO constructor: set lookahead token ?
    private:
        static Parser* instance_;
        Parser() = default;
        LexicalAnalyzer *lexer;
        Token* lookahead;  // how to use ?
};


class nt_retType {
    public:
        bool returnCode;

};


#endif
