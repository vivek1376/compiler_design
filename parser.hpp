#ifndef __PARSER_H__
#define __PARSER_H__

#include "token.hpp"
#include "lexicalAnalyzer.hpp"
#include "prodRetType.hpp"

#define PARSER  Parser::getInstance()



class Parser {
    public:
        static Parser* getInstance();

        bool scanAssume(tokenType ttype);
        ProdRetType* parseNT_program();
        ProdRetType* parseNT_bound();

        // TODO constructor: set lookahead token ?
        void setLexer(LexicalAnalyzer*);
        LexicalAnalyzer* getLexer();


        // parser fns
        void parse_program();
        void parse_program_header();
        void parse_program_body();
        void parse_program_identifier();
        void parse_declaration();
        void parse_statement();
        void parse_procedure_declaration();
        void parse_variable_declaration();
        void parse_procedure_header();
        void parse_procedure_body();
        void parse_identifier();
        void parse_type_mark();
        void parse_parameter_list();
        void parse_parameter();
        void parse_bound();
        void parse_number();
        void parse_assignment_statement();
        void parse_if_statement();
        void parse_loop_statement();
        void parse_return_statement();
        void parse_procedure_call();
        void parse_argument_list();
        void parse_destination();
        void parse_expression();
        void parse_arithOp();
        void parse_expression_();
        void parse_relation();
        void parse_arithOp_();
        void parse_term();
        void parse_factor();
        void parse_relation_();
        void parse_term_();
        void parse_name();
        void parse_string();

    private:
        // TODO make the copy constructor and assignment operators private ?
        static Parser* instance_;
        Parser() = default;
        LexicalAnalyzer *lexer;
        Token* lookahead;  // how to use ?
};


class nt_retType {
    /* public: */
    /*     bool getreturnCode(); */

    public:
        bool returnCode;

};

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

        std::vector<std::pair<nt_retType_declaration*, Token*>> vec_statement_tksemicolon;

        Token* ptr_tk_end;
        Token* ptr_tk_program;
};


class nt_retType_declaration : public nt_retType {
    public:
        int whichRule;

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

        Token* ptr_tk_not;

        nt_retType_arithOp* ptr_arithOp;
        nt_retType_expression_* ptr_expression_;
        Token* ptr_tk_comma;
        nt_retType_argument_list* ptr_argument_list;
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
