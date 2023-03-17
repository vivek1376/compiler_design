#include "symbolTable.hpp"


Token* SymbolTable::lookupTokenString(std::string tokenStr) {
    /* std::cout << "inside lookup()...\n"; */
    if (map_symTab.find(tokenStr) != map_symTab.end()) {
        /* std::cout << "found existing token..\n"; */
        return map_symTab.find(tokenStr)->second;
    }

    // TODO explain
    /* return map_symTab.insert(std::make_pair(tokenStr, */ 
    /*             new Token(tokenType::IDENTIFIER, tokenStr))).first->second; */

    /* std::cout << "inserting new token...\n"; */
    map_symTab.insert(std::make_pair(tokenStr, 
                new Token(tokenType::IDENTIFIER, tokenStr)));

    return map_symTab.find(tokenStr)->second;
}

void SymbolTable::printTable() {
    for (auto p: map_symTab) {
        p.second->printToken();
    }
}

void SymbolTable::printAllKeys() {
    std::cout << "KEYS: ";
    for (auto p: map_symTab) {
        std::cout << p.first << " ";
    }

    std::cout << std::endl;
}

