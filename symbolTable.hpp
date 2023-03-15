#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <iostream>
#include <string>
#include <unordered_map>
#include "token.hpp"


class SymbolTable {
    private:
        std::unordered_map<std::string, Token*> map_symTab;

    public:
        Token* lookupTokenString(std::string tokenStr);

        void printTable();

        void printAllKeys();

};

#endif
