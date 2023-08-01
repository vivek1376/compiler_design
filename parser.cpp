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


void Parser::updateSymbolTable(Token* tok) {

}


Parser* Parser::getInstance() {
    if (!instance_) {
        instance_ = new Parser();
    }

    return instance_;
}


Token* Parser::match(tokenType tt, bool* inCurrentScope, SymInfo** ptr_syminfo) {
    // match token and consume (scan() will advance file pointer)
    Token *tok = lexer->scan(inCurrentScope, ptr_syminfo);

    std::cout << "💎 match() actual str:\t\t" << tok->getTokenStr() + "\t\t"
        + tok->getTokenTypeStr() + "\n";

    /* if (nextTok->getTokenType() == tk->getTokenType() && */
    /*         nextTok->getTokenStr() == tk->getTokenStr()) */
    /*     return nextTok; */

    if (tok->getTokenType() == tt) {
        return tok;
    } else {
        Token tmpTok(tt, "tmptoken");
        logger->reportError("token not matched - expected type / actual: " 
                + tmpTok.getTokenTypeStr()
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

// set true by default; upon parse/type check error, set returnCode false explicitly
nt_retType::nt_retType() : returnCode{true}, syminfo{new SymInfo()} {}


nt_retType_program* Parser::parse_program() {

    /* currTok = lexer->scan(); */
    std::cout << "parse_program()" << std::endl;

    nt_retType_program* ptr_ret = new nt_retType_program();

    ptr_ret->ptr_program_header = parse_program_header();
    ptr_ret->returnCode &= ptr_ret->ptr_program_header->returnCode;

    ptr_ret->ptr_program_body = parse_program_body();
    ptr_ret->returnCode &= ptr_ret->ptr_program_body->returnCode;

    ptr_ret->ptr_tk_dot = match(tokenType::DOT, nullptr, nullptr);

    LEXER->getSymbolTable().removeTable();

    return ptr_ret;
}


nt_retType_program_header* Parser::parse_program_header() {
    auto ptr_ret = new nt_retType_program_header();

    ptr_ret->ptr_tk_program = match(tokenType::PROGRAM_RW, nullptr, nullptr);
    // TODO add check ?

    ptr_ret->ptr_identifier = parse_identifier(nullptr);
    ptr_ret->returnCode &= ptr_ret->ptr_identifier->returnCode;

    ptr_ret->ptr_identifier->syminfo->symtype = symType::PROG_SYM;
    ptr_ret->ptr_identifier->syminfo->symdtype = symDatatype::NA_DTYPE;
                                                                 
    ptr_ret->ptr_tk_is = match(tokenType::IS_RW, nullptr, nullptr);

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
        ptr_ret->returnCode &= ptr_declaration->returnCode;

        tk_semicolon = match(tokenType::SEMICOLON, nullptr, nullptr);

        ptr_ret->vec_declaration_tksemicolon.push_back(std::make_pair(ptr_declaration,
                    tk_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_begin = match(tokenType::BEGIN_RW, nullptr, nullptr);

    lookahead = lexer->getlookahead();

    nt_retType_statement* ptr_statement;
    while (lookahead->getTokenType() != tokenType::END_RW
            && lookahead->getTokenStr() != "end") {
        ptr_statement = parse_statement();
        ptr_ret->returnCode &= ptr_statement->returnCode;

        tk_semicolon = match(tokenType::SEMICOLON, nullptr, nullptr);

        ptr_ret->vec_statement_tksemicolon.push_back(std::make_pair(ptr_statement,
                    tk_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_end = match(tokenType::END_RW, nullptr, nullptr);
    ptr_ret->ptr_tk_program = match(tokenType::PROGRAM_RW, nullptr, nullptr);

    return ptr_ret;
}


nt_retType_declaration* Parser::parse_declaration() {

    nt_retType_declaration* ptr_ret = new nt_retType_declaration();

    Token *lookahead = lexer->getlookahead();

    ptr_ret->ptr_tk_global = nullptr;
    if (lookahead->getTokenType() == tokenType::GLOBAL_RW) {
        ptr_ret->ptr_tk_global = match(tokenType::GLOBAL_RW, nullptr, nullptr);
    }

    ptr_ret->ptr_procedure_declaration = nullptr;  // TODO add this in class constructor
    ptr_ret->ptr_variable_declaration = nullptr;

    lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::PROCEDURE_RW) {
        ptr_ret->ptr_procedure_declaration = parse_procedure_declaration();
        ptr_ret->returnCode &= ptr_ret->ptr_procedure_declaration->returnCode;
    } else if (lookahead->getTokenType() == tokenType::VARIABLE_RW) {
        ptr_ret->ptr_variable_declaration = parse_variable_declaration();
        ptr_ret->returnCode &= ptr_ret->ptr_variable_declaration->returnCode;
    } else {
        ptr_ret->returnCode = false;
        logger->reportError("error in parser_declaration(), lookahead_str: "
                + lookahead->getTokenStr());
        /* throw std::runtime_error("error in parser_declaration(), lookahead_str: " */
        /*         + lookahead->getTokenStr()); */
    }

    return ptr_ret;
}


nt_retType_statement* Parser::parse_statement() {

    auto ptr_ret = new nt_retType_statement();

    Token *lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::IF_RW) {
        ptr_ret->ptr_if_statement = parse_if_statement();
        ptr_ret->returnCode &= ptr_ret->ptr_if_statement->returnCode;
    } else if (lookahead->getTokenType() == tokenType::RETURN_RW) {
        ptr_ret->ptr_return_statement = parse_return_statement();
        ptr_ret->returnCode &= ptr_ret->ptr_return_statement->returnCode;
    } else if (lookahead->getTokenType() == tokenType::FOR_RW) {
        ptr_ret->ptr_loop_statement = parse_loop_statement();
        ptr_ret->returnCode &= ptr_ret->ptr_loop_statement->returnCode;
    } else {  // assignment
        ptr_ret->ptr_assignment_statement = parse_assignment_statement();
        ptr_ret->returnCode &= ptr_ret->ptr_assignment_statement->returnCode;
    }

    return ptr_ret;
}


nt_retType_procedure_declaration* Parser::parse_procedure_declaration() {

    /* LEXER->addSymbolTable(); */

    auto ptr_ret = new nt_retType_procedure_declaration();

    // TODO create new symbol table (add to stack) here?

    ptr_ret->syminfo = new SymInfo_proc();  // procedure_decl contains both proc header and body 
                                            // so, allocate syminfo_proc class here
                                            // TODO OR, just use assign to: ptr_ret->syminfo = ...
    // since return value from parse_procedure_header's syminfo member cannot 
    ptr_ret->ptr_procedure_header = parse_procedure_header(dynamic_cast<SymInfo_proc*>(ptr_ret->syminfo));
    /* if (!ptr_ret->ptr_procedure_header->returnCode) ptr_ret->returnCode = false; */
    ptr_ret->returnCode &= ptr_ret->ptr_procedure_header->returnCode;

    ptr_ret->ptr_procedure_body = parse_procedure_body();
    if (!ptr_ret->ptr_procedure_body->returnCode) ptr_ret->returnCode = false;

    /* LEXER->getSymbolTable().getTable().pop_back(); */
    LEXER->getSymbolTable().removeTable();

    return ptr_ret;
}


nt_retType_variable_declaration* Parser::parse_variable_declaration() {

    auto ptr_ret = new nt_retType_variable_declaration();

    ptr_ret->ptr_tk_variable = match(tokenType::VARIABLE_RW, nullptr, nullptr);  // TODO add ret value check for invalid token ?
    // check
    if (ptr_ret->ptr_tk_variable->getTokenType() == tokenType::INVALID) {
        logger->reportError("VARIABLE_RW not found!");
        ptr_ret->returnCode = false;
    }

    bool inCurrentScope;
    
    ptr_ret->ptr_identifier = parse_identifier(&inCurrentScope);

    // check
    if (!ptr_ret->ptr_identifier->returnCode) {
        logger->reportError("Identifier parse error");
        ptr_ret->returnCode = false;
    }

    /* auto syminfo = new SymInfo(ptr_ret->ptr_identifier->ptr_tk_str); */
    auto syminfo = ptr_ret->ptr_identifier->syminfo;
    ptr_ret->syminfo = syminfo;

    // check duplicate
    if (inCurrentScope) {
        logger->reportError("Duplicate symbol declaration: " 
                + ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr());
        ptr_ret->returnCode = false;

        return ptr_ret;
    }

    ptr_ret->ptr_tk_colon = match(tokenType::COLON, nullptr, nullptr);

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

    // also set symtype
    syminfo->symtype = symType::VAR_SYM;

/*     ptr_ret->ptr_tk_lbkt = nullptr; */
/*     ptr_ret->ptr_tk_rbkt = nullptr; */
/*     ptr_ret->ptr_bound = nullptr; */

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::L_BRACKET) {
        /* SymInfo_array *sym_arr = static_cast<SymInfo_array*>(&syminfo); */
        /* SymInfo_array *sym_arr = new SymInfo_array(dynamic_cast<const SymInfo_array&>(syminfo)); */

        // create new syminfo_arr; we can't simply cast syminfo returned by parse_identifier
        auto sym_arr = new SymInfo_array(*syminfo);
        sym_arr->symtype = symType::ARR_SYM;

        auto arr_name = ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr();

        LEXER->getSymbolTable().getTable().back()[arr_name] = sym_arr;

        // TODO delete syminfo ?

        // print - working
        /* std::cout << "printing syminfo_arr: " << sym_arr->tok->getTokenStr() << std::endl; */

        ptr_ret->ptr_tk_lbkt = match(tokenType::L_BRACKET, nullptr, nullptr);
        ptr_ret->ptr_bound = parse_bound();

        ptr_ret->ptr_tk_rbkt = match(tokenType::R_BRACKET, nullptr, nullptr);

        int size = -1;
        try {
            size = std::stoi(ptr_ret->ptr_bound->ptr_number->ptr_tk_number->getTokenStr());
        } catch (const std::exception &e) {
            std::cout << "can't convert size" << std::endl;
            logger->reportError("can't convert size");
            ptr_ret->returnCode = false;
        }

        sym_arr->size = size;

        if (sym_arr->size <= 0) {
            logger->reportError("size should be a positive integer");
            ptr_ret->returnCode = false;
        }

        std::cout << "size is: " << std::dec << size << std::endl;

        ptr_ret->syminfo = sym_arr;
        std::cout << "symtype of array: " << ptr_ret->syminfo->symtype << std::endl;
        /* ptr_ret->syminfo = sym_arr; */
    }

    // add type checking
 
    // TODO add syminfo to symbol table
    // updateSymbolTable(ptr_ret->ptr_tk_variable);  // syminfo already created in lookup..()

    return ptr_ret;
}


nt_retType_procedure_header* Parser::parse_procedure_header(SymInfo_proc* syminfo_proc) {
/* nt_retType_procedure_header* Parser::parse_procedure_header() { */

    auto ptr_ret = new nt_retType_procedure_header();

    // TODO confusing naming

    ptr_ret->ptr_tk_procedure = match(tokenType::PROCEDURE_RW, nullptr, nullptr);

    bool inCurrentScope;
    ptr_ret->ptr_identifier = parse_identifier(&inCurrentScope);
    ptr_ret->returnCode &= ptr_ret->ptr_identifier->returnCode;

    /* if (!ptr_ret->ptr_identifier->returnCode) { */
    /*     logger->reportError("Identifier parse error in procedure header"); */
    /*     ptr_ret->returnCode = false; */
    /* } */

    /* modify syminfo for this procedure name
     *
     *
     *
     *
     */

    /* std::unordered_map<std::string, SymInfo*>& symtab_curr = LEXER->getSymbolTable().getTable().back(); */

    std::unordered_map<std::string, SymInfo*>* symtab_curr = &(LEXER->getSymbolTable().getTable().back());

    auto proc_name = ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr();

    if (symtab_curr->find(proc_name) == symtab_curr->end()){
        throw std::runtime_error("identifier not found in symbol table");
    } else {
        std::cout << "found identifier in symbol table" << std::endl;
    }

    std::cout << "SymInfo before: " << (*symtab_curr)[proc_name] << std::endl;

    (*symtab_curr)[proc_name] = syminfo_proc;

    /* std::cout << "after: " << symtab_curr[ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr()] << std::endl; */
    /* std::cout << "after using ptr: " << (*symtab_curr2)[ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr()] << std::endl; */

    // check duplicate
    if (inCurrentScope) {
        logger->reportError("Duplicate procedure name symbol declaration: "
                            + ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr());
        ptr_ret->returnCode = false;

        return ptr_ret;
    }

    *syminfo_proc = *ptr_ret->ptr_identifier->syminfo;  // TODO why not just assign pointer?
    // delete ptr_ret->ptr_identifier->syminfo;  // TODO deleting causes problems ?
                                                 // yes, token strings printed in
                                                 // "lookahead is: " and "match()" changes
                                                 // deleting will delete entry in symbol table

    syminfo_proc->symtype = symType::PROC_SYM;
    /* syminfo_proc->dummyval = 42; */

    /* std::cout << "identifier string before: " << ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr() << std::endl; */

    /* std::cout << "syminfo_proc addr: " << syminfo_proc << std::endl; */
    /* std::cout << "symbol table map syminfo addr: " << symtab_curr[ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr()] << std::endl; */

    /* SymInfo_proc *p_syminfoproc = dynamic_cast<SymInfo_proc*>(symtab_curr[ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr()]); */

    /* std::cout << "p_syminfoproc addr using dynamic_cast: " << p_syminfoproc << std::endl; */

    /* std::cout << "accessing base member using SymInfo pointer: " << symtab_curr[ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr()]->symtype << std::endl; */
    /* std::cout << "accessing base member using syminfoproc pointer: " */
    /*     << p_syminfoproc->symtype << std::endl; */
    /* std::cout << "accessing derived member using syminfoproc pointer: " */
    /*     << p_syminfoproc->dummyval << std::endl; */
    /* std::cout << "accessing another derived member using syminfoproc pointer: " */
    /*     << p_syminfoproc->list_param.size() << std::endl; */
    /* std::cout << "dummyval: " << symtab_curr[ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr()]->dummyval << std::endl; */

    ptr_ret->ptr_tk_colon = match(tokenType::COLON, nullptr, nullptr);

    ptr_ret->ptr_type_mark = parse_type_mark();
    ptr_ret->returnCode &= ptr_ret->ptr_type_mark->returnCode;

    if (ptr_ret->ptr_type_mark->ptr_tk_integer) {
        syminfo_proc->symdtype = symDatatype::INT_DTYPE;
    } else if (ptr_ret->ptr_type_mark->ptr_tk_float) {
        syminfo_proc->symdtype = symDatatype::FLOAT_DTYPE;
    } else if (ptr_ret->ptr_type_mark->ptr_tk_string) {
        syminfo_proc->symdtype = symDatatype::STR_DTYPE;
    } else if (ptr_ret->ptr_type_mark->ptr_tk_bool) {
        syminfo_proc->symdtype = symDatatype::BOOL_DTYPE;
    }

    ptr_ret->ptr_tk_lparen = match(tokenType::L_PAREN, nullptr, nullptr);

    std::cout << "symbol table map syminfo addr 2: " << (*symtab_curr)[proc_name] << std::endl;

    // now, add new symbol table to store local variables of the procedure
    LEXER->addSymbolTable();

    // since new symbol table got added, get pointer to second last table in the vector which stores the 
    // procedure symbol info
    symtab_curr = &(LEXER->getSymbolTable().getTable()[LEXER->getSymbolTable().getTable().size() - 2]);

    std::cout << "symbol table map syminfo addr 3: " 
        << (*symtab_curr)[proc_name] << std::endl;

    auto lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() != tokenType::R_PAREN) {
        ptr_ret->ptr_parameter_list = parse_parameter_list();
        ptr_ret->returnCode &= ptr_ret->ptr_parameter_list->returnCode;
        /* // redundant? */
        /* if (!ptr_ret->ptr_parameter_list->returnCode) ptr_ret->returnCode = false; */
       
        // now, build tree? for parameters?
        nt_retType_parameter_list *paramList = ptr_ret->ptr_parameter_list;

        /* std::cout << "paramList: " << paramList << std::endl; */
       
        // turn it into vector of syminfo's
        while (paramList) {
            /* ptr_ret->list_param.push_back( */
            syminfo_proc->list_param.push_back(
                    paramList->ptr_parameter->ptr_variable_declaration->syminfo);

            std::cout << "param is: " <<
                syminfo_proc->list_param.back()->getToken()->getTokenStr() << std::endl;

            /* if (!paramList->returnCode) ptr_ret->returnCode = false; */
            ptr_ret->returnCode &= paramList->returnCode;

            paramList = paramList->ptr_parameter_list;
        }
    }


    // testing dynamic_cast
    SymInfo *p_s1 = new SymInfo_proc(42);
    SymInfo_proc *p_s2 = dynamic_cast<SymInfo_proc*>(p_s1);

    std::cout << "dynamic test: " << std::endl;
    std::cout << "dynamic test: " << p_s2->dummyval << std::endl;

    /* std::cout << "after paramList: " << std::endl; */

    std::cout << "symbol table map syminfo addr after: " << (*symtab_curr)[proc_name] << std::endl;

    p_s2 = dynamic_cast<SymInfo_proc*>((*symtab_curr)[proc_name]);
    std::cout << "p_s2 addr: " << p_s2 << std::endl;
    std::cout << "after paramList: " << p_s2->list_param.size() << std::endl;

    ptr_ret->ptr_tk_rparen = match(tokenType::R_PAREN, nullptr, nullptr);

    return ptr_ret;
}


nt_retType_procedure_body* Parser::parse_procedure_body() {

    auto ptr_ret = new nt_retType_procedure_body();

    auto lookahead = lexer->getlookahead();

    /* std::cout << "in parse_proc_body: " << std::boolalpha << ptr_ret->returnCode << std::endl;  // ok: prints true */
    while (lookahead->getTokenType() != tokenType::BEGIN_RW) {

        auto p_first_decl = parse_declaration();
        ptr_ret->returnCode &= p_first_decl->returnCode;

        auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr, nullptr);

        // TODO no need to store semicolon in the vector
        ptr_ret->vec_declaration_tkcolon.push_back(std::make_pair(p_first_decl,
                    p_second_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_begin = match(tokenType::BEGIN_RW, nullptr, nullptr);

    lookahead = lexer->getlookahead();
    while (lookahead->getTokenType() != tokenType::END_RW) {

        auto p_first_st = parse_statement();
        ptr_ret->returnCode &= p_first_st->returnCode;

        auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr, nullptr);

        ptr_ret->vec_statement_tkcolon.push_back(std::make_pair(p_first_st, 
                    p_second_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_end = match(tokenType::END_RW, nullptr, nullptr);
    ptr_ret->ptr_tk_procedure = match(tokenType::PROCEDURE_RW, nullptr, nullptr);

    return ptr_ret;
}


nt_retType_identifier* Parser::parse_identifier(bool *inCurrentScope) {
    // TODO define identifier only as terminal ?

    nt_retType_identifier* ptr_ret = new nt_retType_identifier();

    // TODO delete existing syminfo member? since syminfo member will store address of SymInfo 
    // allocated by lookupTokenStr()

    // TODO allocate SymInfo here and pass to match ?
    ptr_ret->ptr_tk_str = match(tokenType::IDENTIFIER, inCurrentScope, &ptr_ret->syminfo);
    ptr_ret->returnCode = (ptr_ret->ptr_tk_str->getTokenType() != tokenType::INVALID);

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

    // redundant due to zero-initialization?
    /* ptr_ret->ptr_tk_bool = nullptr; */
    /* ptr_ret->ptr_tk_float = nullptr; */
    /* ptr_ret->ptr_tk_string = nullptr; */
    /* ptr_ret->ptr_tk_integer = nullptr; */

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::INTEGER_RW)
        ptr_ret->ptr_tk_integer = match(tokenType::INTEGER_RW, nullptr, nullptr);
    else if (lookahead->getTokenType() == tokenType::FLOAT_RW)
        ptr_ret->ptr_tk_float = match(tokenType::FLOAT_RW, nullptr, nullptr);
    else if (lookahead->getTokenType() == tokenType::STRING_RW)
        ptr_ret->ptr_tk_string = match(tokenType::STRING_RW, nullptr, nullptr);
    else if (lookahead->getTokenType() == tokenType::BOOL_RW)
        ptr_ret->ptr_tk_bool = match(tokenType::BOOL_RW, nullptr, nullptr);

    // TODO throw

    return ptr_ret;
}


nt_retType_parameter_list* Parser::parse_parameter_list() {

    auto ptr_ret = new nt_retType_parameter_list();

    // both poiner values print zero; 
    // this explains? >> https://en.cppreference.com/w/cpp/language/zero_initialization
    /* std::cout << "in parse_parameter_list(): class member pointer values" << std::endl; */
    /* std::cout << "token: " << ptr_ret->ptr_tk_comma << std::endl; */
    /* std::cout << "parameter_list: " << ptr_ret->ptr_parameter_list << std::endl; */

    ptr_ret->ptr_parameter = parse_parameter();
    ptr_ret->returnCode &= ptr_ret->ptr_parameter->returnCode;

    auto lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::COMMA) {
        ptr_ret->ptr_tk_comma = match(tokenType::COMMA, nullptr, nullptr);

        ptr_ret->ptr_parameter_list = parse_parameter_list();
        ptr_ret->returnCode &= ptr_ret->ptr_parameter_list->returnCode;
    }

    return ptr_ret;
}


nt_retType_parameter* Parser::parse_parameter() {

    auto ptr_ret = new nt_retType_parameter();

    ptr_ret->ptr_variable_declaration = parse_variable_declaration();
    ptr_ret->returnCode &= ptr_ret->ptr_variable_declaration->returnCode;

    return ptr_ret;
}


nt_retType_bound* Parser::parse_bound() {

    auto ptr_ret = new nt_retType_bound();

    ptr_ret->ptr_number = parse_number();
    ptr_ret->returnCode &= ptr_ret->ptr_number->returnCode;

    ptr_ret->syminfo = ptr_ret->ptr_number->syminfo;

    if (ptr_ret->syminfo->symdtype != symDatatype::INT_DTYPE) {
        ptr_ret->returnCode = false;
        logger->reportError("Bound value must be integer type");
    }

    return ptr_ret;
}


nt_retType_number* Parser::parse_number() {

    std::cerr << "parsing no." << std::endl;
    std::cerr << std::endl;

    auto ptr_ret = new nt_retType_number();

    auto lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::INTEGER) {
        ptr_ret->ptr_tk_number = match(tokenType::INTEGER, nullptr, nullptr);
        ptr_ret->syminfo->symdtype = symDatatype::INT_DTYPE;
    } else if (lookahead->getTokenType() == tokenType::FLOAT) {
        ptr_ret->ptr_tk_number = match(tokenType::FLOAT, nullptr, nullptr);
        ptr_ret->syminfo->symdtype = symDatatype::FLOAT_DTYPE;
    } else {
        ptr_ret->returnCode = false;
        std::cout << "lookahead type: " << lookahead->getTokenTypeStr() << std::endl;
        logger->reportError("can't parse number, actual string: " + lookahead->getTokenStr());

        /* ptr_ret->ptr_tk_number = lexer->scan(nullptr, nullptr); */  // TODO why added this?
    }

    return ptr_ret;
}


nt_retType_assignment_statement* Parser::parse_assignment_statement() {

    std::cout << "parse_assignment_statement():\n";
    std::cout << std::endl;

    auto ptr_ret = new nt_retType_assignment_statement();

    ptr_ret->ptr_destination = parse_destination();
    ptr_ret->returnCode &= ptr_ret->ptr_destination->returnCode;

    ptr_ret->ptr_tk_assign = match(tokenType::ASSIGN_OP, nullptr, nullptr);

    ptr_ret->ptr_expression = parse_expression();
    ptr_ret->returnCode &= ptr_ret->ptr_expression->returnCode;

    ptr_ret->syminfo->symdtype = verifyCompatibility(tokenType::ASSIGN_OP, 
            ptr_ret->ptr_destination->syminfo->symdtype, 
            ptr_ret->ptr_expression->syminfo->symdtype);

    ptr_ret->returnCode &= (ptr_ret->syminfo->symdtype != symDatatype::INVALID_DTYPE);
    if (ptr_ret->syminfo->symdtype == symDatatype::INVALID_DTYPE) {
        logger->reportError(std::string("Assignment type mismatch for destination: " 
                    + ptr_ret->ptr_destination->ptr_identifier->ptr_tk_str->getTokenStr()));
    }

    return ptr_ret;
}


nt_retType_if_statement* Parser::parse_if_statement() {

    // TODO ignoring few match() call returns
    nt_retType_if_statement* ptr_ret = new nt_retType_if_statement();

    ptr_ret->ptr_tk_if = match(tokenType::IF_RW, nullptr, nullptr);
    match(tokenType::L_PAREN, nullptr, nullptr);  // NOTE ignoring return pointer
                                                 
    ptr_ret->ptr_expression = parse_expression();
    ptr_ret->returnCode &= ptr_ret->ptr_expression->returnCode;

    std::cout << "IF expr type boolean: " << std::boolalpha << (ptr_ret->ptr_expression->syminfo->symdtype == symDatatype::BOOL_DTYPE) << std::endl;
    /* std::cout << ptr_ret->ptr_expression->syminfo->symdtype << std::endl; */

    if ((ptr_ret->ptr_expression->syminfo->symdtype != symDatatype::BOOL_DTYPE) &&
            (ptr_ret->ptr_expression->syminfo->symdtype != symDatatype::INT_DTYPE)) {
        ptr_ret->returnCode = false;
        logger->reportError("IF expression must evaluate to boolean");
        /* ptr_ret->syminfo->symdtype = symDatatype::INVALID_DTYPE; */
    }

    match(tokenType::R_PAREN, nullptr, nullptr);
    ptr_ret->ptr_tk_then = match(tokenType::THEN_RW, nullptr, nullptr);

    Token* lookahead = lexer->getlookahead();

    while ((lookahead->getTokenType() != tokenType::ELSE_RW) &&
            (lookahead->getTokenType() != tokenType::END_RW)) {
        auto p_first_st = parse_statement();
        ptr_ret->returnCode &= p_first_st->returnCode;

        auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr, nullptr);

        ptr_ret->vec_statement_semicolon.push_back(std::make_pair(p_first_st,
                    p_second_semicolon));

        lookahead = lexer->getlookahead();
    }

    lookahead = lexer->getlookahead();  // TODO redundant due to previous getlookahead() call ?
    if (lookahead->getTokenType() == tokenType::ELSE_RW) {

        ptr_ret->ptr_tk_else = match(tokenType::ELSE_RW, nullptr, nullptr);

        lookahead = lexer->getlookahead();

        while (lookahead->getTokenType() != tokenType::END_RW) {

            auto p_first_st = parse_statement();
            ptr_ret->returnCode &= p_first_st->returnCode;

            auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr, nullptr);

            ptr_ret->vec_statement_semicolon_2.push_back(std::make_pair(p_first_st,
                        p_second_semicolon));

            lookahead = lexer->getlookahead();
        }
    }

    ptr_ret->ptr_tk_end = match(tokenType::END_RW, nullptr, nullptr);
    ptr_ret->ptr_tk_if_2 = match(tokenType::IF_RW, nullptr, nullptr);

    return ptr_ret;
}


nt_retType_loop_statement* Parser::parse_loop_statement() {

    auto ptr_ret = new nt_retType_loop_statement();

    ptr_ret->ptr_tk_for = match(tokenType::FOR_RW, nullptr, nullptr);
    ptr_ret->ptr_tk_lparen = match(tokenType::L_PAREN, nullptr, nullptr);

    ptr_ret->ptr_assignment_statement = parse_assignment_statement();
    ptr_ret->returnCode &= ptr_ret->ptr_assignment_statement->returnCode;

    ptr_ret->ptr_tk_semicolon = match(tokenType::SEMICOLON, nullptr, nullptr);

    ptr_ret->ptr_expression = parse_expression();
    ptr_ret->returnCode &= ptr_ret->ptr_expression->returnCode;

    if ((ptr_ret->ptr_expression->syminfo->symdtype != symDatatype::BOOL_DTYPE) &&
            (ptr_ret->ptr_expression->syminfo->symdtype != symDatatype::INT_DTYPE)) {
        ptr_ret->returnCode = false;
        logger->reportWarning("conditional expression must evaluate to boolean type");
    }

    ptr_ret->ptr_tk_rparen = match(tokenType::R_PAREN, nullptr, nullptr);

    auto lookahead = lexer->getlookahead();

    while (lookahead->getTokenType() != tokenType::END_RW) {
        auto p_first_st = parse_statement();
        ptr_ret->returnCode &= p_first_st->returnCode;

        auto p_second_semicolon = match(tokenType::SEMICOLON, nullptr, nullptr);

        ptr_ret->vec_statement_semicolon.push_back(std::make_pair(p_first_st,
                    p_second_semicolon));

        lookahead = lexer->getlookahead();
    }

    ptr_ret->ptr_tk_end = match(tokenType::END_RW, nullptr, nullptr);
    ptr_ret->ptr_tk_for = match(tokenType::FOR_RW, nullptr, nullptr);

    return ptr_ret;
}


nt_retType_return_statement* Parser::parse_return_statement() {

    prerr("parse_return_statement()");
    auto ptr_ret = new nt_retType_return_statement();

    ptr_ret->ptr_tk_return = match(tokenType::RETURN_RW, nullptr, nullptr);

    ptr_ret->ptr_expression = parse_expression();
    ptr_ret->returnCode &= ptr_ret->ptr_expression->returnCode;

    return ptr_ret;
}


nt_retType_procedure_call* Parser::parse_procedure_call() {
    // NOTE handled in factor parse fn
}


nt_retType_argument_list* Parser::parse_argument_list() {
    // TODO complete
    auto ptr_ret = new nt_retType_argument_list();

    ptr_ret->ptr_expression = parse_expression();

    auto lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::COMMA) {
        ptr_ret->ptr_tk_comma = match(tokenType::COMMA, nullptr, nullptr);
        ptr_ret->ptr_argument_list = parse_argument_list();
    }

    return ptr_ret;
}


nt_retType_destination* Parser::parse_destination() {

    auto ptr_ret = new nt_retType_destination();
    /* ptr_ret->syminfo = new SymInfo(); */

    ptr_ret->ptr_identifier = parse_identifier(nullptr);
    ptr_ret->returnCode &= ptr_ret->ptr_identifier->returnCode;

    // since parse_identifier may return syminfo with NOT_FOUND even if variable is
    // declared in an outer scope
    ptr_ret->syminfo->symdtype = LEXER->getSymbolTable().getSymDtype(
            ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr()); 

    /* ptr_ret->syminfo->symdtype = ptr_ret->ptr_identifier->syminfo->symdtype; */

    Token* lookahead = lexer->getlookahead();

    // TODO delete whichRUle from this class
    if (lookahead->getTokenType() == tokenType::L_BRACKET) {
        ptr_ret->ptr_tk_lbkt = match(tokenType::L_BRACKET, nullptr, nullptr);

        ptr_ret->ptr_expression = parse_expression();
        ptr_ret->returnCode &= ptr_ret->ptr_expression->returnCode;

        if (ptr_ret->ptr_expression->syminfo->symdtype != symDatatype::INT_DTYPE) {
            logger->reportError("Array index must be integer type");
            ptr_ret->returnCode = false;
        }

        ptr_ret->ptr_tk_rbkt = match(tokenType::R_BRACKET, nullptr, nullptr);
    }

    return ptr_ret;
}


nt_retType_expression* Parser::parse_expression() {
 
    // DONE type check

    prerr("parse_expression()");
    auto ptr_ret = new nt_retType_expression();
    /* ptr_ret->syminfo = new SymInfo(); */

    /* std::cout << "new syminfo symdtype: " << ptr_ret->syminfo->symdtype << std::endl; */

    Token* lookahead = lexer->getlookahead();

    /* std::string op_not = ""; */
    /* bool isNOTpresent = false; */

    if (lookahead->getTokenType() == tokenType::NOT_RW) {
        ptr_ret->ptr_tk_not = match(tokenType::NOT_RW, nullptr, nullptr);

        /* op_not = "not"; */
        /* isNOTpresent = true; */
        /* ptr_ret->syminfo->symdtype = symDatatype::NA_DTYPE; */
    }

    ptr_ret->ptr_arithOp = parse_arithOp();
    ptr_ret->returnCode &= ptr_ret->ptr_arithOp->returnCode;

    ptr_ret->ptr_expression_ = parse_expression_(ptr_ret->ptr_arithOp->syminfo->symdtype);
    ptr_ret->returnCode &= ptr_ret->ptr_expression_->returnCode;
 
    ptr_ret->syminfo->symdtype = ptr_ret->ptr_expression_->syminfo->symdtype;

    // if NOT is present, they type must be int or bool
    if (ptr_ret->ptr_tk_not) {
        if ((ptr_ret->syminfo->symdtype != symDatatype::BOOL_DTYPE) 
                && (ptr_ret->syminfo->symdtype != symDatatype::INT_DTYPE)) {
            ptr_ret->syminfo->symdtype = symDatatype::INVALID_DTYPE;
            ptr_ret->returnCode = false;
        }
    }
            /*         ptr_ret->syminfo->symdtype = verifyCompatibility(tokenType::NOT_RW, */ 
            /*                 verifyCompatibility(tokenType::NOT_RW, */ 
            /*                     ptr_ret->ptr_arithOp->syminfo->symdtype, */
            /*                     symDatatype::NA_DTYPE), */
            /*                 ptr_ret->); */

        /* ptr_ret->returnCode = (ptr_ret->syminfo->symdtype != symDatatype::INVALID_DTYPE); */
        /* if ((ptr_ret->ptr_expression_->syminfo->symdtype == symDatatype::INT_DTYPE) || */
        /*         (ptr_ret->ptr_expression_->syminfo->symdtype == symDatatype::BOOL_DTYPE)) { */

        /*     ptr_ret->syminfo->symdtype = ptr_ret->ptr_expression_->syminfo->symdtype; */
        /* } else { */
        /*     ptr_ret->syminfo->symdtype = symDatatype::INVALID_DTYPE; */
        /* } */
    /* } else { */
        /* ptr_ret->syminfo->symdtype = ptr_ret->ptr_expression_->syminfo->symdtype; */
    /* } */

    prerr("done parse_expression");

    /* ptr_ret->syminfo->symdtype = verifyCompatibility(op_not, */
    /*         ptr_ret->ptr_arithOp->syminfo->symdtype, */
    /*         ptr_ret->ptr_expression_->syminfo->symdtype); */

    /* if (ptr_ret->syminfo->symdtype == symDatatype::INVALID_DTYPE) {  // TODO redundant? */
    /*     ptr_ret->returnCode = false; */
    /*     // TODO report error */
    /* } */

    return ptr_ret;
}


nt_retType_arithOp* Parser::parse_arithOp() {

    // DONE type check

    nt_retType_arithOp* ptr_ret = new nt_retType_arithOp();
    /* ptr_ret->syminfo = new SymInfo(); */

    ptr_ret->ptr_relation = parse_relation();
    ptr_ret->returnCode &= ptr_ret->ptr_relation->returnCode;
    /* ptr_ret->syminfo->symdtype = ptr_ret->ptr_relation->syminfo->symdtype; */

    ptr_ret->ptr_arithOp_ = parse_arithOp_(ptr_ret->ptr_relation->syminfo->symdtype);
    ptr_ret->returnCode &= ptr_ret->ptr_arithOp_->returnCode;

    ptr_ret->syminfo->symdtype = ptr_ret->ptr_arithOp_->syminfo->symdtype;

    prerr("done parse_arithOp");
    return ptr_ret;
}


nt_retType_expression_* Parser::parse_expression_(symDatatype symdtype_left) {

    // DONE type check

    nt_retType_expression_* ptr_ret = new nt_retType_expression_();
    /* ptr_ret->syminfo = new SymInfo(); */

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::AMPERSAND) {
        ptr_ret->ptr_tk_ampersand = match(tokenType::AMPERSAND, nullptr, nullptr);

        ptr_ret->ptr_arithOp = parse_arithOp();
        ptr_ret->returnCode &= ptr_ret->ptr_arithOp->returnCode;

        ptr_ret->ptr_expression_ = parse_expression_(verifyCompatibility(tokenType::AMPERSAND, 
                    symdtype_left,
                    ptr_ret->ptr_arithOp->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_expression_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_expression_->syminfo->symdtype;
    } else if (lookahead->getTokenType() == tokenType::PIPE) {
        ptr_ret->ptr_tk_pipe = match(tokenType::PIPE, nullptr, nullptr);
        ptr_ret->ptr_arithOp = parse_arithOp();
        ptr_ret->returnCode &= ptr_ret->ptr_arithOp->returnCode;

        ptr_ret->ptr_expression_ = parse_expression_(verifyCompatibility(tokenType::PIPE, 
                    symdtype_left,
                    ptr_ret->ptr_arithOp->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_expression_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_expression_->syminfo->symdtype;
    } else {
        /* ptr_ret->ptr_tk_pipe = nullptr; */
        /* ptr_ret->ptr_arithOp = nullptr; */
        /* ptr_ret->ptr_expression_ = nullptr; */

        ptr_ret->syminfo->symdtype = symdtype_left;
    }

    return ptr_ret;
}


nt_retType_relation* Parser::parse_relation() {

    // DONE type check
    nt_retType_relation* ptr_ret = new nt_retType_relation();
    /* ptr_ret->syminfo = new SymInfo(); */

    ptr_ret->ptr_term = parse_term();
    ptr_ret->returnCode &= ptr_ret->ptr_term->returnCode;

    ptr_ret->ptr_relation_ = parse_relation_(ptr_ret->ptr_term->syminfo->symdtype);
    ptr_ret->returnCode &= ptr_ret->ptr_relation_->syminfo->symdtype;

    ptr_ret->syminfo->symdtype = ptr_ret->ptr_relation_->syminfo->symdtype;

    prerr("done parse_relation");

    return ptr_ret;
}


nt_retType_arithOp_* Parser::parse_arithOp_(symDatatype symdtype_left) {

    // DONE type check

    auto ptr_ret = new nt_retType_arithOp_();
    /* ptr_ret->syminfo = new SymInfo(); */

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::PLUS) {
        ptr_ret->ptr_tk_plus = match(tokenType::PLUS, nullptr, nullptr);
        ptr_ret->ptr_relation = parse_relation();
        ptr_ret->returnCode &= ptr_ret->ptr_relation->returnCode;

        ptr_ret->ptr_arithOp_ = parse_arithOp_(verifyCompatibility(tokenType::PLUS, 
                    symdtype_left,
                    ptr_ret->ptr_relation->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_arithOp_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_arithOp_->syminfo->symdtype;
    } else if (lookahead->getTokenType() == tokenType::MINUS) {
        ptr_ret->ptr_tk_plus = match(tokenType::MINUS, nullptr, nullptr);
        ptr_ret->ptr_relation = parse_relation();
        ptr_ret->returnCode &= ptr_ret->ptr_relation->returnCode;

        ptr_ret->ptr_arithOp_ = parse_arithOp_(verifyCompatibility(tokenType::MINUS,
                    symdtype_left,
                    ptr_ret->ptr_relation->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_arithOp_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_arithOp_->syminfo->symdtype;
    } else {
        /* ptr_ret->ptr_tk_plus = nullptr; */
        /* ptr_ret->ptr_relation = nullptr; */
        /* ptr_ret->ptr_arithOp_ = nullptr; */

        ptr_ret->syminfo->symdtype = symdtype_left;
    }

    return ptr_ret;
}


nt_retType_term* Parser::parse_term() {

    // DONE type check

    nt_retType_term* ptr_ret = new nt_retType_term();
    /* ptr_ret->syminfo = new SymInfo(); */

    ptr_ret->ptr_factor = parse_factor();
    ptr_ret->returnCode &= ptr_ret->ptr_factor->returnCode;

    prerr("done parse_factor()");
    ptr_ret->ptr_term_ = parse_term_(ptr_ret->ptr_factor->syminfo->symdtype);
    ptr_ret->returnCode &= ptr_ret->ptr_term_->returnCode;

    ptr_ret->syminfo->symdtype = ptr_ret->ptr_term_->syminfo->symdtype;

    prerr("done parse_term_()");

    return ptr_ret;
}


nt_retType_factor* Parser::parse_factor() {

    // TODO type check incomplete

    prerr("parse_factor()");
    nt_retType_factor* ptr_ret = new nt_retType_factor();
    /* ptr_ret->syminfo = new SymInfo(); */

    Token* lookahead = lexer->getlookahead();
    /* std::cout << "🔔 lookahead is :" << lookahead->getTokenStr() << "\ttype: " << */
    /*     lookahead->getTokenTypeStr() << std::endl; */

    if (lookahead->getTokenType() == tokenType::L_PAREN) {  // handle (expression)
        ptr_ret->ptr_tk_lparen = match(tokenType::L_PAREN, nullptr, nullptr);
        ptr_ret->ptr_expression = parse_expression();
        
        ptr_ret->returnCode &= ptr_ret->ptr_expression->returnCode;

        /* std::cout << "in parse_factor(): syminfo: " << ptr_ret->syminfo << std::endl; */
        prerr("post parse_expression()");
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_expression->syminfo->symdtype;
        prerr("post assigning symdtype");
        ptr_ret->ptr_tk_rparen = match(tokenType::R_PAREN, nullptr, nullptr);
    } else if (lookahead->getTokenType() == tokenType::MINUS) {
        ptr_ret->ptr_tk_minus = match(tokenType::MINUS, nullptr, nullptr);

        lookahead = lexer->getlookahead();

        if (lookahead->getTokenType() == tokenType::IDENTIFIER) {  // handle -name
            ptr_ret->ptr_name = parse_name();
            ptr_ret->returnCode &= ptr_ret->ptr_name->returnCode;
            ptr_ret->syminfo->symdtype = ptr_ret->ptr_name->syminfo->symdtype;
        } else if (lookahead->getTokenType() == tokenType::INTEGER) {  // handle -int
            ptr_ret->ptr_number = parse_number();
            ptr_ret->returnCode &= ptr_ret->ptr_number->returnCode;
            ptr_ret->syminfo->symdtype = symDatatype::INT_DTYPE;
        } else if (lookahead->getTokenType() == tokenType::FLOAT) {  // handle -float
            ptr_ret->ptr_number = parse_number();
            ptr_ret->returnCode &= ptr_ret->ptr_number->returnCode;
            ptr_ret->syminfo->symdtype = symDatatype::FLOAT_DTYPE;
        }
    } else if (lookahead->getTokenType() == tokenType::INTEGER) {  // handle int
        prerr("handling int");
        ptr_ret->ptr_number = parse_number();
        ptr_ret->returnCode &= ptr_ret->ptr_number->returnCode;
        ptr_ret->syminfo->symdtype = symDatatype::INT_DTYPE;
        prerr("handled int");
    } else if (lookahead->getTokenType() == tokenType::FLOAT) {  // handle float
        ptr_ret->ptr_number = parse_number();
        ptr_ret->returnCode &= ptr_ret->ptr_number->returnCode;
        ptr_ret->syminfo->symdtype = symDatatype::FLOAT_DTYPE;
    } else if (lookahead->getTokenType() == tokenType::STRING) {  // handle string
        ptr_ret->ptr_string = parse_string();
        ptr_ret->returnCode &= ptr_ret->ptr_string->returnCode;
        ptr_ret->syminfo->symdtype = symDatatype::STR_DTYPE;
    } else if (lookahead->getTokenType() == tokenType::TRUE_RW) {  // handle true
        ptr_ret->ptr_tk_true = match(tokenType::TRUE_RW, nullptr, nullptr);
        ptr_ret->returnCode &= (ptr_ret->ptr_tk_true->getTokenType() != tokenType::INVALID);
        ptr_ret->syminfo->symdtype = symDatatype::BOOL_DTYPE;
    } else if (lookahead->getTokenType() == tokenType::FALSE_RW) {  // handle false
        ptr_ret->ptr_tk_false = match(tokenType::FALSE_RW, nullptr, nullptr);
        ptr_ret->returnCode &= (ptr_ret->ptr_tk_true->getTokenType() != tokenType::INVALID);
        ptr_ret->syminfo->symdtype = symDatatype::BOOL_DTYPE;
    } else {
        /* ptr_ret->ptr_nameOrProcedure = parse_nameOrProcedure( */

        /*
         * here, factor is one of these:
         *   <procedure_call>
         *   <name> : <identifier> [[<expression>]]
         *
         * lookahead already points to identifier (common to both procedure_call and name)
         */

        nt_retType_identifier* ptr_identifier_nameOrProdecure = parse_identifier(nullptr);
        ptr_ret->returnCode &= ptr_identifier_nameOrProdecure->returnCode;

        lookahead = lexer->getlookahead();

        // for name
        nt_retType_name* ptr_name;
        Token *tk_name_lbkt = nullptr;
        nt_retType_expression *ptr_name_expression = nullptr;
        Token *tk_name_rbkt = nullptr;

        // for procedure call
        nt_retType_procedure_call* ptr_procedure_call = nullptr;
        Token* tk_procedurecall_lparen = nullptr;
        nt_retType_argument_list* ptr_procedurecall_arglist = nullptr;
        Token* tk_procedurecall_rparen = nullptr;

        if (lookahead->getTokenType() == tokenType::L_BRACKET) {
            
            // handle [expression] for <name> : <identifier> [[<expression>]]
            tk_name_lbkt = match(tokenType::L_BRACKET, nullptr, nullptr);

            ptr_name_expression = parse_expression();  // should this evaluate to number?
            ptr_ret->returnCode &= ptr_name_expression->returnCode;

            tk_name_rbkt = match(tokenType::R_BRACKET, nullptr, nullptr);

            ptr_name = new nt_retType_name();
            ptr_name->ptr_identifier = ptr_identifier_nameOrProdecure;
            ptr_name->ptr_tk_lbkt = tk_name_lbkt;
            ptr_name->ptr_expression = ptr_name_expression;
            ptr_name->ptr_tk_rbkt = tk_name_rbkt;

            ptr_ret->ptr_name = ptr_name;

            // verify that expression is int
            if (ptr_ret->ptr_name->ptr_expression->syminfo->symdtype != symDatatype::INT_DTYPE) {
                logger->reportError("array index is not INT type");
            }

            // now, get the symdtype of the identifier from the symbol table
            ptr_ret->syminfo->symdtype = LEXER->getSymbolTable().getSymDtype(
                    ptr_name->ptr_identifier->ptr_tk_str->getTokenStr());
        } else if (lookahead->getTokenType() == tokenType::L_PAREN) {

            // handle (arg_list) in <procedure_call>
            tk_procedurecall_lparen = match(tokenType::L_PAREN, nullptr, nullptr);  // handle (

            lookahead = lexer->getlookahead();

            if (lookahead->getTokenType() != tokenType::R_PAREN) {
                ptr_procedurecall_arglist = parse_argument_list();
                ptr_ret->returnCode &= ptr_procedurecall_arglist->returnCode;
            }

            tk_procedurecall_rparen = match(tokenType::R_PAREN, nullptr, nullptr);  // handle )

            ptr_procedure_call = new nt_retType_procedure_call();
            ptr_procedure_call->ptr_identifier = ptr_identifier_nameOrProdecure;
            ptr_procedure_call->ptr_tk_lparen = tk_procedurecall_lparen;
            ptr_procedure_call->ptr_argument_list = ptr_procedurecall_arglist;
            ptr_procedure_call->ptr_tk_rparen = tk_procedurecall_rparen;

            ptr_ret->ptr_procedure_call = ptr_procedure_call;

            // TODO arglist should match
            // arglist from procedure call, not procedure declaration

            std::string proc_name = 
                ptr_ret->ptr_procedure_call->ptr_identifier->ptr_tk_str->getTokenStr();

            /* SymInfo* procedure_decl_SymInfo = LEXER->getSymbolTable().getSymbolInfo(proc_name, */ 
            /*         symType::PROC_SYM); */
            /* std::cout << "proc SymInfo retreived: " << procedure_decl_SymInfo << std::endl; */
            // get symbol info for this procedure name
            SymInfo* procedureName_SymInfo = LEXER->getSymbolTable().getSymbolInfo(proc_name, 
                    symType::PROC_SYM);
            SymInfo_proc* procedureName_SymInfo_proc = dynamic_cast<SymInfo_proc*>(
                    procedureName_SymInfo);
                        /* symType::PROC_SYM)); */

            std::cout << "procedureName_SymInfo: " << procedureName_SymInfo << std::endl;
            std::cout << "procedureName_SymInfo_proc: " << procedureName_SymInfo_proc << std::endl;

            std::cout << "proc_name: " << proc_name << std::endl;

            if (procedureName_SymInfo_proc == nullptr) {
                    /* || (procedure_decl_SymInfo_proc->symtype != symType::PROC_SYM)) { */
                logger->reportError(std::string("Procedure '" + proc_name 
                            + "' not declared"));
                /* LEXER->getSymbolTable().getTable().back()[proc_name]->symtype = symType::PROC_SYM; */
                /* procedureName_SymInfo->symtype = symType::PROC_SYM; */
            } else {

                // TODO add this in declaration
                /* procedureName_SymInfo_proc->symtype = symType::PROC_SYM; */

                /* if ((procedureName_SymInfo_proc->symdtype == symDatatype::NOT_FOUND) */
                if (procedureName_SymInfo_proc->symtype != symType::PROC_SYM) {
                    /* throw std::runtime_error("Procedure name not found in symbol table" ); */
                    logger->reportError(std::string("Procedure '" + proc_name 
                                + "' not declared"));
                }

                // build a vector of SymInfo* from argList
                std::vector<SymInfo*> vec_procedure_call_argList_SymInfo;

                nt_retType_argument_list *argList = ptr_ret->ptr_procedure_call->ptr_argument_list;

                while (argList) {
                    vec_procedure_call_argList_SymInfo.push_back(
                            argList->ptr_expression->syminfo);

                    ptr_ret->returnCode &= argList->returnCode;

                    argList = argList->ptr_argument_list;
                }

                if (procedureName_SymInfo_proc) {
                    if (vec_procedure_call_argList_SymInfo.size() != 
                            procedureName_SymInfo_proc->list_param.size()) {
                        logger->reportError("Procedure call argument list length mismatch");
                    } else {
                        for (int i = 0; i < vec_procedure_call_argList_SymInfo.size(); i++) {
                            if (vec_procedure_call_argList_SymInfo[i]->symdtype !=
                                    procedureName_SymInfo_proc->list_param[i]->symdtype) {
                                logger->reportError("Procedure call arg type mismatch");
                                ptr_ret->returnCode = false;
                                break;
                            }
                        }
                    }
                }

                // now, get the symdtype of the identifier from the symbol table
                ptr_ret->syminfo->symdtype = LEXER->getSymbolTable().getSymDtype(proc_name);
            }
        } else {
            // handle <identifier>
            ptr_name = new nt_retType_name();
            ptr_name->ptr_identifier = ptr_identifier_nameOrProdecure;

            ptr_ret->ptr_name = ptr_name;

            /* symDatatype symd = LEXER->getSymbolTable().getSymDtype( */
            /*         ptr_name->ptr_identifier->ptr_tk_str->getTokenStr()); */
            
            /* symDatatype symdd = ptr_name->syminfo->symdtype; */
            /* symDatatype symddd = symDatatype::NOT_FOUND ; */

            /* std::cout << "ptr_ret->syminfo: " << ptr_ret->syminfo << std::endl; */
            std::cout << "variable tokenString: " 
                << ptr_name->ptr_identifier->ptr_tk_str->getTokenStr() 
                << std::endl;

            // now, get the symdtype of the identifier from the symbol table
            ptr_ret->syminfo->symdtype = LEXER->getSymbolTable().getSymDtype(
                    ptr_name->ptr_identifier->ptr_tk_str->getTokenStr());

            std::cout << "found symdtype\n";
        }
    }

    return ptr_ret;
}


nt_retType_relation_* Parser::parse_relation_(symDatatype symdtype_left) {

    // DONE type check

    nt_retType_relation_* ptr_ret = new nt_retType_relation_();
    /* ptr_ret->syminfo = new SymInfo(); */

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::LESS_THAN) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::LESS_THAN, nullptr, nullptr);

        ptr_ret->ptr_term = parse_term();
        ptr_ret->returnCode &= ptr_ret->ptr_term->returnCode;

        ptr_ret->ptr_relation_ = parse_relation_(verifyCompatibility(tokenType::LESS_THAN, 
                    symdtype_left,
                    ptr_ret->ptr_term->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_relation_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_relation_->syminfo->symdtype;
    } else if (lookahead->getTokenType() == tokenType::GREATER_EQUAL) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::GREATER_EQUAL, nullptr, nullptr);

        ptr_ret->ptr_term = parse_term();
        ptr_ret->returnCode &= ptr_ret->ptr_term->returnCode;

        ptr_ret->ptr_relation_ = parse_relation_(verifyCompatibility(tokenType::GREATER_EQUAL,
                    symdtype_left,
                    ptr_ret->ptr_term->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_relation_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_relation_->syminfo->symdtype;
    } else if (lookahead->getTokenType() == tokenType::LESS_EQUAL) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::LESS_EQUAL, nullptr, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->returnCode &= ptr_ret->ptr_term->returnCode;

        ptr_ret->ptr_relation_ = parse_relation_(verifyCompatibility(tokenType::LESS_EQUAL, 
                    symdtype_left,
                    ptr_ret->ptr_term->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_relation_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_relation_->syminfo->symdtype;
    } else if (lookahead->getTokenType() == tokenType::GREATER_THAN) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::GREATER_THAN, nullptr, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->returnCode &= ptr_ret->ptr_term->returnCode;

        ptr_ret->ptr_relation_ = parse_relation_(verifyCompatibility(tokenType::GREATER_THAN,
                    symdtype_left,
                    ptr_ret->ptr_term->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_relation_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_relation_->syminfo->symdtype;
    } else if (lookahead->getTokenType() == tokenType::EQUALS) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::EQUALS, nullptr, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->returnCode &= ptr_ret->ptr_term->returnCode;

        ptr_ret->ptr_relation_ = parse_relation_(verifyCompatibility(tokenType::EQUALS,
                    symdtype_left,
                    ptr_ret->ptr_term->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_relation_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_relation_->syminfo->symdtype;
    } else if (lookahead->getTokenType() == tokenType::NOT_EQUAL) {
        ptr_ret->ptr_tk_lessthan = match(tokenType::NOT_EQUAL, nullptr, nullptr);
        ptr_ret->ptr_term = parse_term();
        ptr_ret->returnCode &= ptr_ret->ptr_term->returnCode;

        ptr_ret->ptr_relation_ = parse_relation_(verifyCompatibility(tokenType::NOT_EQUAL,
                    symdtype_left,
                    ptr_ret->ptr_term->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_relation_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_relation_->syminfo->symdtype;
    } else {
        /* ptr_ret->ptr_tk_lessthan = nullptr; */
        /* ptr_ret->ptr_term = nullptr; */
        /* ptr_ret->ptr_relation_ = nullptr; */

        ptr_ret->syminfo->symdtype = symdtype_left;
    }

    return ptr_ret;
}


nt_retType_term_* Parser::parse_term_(symDatatype symdtype_left) {

    // DONE type check

    nt_retType_term_* ptr_ret = new nt_retType_term_();
    /* ptr_ret->syminfo = new SymInfo(); */

    Token* lookahead = lexer->getlookahead();

    if (lookahead->getTokenType() == tokenType::MULTIPLY) {
        ptr_ret->ptr_tk_mul = match(tokenType::MULTIPLY, nullptr, nullptr);
        ptr_ret->ptr_factor = parse_factor();
        ptr_ret->returnCode &= ptr_ret->ptr_factor->returnCode;

        ptr_ret->ptr_term_ = parse_term_(verifyCompatibility(tokenType::MULTIPLY, 
                    symdtype_left,
                    ptr_ret->ptr_factor->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_term_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_term_->syminfo->symdtype;
    } else if (lookahead->getTokenType() == tokenType::DIVIDE) {
        ptr_ret->ptr_tk_mul = match(tokenType::DIVIDE, nullptr, nullptr);
        ptr_ret->ptr_factor = parse_factor();
        ptr_ret->returnCode &= ptr_ret->ptr_factor->returnCode;

        ptr_ret->ptr_term_ = parse_term_(verifyCompatibility(tokenType::DIVIDE, 
                    symdtype_left,
                    ptr_ret->ptr_factor->syminfo->symdtype));
        ptr_ret->returnCode &= ptr_ret->ptr_term_->returnCode;
        ptr_ret->syminfo->symdtype = ptr_ret->ptr_term_->syminfo->symdtype;
    } else {
        /* ptr_ret->ptr_tk_mul = nullptr; */
        /* ptr_ret->ptr_factor = nullptr; */
        /* ptr_ret->ptr_term_ = nullptr; */

        ptr_ret->syminfo->symdtype = symdtype_left;
    }

    return ptr_ret;
}


nt_retType_name* Parser::parse_name() {

    nt_retType_name* ptr_ret = new nt_retType_name();

    ptr_ret->ptr_identifier = parse_identifier(nullptr);
    ptr_ret->returnCode &= ptr_ret->ptr_identifier->returnCode;

    ptr_ret->syminfo->symdtype = LEXER->getSymbolTable().getSymDtype(
            ptr_ret->ptr_identifier->ptr_tk_str->getTokenStr()); 

    Token *lookahead = lexer->getlookahead();
    /* std::cout << "h1\n" << std::endl; */
    if (lookahead->getTokenType() == tokenType::L_BRACKET) {
        ptr_ret->ptr_tk_lbkt = match(tokenType::L_BRACKET, nullptr, nullptr);

        ptr_ret->ptr_expression = parse_expression();
        ptr_ret->returnCode &= ptr_ret->ptr_expression->returnCode;

        if (ptr_ret->ptr_expression->syminfo->symdtype != symDatatype::INT_DTYPE) {
            logger->reportError("<expression should evaluate to integer in parse_name");
        }

        ptr_ret->ptr_tk_rbkt = match(tokenType::R_BRACKET, nullptr, nullptr);
    }

    return ptr_ret;
}


nt_retType_string* Parser::parse_string() {

    nt_retType_string* ptr_ret = new nt_retType_string();

    ptr_ret->ptr_tk_str = match(tokenType::STRING, nullptr, nullptr);

    return ptr_ret;
}


symDatatype Parser::verifyCompatibility(tokenType ttype, symDatatype symdtype_left, 
        symDatatype symdtype_right) {

    // TODO check

    std::cout << "verifyCompatibility, types: " << symdtype_left << " " << symdtype_right << std::endl;
    // make sure there are no NOT_FOUND's
    if ((symdtype_left == symDatatype::NOT_FOUND) ||
            (symdtype_right == symDatatype::NOT_FOUND)) {
        return symDatatype::NOT_FOUND;
        /* throw std::runtime_error("NOT_FOUND symDatatype"); */
    }

    if ((symdtype_left == symDatatype::INVALID_DTYPE) ||
            (symdtype_right == symDatatype::INVALID_DTYPE)) {
        return symDatatype::INVALID_DTYPE;
    }

    if (symdtype_left == symDatatype::NA_DTYPE) {  // TODO check 
        return symdtype_right;
    }

    if (symdtype_right == symDatatype::NA_DTYPE) {  // TODO check
        return symdtype_left;
    }

    // TODO check if correct
    if ((ttype == tokenType::AMPERSAND) || (ttype == tokenType::PIPE)) {
        if ((symdtype_left == symDatatype::INT_DTYPE) &&
                (symdtype_right == symDatatype::INT_DTYPE)) {
            return symDatatype::INT_DTYPE;
        } else if ((symdtype_left == symDatatype::BOOL_DTYPE) &&
                (symdtype_right == symDatatype::BOOL_DTYPE)) {
            return symDatatype::BOOL_DTYPE;
        } else {
            return symDatatype::INVALID_DTYPE;
        }
    }

    if ((ttype == tokenType::LESS_THAN)
            || (ttype == tokenType::GREATER_EQUAL) 
            || (ttype == tokenType::LESS_EQUAL)
            || (ttype == tokenType::GREATER_THAN) 
            || (ttype == tokenType::EQUALS)
            || (ttype == tokenType::NOT_EQUAL)) {
        if (((symdtype_left == symDatatype::INT_DTYPE)
                    || (symdtype_left == symDatatype::FLOAT_DTYPE)
                    || (symdtype_left == symDatatype::BOOL_DTYPE)) &&
                ((symdtype_right == symDatatype::INT_DTYPE) 
                 || (symdtype_right == symDatatype::FLOAT_DTYPE)
                 || (symdtype_right == symDatatype::BOOL_DTYPE))) {
            return symDatatype::BOOL_DTYPE;
        } else {
            return symDatatype::INVALID_DTYPE;
        }
    }

    // TODO what if right is epsilon?
    if ((ttype == tokenType::PLUS) || (ttype == tokenType::MINUS)) {
        if ((symdtype_left == symDatatype::INT_DTYPE) &&
                (symdtype_right == symDatatype::INT_DTYPE)) {
            return symDatatype::INT_DTYPE;
        } else if ((symdtype_left == symDatatype::BOOL_DTYPE) &&
                (symdtype_right == symDatatype::BOOL_DTYPE)) {
            return symDatatype::INT_DTYPE;
        } else if ((symdtype_left == symDatatype::FLOAT_DTYPE) &&
                (symdtype_right == symDatatype::FLOAT_DTYPE)) {
            return symDatatype::FLOAT_DTYPE;
        } else if (((symdtype_left == symDatatype::FLOAT_DTYPE)
                    && (symdtype_right == symDatatype::INT_DTYPE))
                || ((symdtype_left == symDatatype::INT_DTYPE)
                    && (symdtype_right == symDatatype::FLOAT_DTYPE))) {
            return symDatatype::FLOAT_DTYPE;
        } else {
            return symDatatype::INVALID_DTYPE;
        }

    }

    if ((ttype == tokenType::MULTIPLY) || (ttype == tokenType::DIVIDE)) {
        if ((symdtype_left == symDatatype::FLOAT_DTYPE) &&
                (symdtype_right == symDatatype::FLOAT_DTYPE)) {
            return symDatatype::FLOAT_DTYPE;
        } else if ((symdtype_left == symDatatype::INT_DTYPE) &&
                (symdtype_right == symDatatype::INT_DTYPE)) {
            return symDatatype::FLOAT_DTYPE;
        } else if (((symdtype_left == symDatatype::FLOAT_DTYPE)
                    && (symdtype_right == symDatatype::INT_DTYPE))
                || ((symdtype_left == symDatatype::INT_DTYPE)
                    && (symdtype_right == symDatatype::FLOAT_DTYPE))) {
            return symDatatype::FLOAT_DTYPE;
        } else {
            return symDatatype::INVALID_DTYPE;
        }
    }

    if (ttype == tokenType::NOT_RW) {
        if (((symdtype_left == symDatatype::BOOL_DTYPE) 
                    || (symdtype_left == symDatatype::NA_DTYPE)) &&
                ((symdtype_right == symDatatype::BOOL_DTYPE) 
                 || (symdtype_right == symDatatype::INT_DTYPE))) {
            return symDatatype::BOOL_DTYPE;
        } else {
            return symDatatype::INVALID_DTYPE;
        }
    }

    if (ttype == tokenType::ASSIGN_OP) {
        if (symdtype_left == symdtype_right) return symdtype_left;

        if ((symdtype_left == symDatatype::BOOL_DTYPE) 
                && (symdtype_right == symDatatype::INT_DTYPE)) {
            return symdtype_left;
        }

        if ((symdtype_left == symDatatype::INT_DTYPE) 
                && (symdtype_right == symDatatype::BOOL_DTYPE)) {
            return symdtype_left;
        }

        if ((symdtype_left == symDatatype::INT_DTYPE) 
                && (symdtype_right == symDatatype::FLOAT_DTYPE)) {
            return symdtype_left;
        }

        if ((symdtype_left == symDatatype::FLOAT_DTYPE) 
                && (symdtype_right == symDatatype::INT_DTYPE)) {
            return symdtype_left;
        }

        return symDatatype::INVALID_DTYPE;
    }

    if (ttype == tokenType::UNKNOWN) {
        if (symdtype_left == symdtype_right) {
            return symdtype_left;
        } else {
            return symDatatype::INVALID_DTYPE;
        }
    }

    return symDatatype::INVALID_DTYPE;
}

