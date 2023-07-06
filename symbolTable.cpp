#include "symbolTable.hpp"


/* Tokeninfo::Tokeninfo(Token *tok, SymInfo *syminfo) */
/*     : tok{tok} */
/*     , syminfo{syminfo} {} */


SymInfo::SymInfo(Token* token) : tok(token) {}


Token* SymInfo::getToken() {
    return tok;
}


void SymbolTable::addTable() {
    // TODO change ?
    vec_symtab.push_back(std::unordered_map<std::string, SymInfo*>());
}


Token* SymbolTable::lookupTokenString(std::string tokenStr, std::pair<bool, SymInfo*>* symInfo) {
    // lookup in current scope, insert if not present in current scope
    // -1 means new symbol in entire program so far
    // NOTE symbol table is only for identifiers

    int numScopes = vec_symtab.size();
    // int scopeDepth = 0;
    std::cerr << "numscopes: " << numScopes << std::endl;

    if (vec_symtab.back().find(tokenStr) != vec_symtab.back().end()) {
        std::cerr << "inside\n" << std::endl;

        if (symInfo) {
            symInfo->first = true;
            symInfo->second = vec_symtab.back().find(tokenStr)->second;
        }

        return vec_symtab.back().find(tokenStr)->second->getToken();
    }

    // for (auto it = vec_symtab.rbegin(); it != vec_symtab.rend(); ++it) {
        // if (it->find(tokenStr) != it->end()) {
            // return std::make_pair(scopeDepth, it->find(tokenStr)->second);
        // }

        // scopeDepth++;
    // }

    /* /1* std::cout << "inside lookup()...\n"; *1/ */
    /* if (map_symTab.find(tokenStr) != map_symTab.end()) { */
    /*     /1* std::cout << "found existing token..\n"; *1/ */
    /*     return map_symTab.find(tokenStr)->second; */
    /* } */

    // TODO explain
    /* return map_symTab.insert(std::make_pair(tokenStr, */ 
    /*             new Token(tokenType::IDENTIFIER, tokenStr))).first->second; */

    /* std::cout << "inserting new token...\n"; */
    /* map_symTab.insert(std::make_pair(tokenStr, */ 
    /*             new Token(tokenType::IDENTIFIER, tokenStr))); */

    std::cout << "inserting...\n" << std::endl;
    vec_symtab.back().insert(std::make_pair(tokenStr, new SymInfo(
                    new Token(tokenType::IDENTIFIER, tokenStr))));
                                                                        // u
                /* new Tokeninfo( */
                /*     new Token(tokenType::IDENTIFIER, tokenStr), */
                /*     new SymInfo()))); */

    std::cout << "inserted...\n" << std::endl;
    return vec_symtab.back().find(tokenStr)->second->getToken();
}


/* void SymbolTable::printTable() { */
/*     for (auto p: map_symTab) { */
/*         p.second->printToken(); */
/*     } */
/* } */

/* void SymbolTable::printAllKeys() { */
/*     std::cout << "KEYS: "; */
/*     for (auto p: map_symTab) { */
/*         std::cout << p.first << " "; */
/*     } */

/*     std::cout << std::endl; */
/* } */

