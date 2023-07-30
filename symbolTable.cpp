#include "symbolTable.hpp"


/* Tokeninfo::Tokeninfo(Token *tok, SymInfo *syminfo) */
/*     : tok{tok} */
/*     , syminfo{syminfo} {} */


/* SymInfo::SymInfo(Token* token) : tok(token) {} */


SymInfo::SymInfo() : tok{nullptr}, symtype{symType::NA_SYM}, symdtype{symDatatype::NOT_FOUND} {}


SymInfo::SymInfo(Token* token) {
    tok = new Token(*token);
}

SymInfo::SymInfo(Token* token, symType symtype, symDatatype symdtype) {
    this->tok = new Token(*token);
    this->symtype = symtype;
    this->symdtype = symdtype;
}

SymInfo::SymInfo(const SymInfo& that) {
    /* tok = new Token(); */
    std::cout << "syminfo copy constructor called" << std::endl;
    std::cout << "that.tok addr: " << std::hex << that.tok << std::endl;
    tok = new Token(*that.tok);
    std::cout << "after Token copy" << std::endl;

    /* tok = new Token(); */
    symtype = that.symtype;
    symdtype = that.symdtype;
}


Token* SymInfo::getToken() {
    return tok;
}

void SymInfo::print() {}

SymInfo::~SymInfo() noexcept{
    delete tok;  // TODO check
}


SymInfo_array::SymInfo_array(const SymInfo& that) : SymInfo(that) {
    std::cout << "Syminfo_array copy called" << std::endl;
}

SymInfo_proc::SymInfo_proc() : SymInfo() {}

SymInfo_proc::SymInfo_proc(Token* tok, symDatatype symdtype) : SymInfo(tok, symType::PROC_SYM, symdtype) {
    /* symtype = symType::PROC_SYM; */
}


SymInfo_proc& SymInfo_proc::operator=(const SymInfo& syminfo) {
    /* *tok = *syminfo.tok; */
    /* tok = new Token(syminfo.to */
    tok = new Token(*syminfo.tok);  // TODO delete to prevent memory leak?
    symtype = syminfo.symtype;
    symdtype = syminfo.symdtype;

    return *this;
}

void SymInfo_proc::print() {}


void SymbolTable::addTable() {
    // TODO change ?
    vec_symtab.push_back(std::unordered_map<std::string, SymInfo*>());
}


/* Token* SymbolTable::lookupTokenString(std::string tokenStr, SymbolScopeInfo *symscopeinfo) { */
Token* SymbolTable::lookupTokenString(std::string tokenStr, bool* ptr_inCurrentScope, SymInfo** p_syminfo) {
    // lookup in current scope, insert if not present in current scope
    // NOTE symbol table only stores identifiers

    int numScopes = vec_symtab.size();
    // int scopeDepth = 0;
    std::cerr << "numscopes: " << numScopes << std::endl;

    if (vec_symtab.back().find(tokenStr) != vec_symtab.back().end()) {
        std::cerr << "inside\n" << std::endl;

        if (ptr_inCurrentScope) {
            *ptr_inCurrentScope = true;
        }
        /* if (symscopeinfo) { */
        /*     symscopeinfo->ptr_inCurrentScope = true; */
        /*     symscopeinfo->syminfo = vec_symtab.back().find(tokenStr)->second; */
        /* } */

        if (p_syminfo) *p_syminfo = vec_symtab.back().find(tokenStr)->second;
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

    std::cout << "inserting... tokenStr: " << tokenStr << std::endl;
    vec_symtab.back().insert(std::make_pair(tokenStr, new SymInfo(
            new Token(tokenType::IDENTIFIER, tokenStr))));

                /* new Tokeninfo( */
                /*     new Token(tokenType::IDENTIFIER, tokenStr), */
                /*     new SymInfo()))); */

    std::cout << "inserted... size of symbol table: " << vec_symtab.back().size() << std::endl;

    if (ptr_inCurrentScope) {
        *ptr_inCurrentScope = false;
    }

    if (p_syminfo) *p_syminfo = vec_symtab.back().find(tokenStr)->second;

    return vec_symtab.back().find(tokenStr)->second->getToken();
}


symDatatype SymbolTable::getSymDtype(std::string tokenStr) {

    symDatatype ret_symdtype = symDatatype::NOT_FOUND;

    for (auto it = vec_symtab.rbegin(); it != vec_symtab.rend(); ++it) {
        if (it->find(tokenStr) != it->end()) {
            /* return std::make_pair(scopeDepth, it->find(tokenStr)->second); */
            return it->find(tokenStr)->second->symdtype;
        }

        /* scopeDepth++; */
    }

    return ret_symdtype;
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

