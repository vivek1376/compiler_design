#ifndef __TOKEN_H__
#define __TOKEN_H__


#include <iostream>
#include <string>



enum tokenType {
    PLUS, MINUS, IF_RW, LOOP_RW, END_RW, L_PAREN, R_PAREN,
    L_BRACKET, R_BRACKET, NUMBER, IDENTIFIER,
    LESS_THAN, LESS_EQUAL, INTEGER, FLOAT, STRING, UNDEF, INVALID
    // EOF? add more
};

class Token {
    private:
        tokenType ttype;
        std::string tokenStr;

    public:

        Token() = delete;

        Token(tokenType ttype, std::string tokenStr) {
            this->tokenStr = tokenStr;
            this->ttype = ttype;
        }

        Token(tokenType ttype, Token* tok) {
            if (tok != NULL) {
                tok->ttype = ttype;
            }
        }
};


#endif
