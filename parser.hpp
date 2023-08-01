#ifndef __PARSER_H__
#define __PARSER_H__

#include "symbolTable.hpp"
#include "lexicalAnalyzer.hpp"
#include "prodRetType.hpp"

#define PARSER  Parser::getInstance()


class nt_retType_program;
class nt_retType_program_header;
class nt_retType_program_body;
/* class nt_retType_program_identifier; */
class nt_retType_program_body;
class nt_retType_declaration;
class nt_retType_statement;
class nt_retType_procedure_declaration;
class nt_retType_variable_declaration;
class nt_retType_procedure_header;
class nt_retType_procedure_body;
class nt_retType_identifier;
class nt_retType_type_mark;
class nt_retType_parameter_list;
class nt_retType_parameter;
class nt_retType_bound;
class nt_retType_number;
class nt_retType_assignment_statement;
class nt_retType_if_statement;
class nt_retType_loop_statement;
class nt_retType_return_statement;
class nt_retType_procedure_call;
class nt_retType_argument_list;
class nt_retType_destination;
class nt_retType_expression;
class nt_retType_arithOp;
class nt_retType_expression_;
class nt_retType_relation;
class nt_retType_arithOp_;
class nt_retType_term;
class nt_retType_factor;
class nt_retType_relation_;
class nt_retType_term_;
class nt_retType_name;
class nt_retType_string;


class Parser {
    public:
        static Parser* getInstance();

        /* bool scanAssume(tokenType ttype); */
        /* ProdRetType* parseNT_program(); */
        /* ProdRetType* parseNT_bound(); */

        // TODO constructor: set lookahead token ?
        void initLexer(LexicalAnalyzer*);
        void initLogger(Reporting*);
        LexicalAnalyzer* getLexer();
        void updateSymbolTable(Token*);


        /* Token* getNextToken(); */
        Token* match(tokenType tt, bool*, SymInfo**, bool);

        // parser fns

        nt_retType_program* parse_program();
        nt_retType_program_header* parse_program_header();
        nt_retType_program_body* parse_program_body();
        /* nt_retType_program_identifier* parse_program_identifier(); */
        nt_retType_declaration* parse_declaration();
        nt_retType_statement* parse_statement();
        nt_retType_procedure_declaration* parse_procedure_declaration(bool);
        nt_retType_variable_declaration* parse_variable_declaration(bool);
        nt_retType_procedure_header* parse_procedure_header(SymInfo_proc*);
        /* nt_retType_procedure_header* parse_procedure_header(); */
        nt_retType_procedure_body* parse_procedure_body();
        nt_retType_identifier* parse_identifier(bool*, bool);
        nt_retType_type_mark* parse_type_mark();
        nt_retType_parameter_list* parse_parameter_list();
        nt_retType_parameter* parse_parameter();
        nt_retType_bound* parse_bound();
        nt_retType_number* parse_number();
        nt_retType_assignment_statement* parse_assignment_statement();
        nt_retType_if_statement* parse_if_statement();
        nt_retType_loop_statement* parse_loop_statement();
        nt_retType_return_statement* parse_return_statement();
        nt_retType_procedure_call* parse_procedure_call();
        nt_retType_argument_list* parse_argument_list();
        nt_retType_destination* parse_destination();
        nt_retType_expression* parse_expression();
        nt_retType_arithOp* parse_arithOp();
        nt_retType_expression_* parse_expression_(symDatatype);
        nt_retType_relation* parse_relation();
        nt_retType_arithOp_* parse_arithOp_(symDatatype);
        nt_retType_term* parse_term();
        nt_retType_factor* parse_factor();
        nt_retType_relation_* parse_relation_(symDatatype);
        nt_retType_term_* parse_term_(symDatatype);
        nt_retType_name* parse_name();
        nt_retType_string* parse_string();

        symDatatype verifyCompatibility(tokenType, symDatatype, symDatatype);
    private:
        // TODO make the copy constructor and assignment operators private ?
        static Parser* instance_;
        Parser() = default;
        LexicalAnalyzer *lexer;
        Reporting* logger;

        /* Token* lookahead;  // how to use ? */
};


class nt_retType {
    /* public: */
    /*     bool getreturnCode(); */

    public:
        bool returnCode;
        nt_retType();
        SymbolScopeInfo *symscopeinfo;  // TODO or syminfo ?
        SymInfo *syminfo;
};


class nt_retType_program : public nt_retType {
    public:
        nt_retType_program_header* ptr_program_header;
        nt_retType_program_body* ptr_program_body;
        Token* ptr_tk_dot;
};


class nt_retType_program_header : public nt_retType {
    public:
        Token* ptr_tk_program;
        nt_retType_identifier* ptr_identifier;
        Token* ptr_tk_is;
};


class nt_retType_program_body : public nt_retType {
    public:

        std::vector<std::pair<nt_retType_declaration*, Token*>> vec_declaration_tksemicolon;

        Token* ptr_tk_begin;

        std::vector<std::pair<nt_retType_statement*, Token*>> vec_statement_tksemicolon;

        Token* ptr_tk_end;
        Token* ptr_tk_program;
};


class nt_retType_declaration : public nt_retType {
    public:
        Token* ptr_tk_global;
        nt_retType_procedure_declaration* ptr_procedure_declaration;

        nt_retType_variable_declaration* ptr_variable_declaration;
};


class nt_retType_statement : public nt_retType {
    public:
        int whichRule;

        nt_retType_assignment_statement* ptr_assignment_statement;
        nt_retType_if_statement* ptr_if_statement;
        nt_retType_loop_statement* ptr_loop_statement;
        nt_retType_return_statement* ptr_return_statement;
};


class nt_retType_procedure_declaration : public nt_retType {
    public:
        nt_retType_procedure_header* ptr_procedure_header;
        nt_retType_procedure_body* ptr_procedure_body;
};


class nt_retType_variable_declaration : public nt_retType {
    public:
        Token* ptr_tk_variable;
        nt_retType_identifier* ptr_identifier;
        Token* ptr_tk_colon;
        nt_retType_type_mark* ptr_type_mark;
        Token* ptr_tk_lbkt;
        nt_retType_bound* ptr_bound;
        Token* ptr_tk_rbkt;
};


class nt_retType_procedure_header : public nt_retType {
    public:
        Token* ptr_tk_procedure;
        nt_retType_identifier* ptr_identifier;
        Token* ptr_tk_colon;
        nt_retType_type_mark* ptr_type_mark;
        Token* ptr_tk_lparen;
        nt_retType_parameter_list* ptr_parameter_list;
        Token* ptr_tk_rparen;
};

class nt_retType_procedure_body : public nt_retType {
    public:
        std::vector<std::pair<nt_retType_declaration*, Token*>> vec_declaration_tkcolon;

        Token* ptr_tk_begin;

        std::vector<std::pair<nt_retType_statement*, Token*>> vec_statement_tkcolon;

        Token* ptr_tk_end;
        Token* ptr_tk_procedure;
};

class nt_retType_identifier : public nt_retType {
    public:
        // TODO token, symbol table ?
        Token* ptr_tk_str;
};

class nt_retType_type_mark : public nt_retType {
    public:
        int whichRule;

        Token* ptr_tk_integer;
        Token* ptr_tk_float;
        Token* ptr_tk_string;
        Token* ptr_tk_bool;
};

class nt_retType_parameter_list : public nt_retType {
    public:
        int whichRule;

        nt_retType_parameter* ptr_parameter;
        Token* ptr_tk_comma;

        // pointer is being zero-initialized probably;
        // see https://en.cppreference.com/w/cpp/language/zero_initialization
        // https://stackoverflow.com/a/15212351/9894266
        nt_retType_parameter_list* ptr_parameter_list;
};


class nt_retType_parameter : public nt_retType {
    public:
        nt_retType_variable_declaration* ptr_variable_declaration;
};


class nt_retType_bound : public nt_retType {
    public:
        nt_retType_number* ptr_number;
};


class nt_retType_number : public nt_retType {
    public:
        Token* ptr_tk_number;
};


class nt_retType_assignment_statement : public nt_retType {
    public:
        nt_retType_destination* ptr_destination;
        Token* ptr_tk_assign;
        nt_retType_expression* ptr_expression;
};


class nt_retType_if_statement : public nt_retType {
    public:
        Token* ptr_tk_if;
        Token* ptr_tk_lparen;
        nt_retType_expression* ptr_expression;
        Token* ptr_tk_rparen;
        Token* ptr_tk_then;

        std::vector<std::pair<nt_retType_statement*, Token*>> vec_statement_semicolon;

        Token* ptr_tk_else;
        std::vector<std::pair<nt_retType_statement*, Token*>> vec_statement_semicolon_2;

        Token* ptr_tk_end;
        Token* ptr_tk_if_2;
};


class nt_retType_loop_statement : public nt_retType {
    public:
        Token* ptr_tk_loop;
        Token* ptr_tk_lparen;
        nt_retType_assignment_statement* ptr_assignment_statement;
        Token* ptr_tk_semicolon;
        nt_retType_expression* ptr_expression;
        Token* ptr_tk_rparen;

        std::vector<std::pair<nt_retType_statement*, Token*>> vec_statement_semicolon;

        Token* ptr_tk_end;
        Token* ptr_tk_for;
};


class nt_retType_return_statement : public nt_retType {
    public:
        Token* ptr_tk_return;
        nt_retType_expression* ptr_expression;
};


class nt_retType_procedure_call : public nt_retType {
    public:
        nt_retType_identifier* ptr_identifier;
        Token* ptr_tk_lparen;

        nt_retType_argument_list* ptr_argument_list;  // []

        Token* ptr_tk_rparen;
};


class nt_retType_argument_list : public nt_retType {
    public:
        int whichRule;

        nt_retType_expression* ptr_expression;
        Token* ptr_tk_comma;
        nt_retType_argument_list* ptr_argument_list;

        // Token* ptr_tk_not;

        /* nt_retType_arithOp* ptr_arithOp; */
        /* nt_retType_expression_* ptr_expression_; */
        /* Token* ptr_tk_comma; */
        /* nt_retType_argument_list* ptr_argument_list; */
};


class nt_retType_destination : public nt_retType {
    public:
        nt_retType_identifier* ptr_identifier;

        Token* ptr_tk_lbkt;
        nt_retType_expression* ptr_expression;  // []
        Token* ptr_tk_rbkt;
};


class nt_retType_expression : public nt_retType {
    public:
        Token* ptr_tk_not;  // []

        nt_retType_arithOp* ptr_arithOp;
        nt_retType_expression_* ptr_expression_;
};


class nt_retType_arithOp : public nt_retType {
    public:
        nt_retType_relation* ptr_relation;
        nt_retType_arithOp_* ptr_arithOp_;
};


class nt_retType_expression_ : public nt_retType {
    public:
        int whichRule;

        Token* ptr_tk_ampersand;
        nt_retType_arithOp* ptr_arithOp;
        nt_retType_expression_* ptr_expression_;

        Token* ptr_tk_pipe;
};


class nt_retType_relation : public nt_retType {
    public:
        nt_retType_term* ptr_term;
        nt_retType_relation_* ptr_relation_;
};


class nt_retType_arithOp_ : public nt_retType {
    public:
        int whichRule;

        Token* ptr_tk_plus;
        nt_retType_relation* ptr_relation;
        nt_retType_arithOp_* ptr_arithOp_;

        Token* ptr_tk_minus;
};


class nt_retType_term : public nt_retType {
    public:
        nt_retType_factor* ptr_factor;
        nt_retType_term_* ptr_term_;
};


class nt_retType_factor : public nt_retType {
    public:
        int whichRule;

        Token* ptr_tk_lparen;
        nt_retType_expression* ptr_expression;
        Token* ptr_tk_rparen;

        nt_retType_procedure_call* ptr_procedure_call;

        Token* ptr_tk_minus;  // []
        nt_retType_name* ptr_name;

        nt_retType_number* ptr_number;

        nt_retType_string* ptr_string;

        Token* ptr_tk_true;

        Token* ptr_tk_false;
};


class nt_retType_relation_ : public nt_retType {
    public:
        int whichRule;

        Token* ptr_tk_lessthan;
        nt_retType_term* ptr_term;
        nt_retType_relation_* ptr_relation_;

        Token* ptr_tk_gtequal;

        Token* ptr_tk_lessequal;

        Token* ptr_tk_gt;

        Token* ptr_tk_equals;

        Token* ptr_tk_noteq;
};


class nt_retType_term_ : public nt_retType {
    public:
        int whichRule;

        Token* ptr_tk_mul;
        nt_retType_factor* ptr_factor;
        nt_retType_term_* ptr_term_;

        Token* ptr_tk_divide;
};


class nt_retType_name : public nt_retType {
    public:
        nt_retType_identifier* ptr_identifier;

        Token* ptr_tk_lbkt;
        nt_retType_expression* ptr_expression;  // []
        Token* ptr_tk_rbkt;
};


class nt_retType_string : public nt_retType {
    public:
        Token* ptr_tk_str;
};


#endif
