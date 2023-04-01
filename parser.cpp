#include "parser.hpp"

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


ProdRetType* Parser::parseNT_bound() {
    /* scanAssume( */
}
