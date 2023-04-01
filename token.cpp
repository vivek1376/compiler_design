#include "token.hpp"



Token::Token(tokenType ttype, std::string tokenStr) {
    this->tokenStr = tokenStr;
    this->ttype = ttype;
}

/* Token::Token(tokenType ttype, Token* tok) { */
/*     if (tok != NULL) { */
/*         tok->ttype = ttype; */
/*     } */
/* } */

void Token::setTokenType(tokenType ttype) {
    this->ttype = ttype;
}

void Token::printToken() {
    std::cout << "<" << map_ttypeStr[this->ttype] << ">";
    std::cout << "" << this->tokenStr << std::endl;
}

tokenType Token::getTokenType() {
    return this->ttype;
}
