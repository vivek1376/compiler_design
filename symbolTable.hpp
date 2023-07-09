#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "token.hpp"

enum symType {VAR_SYM, PROC_SYM, CONST_SYM, OTHERS_SYM};
enum symDatatype {INT_DTYPE, FLOAT_DTYPE, STR_DTYPE, BOOL_DTYPE, ARRAY_DTYPE, NA_DTYPE};

class SymInfo {
    protected:
        // for identifier tokens


    public:
        /* Tokeninfo(Token* tok, SymInfo *syminfo); */

        // TODO explain purpose
        SymInfo(Token*);
        SymInfo() = default;  // TODO change ?
        Token* getToken();

        Token *tok;
        symType symtype;
        symDatatype symdtype;
};


class SymInfo_array : public SymInfo {
    public:  // TODO change to private, and add getter/setter
        int size;
};


class SymInfo_proc : public SymInfo {
};

/* class Tokeninfo { */
/*     private: */
/*         Token *tok; */
/*         SymInfo *syminfo; */

/* }; */

class SymbolScopeInfo {
    public:
        bool inCurrentScope;
        SymInfo *syminfo;
};

class SymbolTable {
    private:
        std::vector<std::unordered_map<std::string, SymInfo*>> vec_symtab;
        /* std::unordered_map<std::string, Token*> map_symTab; */

    public:
        Token* lookupTokenString(std::string, bool*);

        void printTable();
        void addTable();
        void printAllKeys();
        /* SymbolTable(); */

};

#endif
