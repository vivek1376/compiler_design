#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <iostream>
#include <string>
#include <unordered_map>
#include "token.hpp"


class SymbolTable {
    private:
        std::unordered_map<std::string, Token*> symTab;

    public:
        Token* lookupTokenString(std::string tokenStr) {

            if (symTab.find(tokenStr) != symTab.end()) {
                return symTab.find(tokenStr)->second;
            }

            return symTab.insert(std::make_pair(tokenStr, 
                        new Token(tokenType::IDENTIFIER, tokenStr))).first->second;

        }

};

#endif
