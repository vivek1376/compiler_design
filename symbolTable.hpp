#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "token.hpp"

class SymInfo {
    protected:
        // for identifier tokens
        enum symType {VAR, PROC, CONST, OTHERS};
        enum symDatatype {INT, FLOAT, STR, BOOL, ARRAY, NA};
};


class SymInfo_array : public SymInfo {
    public:  // TODO change to private, and add getter/setter
        int size;
};


class SymInfo_proc : public SymInfo {
};

class Tokeninfo {
    private:
        Token *tok;
        SymInfo *syminfo;

    public:
        Tokeninfo(Token* tok, SymInfo *syminfo);
};


class SymbolTable {
    private:
        std::vector<std::unordered_map<std::string, Tokeninfo*>> vec_symtab;
        /* std::unordered_map<std::string, Token*> map_symTab; */

    public:
        std::pair<int, Tokeninfo*> lookupTokenString(std::string tokenStr);

        void printTable();

        void printAllKeys();

};

#endif
