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


void parse_program() {

}


void parse_program_header() {

}


void parse_program_body() {

}


void parse_program_identifier() {

}


void parse_declaration() {

}


void parse_statement() {

}


void parse_procedure_declaration() {

}


void parse_variable_declaration() {

}


void parse_procedure_header() {

}


void parse_procedure_body() {

}


void parse_identifier() {

}


void parse_type_mark() {

}


void parse_parameter_list() {

}


void parse_parameter() {

}


void parse_bound() {

}


void parse_number() {

}


void parse_assignment_statement() {

}


void parse_if_statement() {

}


void parse_loop_statement() {

}


void parse_return_statement() {

}


void parse_procedure_call() {

}


void parse_argument_list() {

}


void parse_destination() {

}


void parse_expression() {

}


void parse_arithOp() {

}


void parse_expression_() {

}


void parse_relation() {

}


void parse_arithOp_() {

}


void parse_term() {

}


void parse_factor() {

}


void parse_relation_() {

}


void parse_term_() {

}


void parse_name() {

}


void parse_string() {

}
