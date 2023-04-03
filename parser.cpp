#include "parser.hpp"


Parser* Parser::instance_ = nullptr;


/* bool Parser::scanAssume(tokenType ttype) { */
/*     /1* auto tok = lexer->scan(); *1/ */

/*     /1* if (tok->tokenType == ttype) return true; *1/ */

/*     /1* return false; *1/ */


/*     if (lookahead->getTokenType() == ttype) { */
/*         this->lookahead = lexer->scan(); */
/*         return true; */
/*     } else { */
/*         // TODO report error */
/*         return false; */
/*     } */
/* } */

LexicalAnalyzer* Parser::getLexer() {
    return lexer;
}


void Parser::initLexer(LexicalAnalyzer* lexer) {
    this->lexer = lexer;
}

Parser* Parser::getInstance() {
    if (!instance_) {
        instance_ = new Parser();
    }

    return instance_;
}


Token* Parser::match(tokenType tt) {
    // match token and consume (scan() will advance file pointer)
    Token *nextTok = lexer->scan();

    std::cout << "match() actual str: " << nextTok->getTokenStr() + ", " 
        + nextTok->getTokenTypeStr() + "\n";
    /* if (nextTok->getTokenType() == tk->getTokenType() && */
    /*         nextTok->getTokenStr() == tk->getTokenStr()) */
    /*     return nextTok; */

    if (nextTok->getTokenType() == tt)
        return nextTok;

    Token tmpTok(tt, "tmptoken");
    throw std::runtime_error("scan_assume_failed expected/actual: " + tmpTok.getTokenTypeStr()
            + "/"
            + nextTok->getTokenTypeStr());
}


// nt_* parsers

nt_retType_program* Parser::parse_program() {

    /* currTok = lexer->scan(); */

    nt_retType_program* ptr_ret = new nt_retType_program();

    ptr_ret->ptr_program_header = parse_program_header();
    ptr_ret->ptr_program_body = parse_program_body();

    ptr_ret->ptr_tk_dot = match(tokenType::DOT);

    return ptr_ret;
}


nt_retType_program_header* Parser::parse_program_header() {
    nt_retType_program_header* ptr_ret = new nt_retType_program_header();

    // Token* tk = new Token(tokenType::PROGRAM_RW, "program");
    // match(tk);

    ptr_ret->ptr_tk_program = match(tokenType::PROGRAM_RW);
    ptr_ret->ptr_identifier = parse_identifier();

    ptr_ret->ptr_tk_is = match(tokenType::IS_RW);

    return ptr_ret;
}


nt_retType_program_body* Parser::parse_program_body() {

    nt_retType_program_body* ptr_ret = new nt_retType_program_body();

    Token* lookahead = lexer->getlookahead();

    nt_retType_declaration* ptr_declaration;
    Token* tk_semicolon;

    while (lookahead->getTokenType() != tokenType::BEGIN_RW 
            && lookahead->getTokenStr() != "begin") {
        ptr_declaration = parse_declaration();
        tk_semicolon = match(tokenType::SEMICOLON);

        ptr_ret->vec_declaration_tksemicolon.push_back(std::make_pair(ptr_declaration,
                    tk_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_begin = match(tokenType::BEGIN_RW);

    lookahead = lexer->getlookahead();

    nt_retType_statement* ptr_statement;
    while (lookahead->getTokenType() != tokenType::END_RW
            && lookahead->getTokenStr() != "end") {

        ptr_statement = parse_statement();
        tk_semicolon = match(tokenType::SEMICOLON);

        ptr_ret->vec_statement_tksemicolon.push_back(std::make_pair(ptr_statement,
                    tk_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_end = match(tokenType::END_RW);
    ptr_ret->ptr_tk_program = match(tokenType::PROGRAM_RW);

    return ptr_ret;
}


nt_retType_declaration* Parser::parse_declaration() {

    nt_retType_declaration* ptr_ret = new nt_retType_declaration();

    Token *lookahead = lexer->getlookahead();

    ptr_ret->ptr_tk_global = nullptr;
    if (lookahead->getTokenType() == tokenType::GLOBAL_RW) {
        ptr_ret->ptr_tk_global = match(tokenType::GLOBAL_RW);
    }


    ptr_ret->ptr_procedure_declaration = nullptr;  // TODO add this in class constructor
    ptr_ret->ptr_variable_declaration = nullptr;

    lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::PROCEDURE_RW) {
        ptr_ret->ptr_procedure_declaration = parse_procedure_declaration();
        ptr_ret->whichRule = 0;
    } else if (lookahead->getTokenType() == tokenType::VARIABLE_RW) {
        ptr_ret->ptr_variable_declaration = parse_variable_declaration();
        ptr_ret->whichRule = 1;
    } else {
        throw std::runtime_error("error in parser_declaration(), lookahead_str: " 
                + lookahead->getTokenStr());
    }

    return ptr_ret;
}


nt_retType_statement* Parser::parse_statement() {

}


nt_retType_procedure_declaration* Parser::parse_procedure_declaration() {

}


nt_retType_variable_declaration* Parser::parse_variable_declaration() {
    nt_retType_variable_declaration* ptr_ret = new nt_retType_variable_declaration();

    ptr_ret->ptr_tk_variable = match(tokenType::VARIABLE_RW);
    ptr_ret->ptr_identifier = parse_identifier();
    ptr_ret->ptr_tk_colon = match(tokenType::COLON);
    ptr_ret->ptr_type_mark = parse_type_mark();

    ptr_ret->ptr_tk_lbkt = nullptr;
    ptr_ret->ptr_tk_rbkt = nullptr;
    ptr_ret->ptr_bound = nullptr;

    Token* lookahead = lexer->getlookahead();

    // TODO delete whichRUle from this class
    if (lookahead->getTokenType() == tokenType::L_BRACKET) {
        ptr_ret->ptr_tk_lbkt = match(tokenType::L_BRACKET);
        ptr_ret->ptr_bound = parse_bound();
        ptr_ret->ptr_tk_rbkt = match(tokenType::R_BRACKET);
    }

    return ptr_ret;
}


nt_retType_procedure_header* Parser::parse_procedure_header() {

}


nt_retType_procedure_body* Parser::parse_procedure_body() {

}


nt_retType_identifier* Parser::parse_identifier() {
   nt_retType_identifier* ptr_ret = new nt_retType_identifier();

   ptr_ret->ptr_tk_str = match(tokenType::IDENTIFIER);

   return ptr_ret;
   /* Token* tk = lexer->scan(); */
   /* if (tk->getTokenType() == tokenType::IDENTIFIER) { */
   /*     ptr_ret->ptr_tk_str = tk; */
   /*     return ptr_ret; */
   /* } */

   /* throw std::runtime_error("error in nt_retType_identifier()"); */
}


nt_retType_type_mark* Parser::parse_type_mark() {
    nt_retType_type_mark* ptr_ret = new nt_retType_type_mark();

    ptr_ret->ptr_tk_bool = nullptr;
    ptr_ret->ptr_tk_float = nullptr;
    ptr_ret->ptr_tk_string = nullptr;
    ptr_ret->ptr_tk_integer = nullptr;

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::INTEGER_RW)
        ptr_ret->ptr_tk_integer = match(tokenType::INTEGER_RW);
    else if (lookahead->getTokenType() == tokenType::FLOAT_RW)
        ptr_ret->ptr_tk_integer = match(tokenType::FLOAT_RW);
    else if (lookahead->getTokenType() == tokenType::STRING_RW)
        ptr_ret->ptr_tk_integer = match(tokenType::STRING_RW);
    else if (lookahead->getTokenType() == tokenType::BOOL_RW)
        ptr_ret->ptr_tk_integer = match(tokenType::BOOL_RW);

    // TODO throw

    return ptr_ret;
}


nt_retType_parameter_list* Parser::parse_parameter_list() {

}


nt_retType_parameter* Parser::parse_parameter() {

}


nt_retType_bound* Parser::parse_bound() {

}


nt_retType_number* Parser::parse_number() {

}


nt_retType_assignment_statement* Parser::parse_assignment_statement() {

}


nt_retType_if_statement* Parser::parse_if_statement() {

}


nt_retType_loop_statement* Parser::parse_loop_statement() {

}


nt_retType_return_statement* Parser::parse_return_statement() {

}


nt_retType_procedure_call* Parser::parse_procedure_call() {

}


nt_retType_argument_list* Parser::parse_argument_list() {

}


nt_retType_destination* Parser::parse_destination() {

}


nt_retType_expression* Parser::parse_expression() {

}


nt_retType_arithOp* Parser::parse_arithOp() {

}


nt_retType_expression_* Parser::parse_expression_() {

}


nt_retType_relation* Parser::parse_relation() {

}


nt_retType_arithOp_* Parser::parse_arithOp_() {

}


nt_retType_term* Parser::parse_term() {

}


nt_retType_factor* Parser::parse_factor() {

}


nt_retType_relation_* Parser::parse_relation_() {

}


nt_retType_term_* Parser::parse_term_() {

}


nt_retType_name* Parser::parse_name() {

}


nt_retType_string* Parser::parse_string() {

}
