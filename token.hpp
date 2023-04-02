#ifndef __TOKEN_H__
#define __TOKEN_H__


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


// TODO for single char token, use char values
// use distinct values for each enum, make sure there's no conflict with
// ascii values
enum tokenType {
    PROGRAM_RW = 1000, IS_RW = 1001, BEGIN_RW = 1002, END_RW = 1003, 
    GLOBAL_RW = 1004, PROCEDURE_RW = 1005,
    VARIABLE_RW = 1006, INTEGER_RW = 1007, FLOAT_RW = 1008, STRING_RW = 1009,
    BOOL_RW = 1010, IF_RW = 1011, THEN_RW = 1012, ELSE_RW = 1013, 
    FOR_RW = 1014, RETURN_RW = 1015, 
    NOT_RW = 1016, TRUE_RW = 1017, FALSE_RW = 1018,  // rw over
    COLON = ':', SEMICOLON = ';', L_PAREN = '(', R_PAREN = ')', COMMA = ',',
    L_BRACKET = '[', R_BRACKET = ']', UNDERSCORE = '_',
    ASSIGN_OP = 1019,  // NOTE assign is not equal operator
    AMPERSAND = '&', PLUS = '+', MINUS = '-', LESS_THAN = '<', PIPE = '|',
    GREATER_EQUAL = 1020, LESS_EQUAL = 1021,
    GREATER_THAN = 1022, EQUALS = 1023, NOT_EQUAL = 1024, 
    MULTIPLY = '*', DIVIDE = '/',
    /* QUOTE = '"',  // TODO no need for separate quote token ? */
    DOT = '.',  
    IDENTIFIER = 1025,
    INTEGER = 1026, FLOAT = 1027, STRING = 1028, EOFILE = 1029, INVALID = 1030
        // EOF? add more
};


class Token {
    private:
        tokenType ttype;
        std::string tokenStr;

        /* std::vector<std::string> ttype_str { */
        /*     "PROGRAM_RW", "IS_RW", "BEGIN_RW", "END_RW", */ 
        /*     "GLOBAL_RW", "PROCEDURE_RW", "VARIABLE_RW", "INTEGER_RW", "FLOAT_RW", "STRING_RW", */
        /*     "BOOL_RW", "IF_RW", "THEN_RW", "ELSE_RW", "FOR_RW", "RETURN_RW", "NOT_RW", */ 
        /*     "TRUE_RW", "FALSE_RW", "SEMICOLON", "L_PAREN", "R_PAREN", "COMMA", "L_BRACKET", */ 
        /*     "R_BRACKET", "UNDERSCORE", "AMPERSAND", "PLUS", "MINUS", "LESS_THAN", */ 
        /*     "GREATER_EQUAL", "LESS_EQUAL", "GREATER_THAN", "EQUALS", "NOT_EQUAL", */ 
        /*     "MULTIPLY", "DIVIDE", "DOT", "IDENTIFIER", "INTEGER", "FLOAT", */ 
        /*     "STRING", "INVALID" */
        /* }; */

        // TODO should this be outside this class?
        // this is just to print out the token string
        std::unordered_map<tokenType, std::string> map_ttypeStr {
            {tokenType::PROGRAM_RW, "PROGRAM_RW"},
                {tokenType::IS_RW, "IS_RW"},
                {tokenType::BEGIN_RW, "BEGIN_RW"},
                {tokenType::END_RW, "END_RW"},
                {tokenType::GLOBAL_RW, "GLOBAL_RW"},
                {tokenType::PROCEDURE_RW, "PROCEDURE_RW"},
                {tokenType::VARIABLE_RW, "VARIABLE_RW"},
                {tokenType::INTEGER_RW, "INTEGER_RW"},
                {tokenType::FLOAT_RW, "FLOAT_RW"},
                {tokenType::STRING_RW, "STRING_RW"},
                {tokenType::BOOL_RW, "BOOL_RW"},
                {tokenType::IF_RW, "IF_RW"},
                {tokenType::THEN_RW, "THEN_RW"},
                {tokenType::ELSE_RW, "ELSE_RW"},
                {tokenType::FOR_RW, "FOR_RW"},
                {tokenType::RETURN_RW, "RETURN_RW"},
                {tokenType::NOT_RW, "NOT_RW"},
                {tokenType::TRUE_RW, "TRUE_RW"},
                {tokenType::FALSE_RW, "FALSE_RW"},
                {tokenType::COLON, "COLON"},
                {tokenType::SEMICOLON, "SEMICOLON"},
                {tokenType::L_PAREN, "L_PAREN"},
                {tokenType::R_PAREN, "R_PAREN"},
                {tokenType::COMMA, "COMMA"},
                {tokenType::L_BRACKET, "L_BRACKET"},
                {tokenType::R_BRACKET, "R_BRACKET"},
                {tokenType::UNDERSCORE, "UNDERSCORE"},
                {tokenType::ASSIGN_OP, "ASSIGN_OP"},
                {tokenType::AMPERSAND, "AMPERSAND"},
                {tokenType::PLUS, "PLUS"},
                {tokenType::MINUS, "MINUS"},
                {tokenType::LESS_THAN, "LESS_THAN"},
                {tokenType::GREATER_EQUAL, "GREATER_EQUAL"},
                {tokenType::LESS_EQUAL, "LESS_EQUAL"},
                {tokenType::GREATER_THAN, "GREATER_THAN"},
                {tokenType::EQUALS, "EQUALS"},
                {tokenType::NOT_EQUAL, "NOT_EQUAL"},
                {tokenType::MULTIPLY, "MULTIPLY"},
                {tokenType::DIVIDE, "DIVIDE"},
                {tokenType::DOT, "DOT"},
                {tokenType::IDENTIFIER, "IDENTIFIER"},
                {tokenType::INTEGER, "INTEGER"},
                {tokenType::FLOAT, "FLOAT"},
                {tokenType::STRING, "STRING"},
                {tokenType::EOFILE, "EOFILE"},
                {tokenType::INVALID, "INVALID"}
        };

    public:

        Token() = delete;

        Token(tokenType ttype, std::string tokenStr);
        /* Token(tokenType ttype, Token* tok) { */
        /*     if (tok != NULL) { */
        /*         tok->ttype = ttype; */
        /*     } */
        /* } */

        void setTokenType(tokenType ttype);
        void printToken();
        tokenType getTokenType();
};


#endif
