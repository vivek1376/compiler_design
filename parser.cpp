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

void Parser::initLogger(Reporting* logger) {
    this->logger = logger;
}


Parser* Parser::getInstance() {
    if (!instance_) {
        instance_ = new Parser();
    }

    return instance_;
}


Token* Parser::match(tokenType tt, bool* inCurrentScope) {
    // match token and consume (scan() will advance file pointer)
    Token *tok = lexer->scan(inCurrentScope);

    std::cout << "match() actual str:\t\t" << tok->getTokenStr() + "\t\t"
        + tok->getTokenTypeStr() + "\n";

    /* if (nextTok->getTokenType() == tk->getTokenType() && */
    /*         nextTok->getTokenStr() == tk->getTokenStr()) */
    /*     return nextTok; */

    if (tok->getTokenType() == tt) {
        return tok;
    } else {
        Token tmpTok(tt, "tmptoken");
        logger->reportError("token not matched - expected / actual: " 
                + tmpTok.getTokenStr()
                + " / "
                + tok->getTokenStr());

        // tokenType isn't as expected
        tok->setTokenType(tokenType::INVALID);
        return tok;
    }

    /* Token tmpTok(tt, "tmptoken"); */
    /* throw std::runtime_error("scan_assume_failed expected/actual: " + tmpTok.getTokenTypeStr() */
    /*         + "/" */
    /*         + tok->getTokenTypeStr()); */
}


// nt_* parsers

nt_retType::nt_retType() {
    // set true by default; upon parse/type check error, set returnCode false explicitly
    returnCode = true;
}

nt_retType_program* Parser::parse_program() {

    /* currTok = lexer->scan(); */
    std::cout << "parse_program()" << std::endl;

    nt_retType_program* ptr_ret = new nt_retType_program();

    ptr_ret->ptr_program_header = parse_program_header();
    ptr_ret->ptr_program_body = parse_program_body();

    ptr_ret->ptr_tk_dot = match(tokenType::DOT, nullptr);

    return ptr_ret;
}


nt_retType_program_header* Parser::parse_program_header() {
    nt_retType_program_header* ptr_ret = new nt_retType_program_header();

    // Token* tk = new Token(tokenType::PROGRAM_RW, "program");
    // match(tk);

    ptr_ret->ptr_tk_program = match(tokenType::PROGRAM_RW, nullptr);
    ptr_ret->ptr_identifier = parse_identifier(nullptr);

    ptr_ret->ptr_tk_is = match(tokenType::IS_RW, nullptr);

    return ptr_ret;
}


nt_retType_program_body* Parser::parse_program_body() {

    nt_retType_program_body* ptr_ret = new nt_retType_program_body();

    Token* lookahead = lexer->getlookahead();

    nt_retType_declaration* ptr_declaration;
    Token* tk_semicolon;

    while (lookahead->getTokenType() != tokenType::BEGIN_RW
            && lookahead->getTokenStr() != "begin") {  // TODO second condition redundant ?
        ptr_declaration = parse_declaration();
        tk_semicolon = match(tokenType::SEMICOLON, nullptr);

        ptr_ret->vec_declaration_tksemicolon.push_back(std::make_pair(ptr_declaration,
                    tk_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_begin = match(tokenType::BEGIN_RW, nullptr);

    lookahead = lexer->getlookahead();

    nt_retType_statement* ptr_statement;
    while (lookahead->getTokenType() != tokenType::END_RW
            && lookahead->getTokenStr() != "end") {

        ptr_statement = parse_statement();
        tk_semicolon = match(tokenType::SEMICOLON, nullptr);

        ptr_ret->vec_statement_tksemicolon.push_back(std::make_pair(ptr_statement,
                    tk_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_end = match(tokenType::END_RW, nullptr);
    ptr_ret->ptr_tk_program = match(tokenType::PROGRAM_RW, nullptr);

    return ptr_ret;
}


nt_retType_declaration* Parser::parse_declaration() {

    nt_retType_declaration* ptr_ret = new nt_retType_declaration();

    Token *lookahead = lexer->getlookahead();

    ptr_ret->ptr_tk_global = nullptr;
    if (lookahead->getTokenType() == tokenType::GLOBAL_RW) {
        ptr_ret->ptr_tk_global = match(tokenType::GLOBAL_RW, nullptr);
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

    nt_retType_statement* ptr_ret = new nt_retType_statement();

    Token *lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::IF_RW) {
        ptr_ret->ptr_if_statement = parse_if_statement();
    } else if (lookahead->getTokenType() == tokenType::RETURN_RW) {
        ptr_ret->ptr_return_statement = parse_return_statement();
    } else if (lookahead->getTokenType() == tokenType::FOR_RW) {
        ptr_ret->ptr_loop_statement = parse_loop_statement();
    } else {  // assignment
        ptr_ret->ptr_assignment_statement = parse_assignment_statement();
    }

    return ptr_ret;
}


nt_retType_procedure_declaration* Parser::parse_procedure_declaration() {
    
    auto ptr_ret = new nt_retType_procedure_declaration();

    ptr_ret->ptr_procedure_header = parse_procedure_header();
    ptr_ret->ptr_procedure_body = parse_procedure_body();

    return ptr_ret;
}


nt_retType_variable_declaration* Parser::parse_variable_declaration() {

    nt_retType_variable_declaration* ptr_ret = new nt_retType_variable_declaration();

    SymInfo* syminfo = new SymInfo();

    ptr_ret->ptr_tk_variable = match(tokenType::VARIABLE_RW, nullptr);  // TODO add ret value check for invalid token ?

    if (ptr_ret->ptr_tk_variable->getTokenType() == tokenType::INVALID) {
        logger->reportError("VARIABLE_RW not found!");
        ptr_ret->returnCode = false;
    }


    bool inCurrentScope;
    ptr_ret->ptr_identifier = parse_identifier(&inCurrentScope);

    if (ptr_ret->ptr_identifier->returnCode == false) {
        logger->reportError("Identifier parse error");
        ptr_ret->returnCode = false;
    }

    // check duplicate
    if (inCurrentScope == true) {
        logger->reportError("Duplicate symbol declaration");
        ptr_ret->returnCode = false;

        return ptr_ret;
    }


    ptr_ret->ptr_tk_colon = match(tokenType::COLON, nullptr);


    ptr_ret->ptr_type_mark = parse_type_mark();

    if (ptr_ret->ptr_type_mark->ptr_tk_integer) {
        syminfo->symdtype = symDatatype::INT_DTYPE;
    } else if (ptr_ret->ptr_type_mark->ptr_tk_float) {
        syminfo->symdtype = symDatatype::FLOAT_DTYPE;
    } else if (ptr_ret->ptr_type_mark->ptr_tk_string) {
        syminfo->symdtype = symDatatype::STR_DTYPE;
    } else if (ptr_ret->ptr_type_mark->ptr_tk_bool) {
        syminfo->symdtype = symDatatype::BOOL_DTYPE;
    }




    ptr_ret->ptr_tk_lbkt = nullptr;
    ptr_ret->ptr_tk_rbkt = nullptr;
    ptr_ret->ptr_bound = nullptr;

    Token* lookahead = lexer->getlookahead();

    // TODO delete whichRUle from this class
    if (lookahead->getTokenType() == tokenType::L_BRACKET) {
        /* SymInfo_array *sym_arr = static_cast<SymInfo_array*>(&syminfo); */
        /* SymInfo_array *sym_arr = new SymInfo_array(dynamic_cast<const SymInfo_array&>(syminfo)); */
        SymInfo_array *sym_arr = new SymInfo_array(*syminfo);

        ptr_ret->ptr_tk_lbkt = match(tokenType::L_BRACKET, nullptr);
        ptr_ret->ptr_bound = parse_bound();
        ptr_ret->ptr_tk_rbkt = match(tokenType::R_BRACKET, nullptr);

        /* ptr_ret->syminfo = sym_arr; */
    }

    // add type checking
 




    return ptr_ret;
}


nt_retType_procedure_header* Parser::parse_procedure_header() {

    auto ptr_ret = new nt_retType_procedure_header();

    ptr_ret->ptr_tk_procedure = match(tokenType::PROCEDURE_RW, nullptr);
    ptr_ret->ptr_identifier = parse_identifier(nullptr);
    ptr_ret->ptr_tk_colon = match(tokenType::COLON, nullptr);
    ptr_ret->ptr_type_mark = parse_type_mark();

    ptr_ret->ptr_tk_lparen = match(tokenType::L_PAREN, nullptr);

    auto lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() != tokenType::R_PAREN) {
        ptr_ret->ptr_parameter_list = parse_parameter_list();
    }

    ptr_ret->ptr_tk_rparen = match(tokenType::R_PAREN, nullptr);

    return ptr_ret;
}


nt_retType_procedure_body* Parser::parse_procedure_body() {

    auto ptr_ret = new nt_retType_procedure_body();

    auto lookahead = lexer->getlookahead();

    while (lookahead->getTokenType() != tokenType::BEGIN_RW) {

        auto p_first_decl = parse_declaration();
        auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr);

        ptr_ret->vec_declaration_tkcolon.push_back(std::make_pair(p_first_decl,
                    p_second_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_begin = match(tokenType::BEGIN_RW, nullptr);

    lookahead = lexer->getlookahead();
    while (lookahead->getTokenType() != tokenType::END_RW) {

        auto p_first_st = parse_statement();
        auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr);

        ptr_ret->vec_statement_tkcolon.push_back(std::make_pair(p_first_st, 
                    p_second_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_end = match(tokenType::END_RW, nullptr);
    ptr_ret->ptr_tk_procedure = match(tokenType::PROCEDURE_RW, nullptr);

    return ptr_ret;
}


nt_retType_identifier* Parser::parse_identifier(bool *inCurrentScope) {
    // TODO define identifier only as terminal ?

    nt_retType_identifier* ptr_ret = new nt_retType_identifier();

    ptr_ret->ptr_tk_str = match(tokenType::IDENTIFIER, inCurrentScope);

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
        ptr_ret->ptr_tk_integer = match(tokenType::INTEGER_RW, nullptr);
    else if (lookahead->getTokenType() == tokenType::FLOAT_RW)
        ptr_ret->ptr_tk_float = match(tokenType::FLOAT_RW, nullptr);
    else if (lookahead->getTokenType() == tokenType::STRING_RW)
        ptr_ret->ptr_tk_string = match(tokenType::STRING_RW, nullptr);
    else if (lookahead->getTokenType() == tokenType::BOOL_RW)
        ptr_ret->ptr_tk_bool = match(tokenType::BOOL_RW, nullptr);

    // TODO throw

    return ptr_ret;
}


nt_retType_parameter_list* Parser::parse_parameter_list() {

    auto ptr_ret = new nt_retType_parameter_list();

    ptr_ret->ptr_parameter = parse_parameter();

    auto lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::COMMA) {
        ptr_ret->ptr_tk_comma = match(tokenType::COMMA, nullptr);
        ptr_ret->ptr_parameter_list = parse_parameter_list();
    }

    return ptr_ret;
}


nt_retType_parameter* Parser::parse_parameter() {

    auto ptr_ret = new nt_retType_parameter();

    ptr_ret->ptr_variable_declaration = parse_variable_declaration();

    return ptr_ret;
}


nt_retType_bound* Parser::parse_bound() {

    auto ptr_ret = new nt_retType_bound();

    ptr_ret->ptr_number = parse_number();

    return ptr_ret;
}


nt_retType_number* Parser::parse_number() {

    std::cerr << "parsing no." << std::endl;
    std::cerr << std::endl;

    nt_retType_number* ptr_ret = new nt_retType_number();

    auto lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::INTEGER)
        ptr_ret->ptr_tk_number = match(tokenType::INTEGER, nullptr);
    else if (lookahead->getTokenType() == tokenType::FLOAT)
        ptr_ret->ptr_tk_number = match(tokenType::FLOAT, nullptr);
    else
        throw std::runtime_error("can't parse number, actual string: " +
                lookahead->getTokenStr());

    return ptr_ret;
}


nt_retType_assignment_statement* Parser::parse_assignment_statement() {

    std::cout << "parse_assignment_statement():\n";
    std::cout << std::endl;

    auto ptr_ret = new nt_retType_assignment_statement();

    ptr_ret->ptr_destination = parse_destination();
    ptr_ret->ptr_tk_assign = match(tokenType::ASSIGN_OP, nullptr);
    ptr_ret->ptr_expression = parse_expression();

    return ptr_ret;
}


nt_retType_if_statement* Parser::parse_if_statement() {

    // TODO ignoring few match() call returns
    nt_retType_if_statement* ptr_ret = new nt_retType_if_statement();

    ptr_ret->ptr_tk_if = match(tokenType::IF_RW, nullptr);
    match(tokenType::L_PAREN, nullptr);  // NOTE ignoring return pointer
    ptr_ret->ptr_expression = parse_expression();
    match(tokenType::R_PAREN, nullptr);

    ptr_ret->ptr_tk_then = match(tokenType::THEN_RW, nullptr);

    Token* lookahead = lexer->getlookahead();

    while (lookahead->getTokenType() != tokenType::ELSE_RW &&
            lookahead->getTokenType() != tokenType::END_RW) {
        auto p_first_st = parse_statement();
        auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr);

        ptr_ret->vec_statement_semicolon.push_back(std::make_pair(p_first_st,
                    p_second_semicolon));

        lookahead = lexer->getlookahead();
    }

    lookahead = lexer->getlookahead();  // TODO redundant due to previous getlookahead() call ?
    if (lookahead->getTokenType() == tokenType::ELSE_RW) {

        ptr_ret->ptr_tk_else = match(tokenType::ELSE_RW, nullptr);

        lookahead = lexer->getlookahead();

        while (lookahead->getTokenType() != tokenType::END_RW) {

            auto p_first_st = parse_statement();
            auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr);

            ptr_ret->vec_statement_semicolon_2.push_back(std::make_pair(p_first_st,
                        p_second_semicolon));

            lookahead = lexer->getlookahead();
        }
    }

    ptr_ret->ptr_tk_end = match(tokenType::END_RW, nullptr);
    ptr_ret->ptr_tk_if_2 = match(tokenType::IF_RW, nullptr);

    return ptr_ret;
}


nt_retType_loop_statement* Parser::parse_loop_statement() {

    auto ptr_ret = new nt_retType_loop_statement();

    ptr_ret->ptr_tk_for = match(tokenType::FOR_RW, nullptr);
    ptr_ret->ptr_tk_lparen = match(tokenType::L_PAREN, nullptr);
    ptr_ret->ptr_assignment_statement = parse_assignment_statement();
    ptr_ret->ptr_tk_semicolon = match(tokenType::SEMICOLON, nullptr);
    ptr_ret->ptr_expression = parse_expression();
    ptr_ret->ptr_tk_rparen = match(tokenType::R_PAREN, nullptr);

    auto lookahead = lexer->getlookahead();

    while (lookahead->getTokenType() != tokenType::END_RW) {
        auto p_first_st = parse_statement();
        auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr);

        ptr_ret->vec_statement_semicolon.push_back(std::make_pair(p_first_st,
                    p_second_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_end = match(tokenType::END_RW, nullptr);
    ptr_ret->ptr_tk_for = match(tokenType::FOR_RW, nullptr);

    return ptr_ret;
}


nt_retType_return_statement* Parser::parse_return_statement() {
    
    auto ptr_ret = new nt_retType_return_statement();

    ptr_ret->ptr_tk_return = match(tokenType::RETURN_RW, nullptr);
    ptr_ret->ptr_expression = parse_expression();

    return ptr_ret;
}


nt_retType_procedure_call* Parser::parse_procedure_call() {
    // NOTE handled in factor parse fn
}


nt_retType_argument_list* Parser::parse_argument_list() {

    auto ptr_ret = new nt_retType_argument_list();

    ptr_ret->ptr_expression = parse_expression();

    auto lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::COMMA) {
        ptr_ret->ptr_tk_comma = match(tokenType::COMMA, nullptr);
        ptr_ret->ptr_argument_list = parse_argument_list();
    }

    return ptr_ret;
}


nt_retType_destination* Parser::parse_destination() {

    nt_retType_destination *ptr_ret = new nt_retType_destination();

    ptr_ret->ptr_identifier = parse_identifier(nullptr);

    Token* lookahead = lexer->getlookahead();

    // TODO delete whichRUle from this class
    if (lookahead->getTokenType() == tokenType::L_BRACKET) {
        ptr_ret->ptr_tk_lbkt = match(tokenType::L_BRACKET, nullptr);
        ptr_ret->ptr_expression = parse_expression();
        ptr_ret->ptr_tk_rbkt = match(tokenType::R_BRACKET, nullptr);
    }

    return ptr_ret;
}


nt_retType_expression* Parser::parse_expression() {

    nt_retType_expression* ptr_ret = new nt_retType_expression();

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::NOT_RW) {
        ptr_ret->ptr_tk_not = match(tokenType::NOT_RW, nullptr);
    }

    ptr_ret->ptr_arithOp = parse_arithOp();
    ptr_ret->ptr_expression_ = parse_expression_();

    return ptr_ret;
}


nt_retType_arithOp* Parser::parse_arithOp() {

    nt_retType_arithOp* ptr_ret = new nt_retType_arithOp();

    ptr_ret->ptr_relation = parse_relation();
    ptr_ret->ptr_arithOp_ = parse_arithOp_();

    return ptr_ret;
}


nt_retType_expression_* Parser::parse_expression_() {
    nt_retType_expression_* ptr_ret = new nt_retType_expression_();

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::AMPERSAND) {
        ptr_ret->ptr_tk_ampersand = match(tokenType::AMPERSAND, nullptr);
        ptr_ret->ptr_arithOp = parse_arithOp();
        ptr_ret->ptr_expression_ = parse_expression_();
    } else if (lookahead->getTokenType() == tokenType::PIPE) {
        ptr_ret->ptr_tk_pipe = match(tokenType::PIPE, nullptr);
        ptr_ret->ptr_arithOp = parse_arithOp();
        ptr_ret->ptr_expression_ = parse_expression_();
    } else {
        ptr_ret->ptr_tk_pipe = nullptr;
        ptr_ret->ptr_arithOp = nullptr;
        ptr_ret->ptr_expression_ = nullptr;
    }

    return ptr_ret;
}


nt_retType_relation* Parser::parse_relation() {

    nt_retType_relation* ptr_ret = new nt_retType_relation();

    ptr_ret->ptr_term = parse_term();
    ptr_ret->ptr_relation_ = parse_relation_();

    return ptr_ret;
}


nt_retType_arithOp_* Parser::parse_arithOp_() {

    nt_retType_arithOp_* ptr_ret = new nt_retType_arithOp_();

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::PLUS) {
        ptr_ret->ptr_tk_plus = match(tokenType::PLUS, nullptr);
        ptr_ret->ptr_relation = parse_relation();
        ptr_ret->ptr_arithOp_ = parse_arithOp_();
    } else if (lookahead->getTokenType() == tokenType::MINUS) {
        ptr_ret->ptr_tk_plus = match(tokenType::MINUS, nullptr);
        ptr_ret->ptr_relation = parse_relation();
        ptr_ret->ptr_arithOp_ = parse_arithOp_();
    } else {
        ptr_ret->ptr_tk_plus = nullptr;
        ptr_ret->ptr_relation = nullptr;
        ptr_ret->ptr_arithOp_ = nullptr;
    }

    return ptr_ret;
}


nt_retType_term* Parser::parse_term() {
    nt_retType_term* ptr_ret = new nt_retType_term();

    ptr_ret->ptr_factor = parse_factor();
    ptr_ret->ptr_term_ = parse_term_();

    return ptr_ret;
}


nt_retType_factor* Parser::parse_factor() {
    nt_retType_factor* ptr_ret = new nt_retType_factor();

    Token* lookahead = lexer->getlookahead();
    std::cout << "lookahead is :" << lookahead->getTokenStr() << " type: " <<
        lookahead->getTokenTypeStr() << std::endl;

    if (lookahead->getTokenType() == tokenType::L_PAREN) {
        ptr_ret->ptr_tk_lparen = match(tokenType::L_PAREN, nullptr);
        ptr_ret->ptr_expression = parse_expression();
        ptr_ret->ptr_tk_rparen = match(tokenType::R_PAREN, nullptr);
    } else if (lookahead->getTokenType() == tokenType::MINUS) {
        ptr_ret->ptr_tk_minus = match(tokenType::MINUS, nullptr);

        lookahead = lexer->getlookahead();

        if (lookahead->getTokenType() == tokenType::IDENTIFIER) {
            ptr_ret->ptr_name = parse_name();
        } else if (lookahead->getTokenType() == tokenType::INTEGER ||
                lookahead->getTokenType() == tokenType::FLOAT) {
            ptr_ret->ptr_number = parse_number();
        }

    } else if (lookahead->getTokenType() == tokenType::INTEGER ||
            lookahead->getTokenType() == tokenType::FLOAT) {
        ptr_ret->ptr_number = parse_number();
    } else if (lookahead->getTokenType() == tokenType::STRING) {
        ptr_ret->ptr_string = parse_string();
    } else if (lookahead->getTokenType() == tokenType::TRUE_RW) {
        ptr_ret->ptr_tk_true = match(tokenType::TRUE_RW, nullptr);
    } else if (lookahead->getTokenType() == tokenType::FALSE_RW) {
        ptr_ret->ptr_tk_false = match(tokenType::FALSE_RW, nullptr);
    } else {
        /* ptr_ret->ptr_nameOrProcedure = parse_nameOrProcedure( */

        /*
         * here, factor is one of these:
         *   <procedure_call>
         *   <name>
         *
         * lookahead already points to identifier (common to both procedure_call and name)
         */

        nt_retType_identifier* ptr_identifier_nameOrProdecure = parse_identifier(nullptr);

        lookahead = lexer->getlookahead();

        // for name
        nt_retType_name* ptr_name;
        Token *tk_name_lbkt = nullptr;
        nt_retType_expression *ptr_name_expression = nullptr;
        Token *tk_name_rbkt = nullptr;

        // for procedure call
        nt_retType_procedure_call* ptr_procedurecall = nullptr;

        Token* tk_procedurecall_lparen = nullptr;
        nt_retType_argument_list* ptr_procedurecall_arglist = nullptr;
        Token* tk_procedurecall_rparen = nullptr;

        if (lookahead->getTokenType() == tokenType::L_BRACKET) {
            // handle variable[index]
            tk_name_lbkt = match(tokenType::L_BRACKET, nullptr);
            ptr_name_expression = parse_expression();
            tk_name_rbkt = match(tokenType::R_BRACKET, nullptr);

            ptr_name = new nt_retType_name();
            ptr_name->ptr_identifier = ptr_identifier_nameOrProdecure;
            ptr_name->ptr_tk_lbkt = tk_name_lbkt;
            ptr_name->ptr_expression = ptr_name_expression;
            ptr_name->ptr_tk_rbkt = tk_name_rbkt;

            ptr_ret->ptr_name = ptr_name;
        } else if (lookahead->getTokenType() == tokenType::L_PAREN) {
            // handle procedurename(arg_list)
            tk_procedurecall_lparen = match(tokenType::L_PAREN, nullptr);

            lookahead = lexer->getlookahead();

            if (lookahead->getTokenType() != tokenType::R_PAREN) {
                ptr_procedurecall_arglist = parse_argument_list();
            }

            tk_procedurecall_rparen = match(tokenType::R_PAREN, nullptr);

            ptr_procedurecall = new nt_retType_procedure_call();
            ptr_procedurecall->ptr_identifier = ptr_identifier_nameOrProdecure;
            ptr_procedurecall->ptr_tk_lparen = tk_procedurecall_lparen;
            ptr_procedurecall->ptr_argument_list = ptr_procedurecall_arglist;
            ptr_procedurecall->ptr_tk_rparen = tk_procedurecall_rparen;

            ptr_ret->ptr_procedure_call = ptr_procedurecall;
        } else {
            // handle variable
            ptr_name = new nt_retType_name();
            ptr_name->ptr_identifier = ptr_identifier_nameOrProdecure;

            ptr_ret->ptr_name = ptr_name;
        }
    }

    return ptr_ret;
}


nt_retType_relation_* Parser::parse_relation_() {

    nt_retType_relation_* ptr_ret = new nt_retType_relation_();

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::LESS_THAN) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::LESS_THAN, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->ptr_relation_ = parse_relation_();
    } else if (lookahead->getTokenType() == tokenType::GREATER_EQUAL) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::GREATER_EQUAL, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->ptr_relation_ = parse_relation_();
    } else if (lookahead->getTokenType() == tokenType::LESS_EQUAL) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::LESS_EQUAL, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->ptr_relation_ = parse_relation_();
    } else if (lookahead->getTokenType() == tokenType::GREATER_THAN) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::GREATER_THAN, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->ptr_relation_ = parse_relation_();
    } else if (lookahead->getTokenType() == tokenType::EQUALS) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::EQUALS, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->ptr_relation_ = parse_relation_();
    } else if (lookahead->getTokenType() == tokenType::NOT_EQUAL) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::NOT_EQUAL, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->ptr_relation_ = parse_relation_();
    } else {
        ptr_ret->ptr_tk_lessthan = nullptr;
        ptr_ret->ptr_term = nullptr;
        ptr_ret->ptr_relation_ = nullptr;
    }

    return ptr_ret;
}


nt_retType_term_* Parser::parse_term_() {

    nt_retType_term_* ptr_ret = new nt_retType_term_();

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::MULTIPLY) {
        ptr_ret->ptr_tk_mul = match(tokenType::MULTIPLY, nullptr);
        ptr_ret->ptr_factor = parse_factor();
        ptr_ret->ptr_term_ = parse_term_();
    } else if (lookahead->getTokenType() == tokenType::DIVIDE) {
        ptr_ret->ptr_tk_mul = match(tokenType::DIVIDE, nullptr);
        ptr_ret->ptr_factor = parse_factor();
        ptr_ret->ptr_term_ = parse_term_();
    } else {
        ptr_ret->ptr_tk_mul = nullptr;
        ptr_ret->ptr_factor = nullptr;
        ptr_ret->ptr_term_ = nullptr;
    }

    return ptr_ret;
}


nt_retType_name* Parser::parse_name() {

    nt_retType_name* ptr_ret = new nt_retType_name();

    ptr_ret->ptr_identifier = parse_identifier(nullptr);

    Token *lookahead = lexer->getlookahead();
    std::cout << "h1\n" << std::endl;
    if (lookahead->getTokenType() == tokenType::L_BRACKET) {
        ptr_ret->ptr_tk_lbkt = match(tokenType::L_BRACKET, nullptr);
        ptr_ret->ptr_expression = parse_expression();
        ptr_ret->ptr_tk_rbkt = match(tokenType::R_BRACKET, nullptr);
    }

    return ptr_ret;
}


nt_retType_string* Parser::parse_string() {

    nt_retType_string* ptr_ret;

    ptr_ret->ptr_tk_str = match(tokenType::STRING, nullptr);

    return ptr_ret;
}

