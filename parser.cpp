#include "parser.hpp"


Parser* Parser::instance_ = nullptr;


bool Parser::scanAssume(tokenType ttype) {
    /* auto tok = lexer->scan(); */

    /* if (tok->tokenType == ttype) return true; */

    /* return false; */


    if (lookahead->getTokenType() == ttype) {
        this->lookahead = lexer->scan();
        return true;
    } else {
        // TODO report error
        return false;
    }
}

LexicalAnalyzer* Parser::getLexer() {
    return lexer;
}

ProdRetType* Parser::parseNT_bound() {
    /* scanAssume( */
    return nullptr;
}

void Parser::setLexer(LexicalAnalyzer* lexer) {
    this->lexer = lexer;
}

Parser* Parser::getInstance() {
    if (!instance_) {
        instance_ = new Parser();
    }

    return instance_;
}
