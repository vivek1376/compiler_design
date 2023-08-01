#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "token.hpp"

// PROG_SYM for whole program?
enum symType {VAR_SYM, ARR_SYM, PROC_SYM, PROG_SYM, CONST_SYM, RW_SYM, OTHERS_SYM, NA_SYM};

// NA_DTYPE is for epsilon rule
enum symDatatype {INT_DTYPE, FLOAT_DTYPE, STR_DTYPE, BOOL_DTYPE, /* ARRAY_DTYPE, */ NA_DTYPE, NOT_FOUND, 
    INVALID_DTYPE};

class SymInfo {
    protected:
        // for identifier tokens


    public:

        Token *tok;
        symType symtype;
        symDatatype symdtype;


        /* Tokeninfo(Token* tok, SymInfo *syminfo); */

        // TODO explain purpose
        /* SymInfo() = default;  // TODO change ? */
        SymInfo();  // TODO change ?
        explicit SymInfo(Token*);
        explicit SymInfo(Token*, symType, symDatatype);
        Token* getToken();
        SymInfo(const SymInfo& that);
        // TODO define copy assignment too?


        virtual void print();  // TODO for dynamic_cast ?
        /* virtual ~SymInfo() noexcept;  // see https://stackoverflow.com/a/4227337/9894266 */
        /* virtual ~SymInfo() {}; */
        virtual ~SymInfo() = default;  // https://stackoverflow.com/a/62409048/9894266
};


class SymInfo_array : public SymInfo {
    public:  // TODO change to private, and add getter/setter
        int size;

        void print() override;
        explicit SymInfo_array(const SymInfo& that);
        /* SymInfo_array(const SymInfo_array& that); */
};


class SymInfo_proc : public SymInfo {

    public:

        std::vector<SymInfo*> list_param;
        int dummyval;

        /* explicit  SymInfo_proc(); */
        SymInfo_proc();
        SymInfo_proc(int);
        /* explicit SymInfo_proc(Token*, symDatatype); */
        SymInfo_proc(Token*, symDatatype);
        SymInfo_proc& operator=(const SymInfo&);
        void print() override;
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
        Token* lookupTokenString(std::string, bool*, SymInfo**, bool);

        void printTable();
        SymInfo* getSymbolInfo(std::string, symType);
        /* SymInfo* getSymbolInfo(std::string); */
        symDatatype getSymDtype(std::string);
        void printSymbolTable();
        void printSymbolTableEntry(const std::pair<std::string, SymInfo*>&);
        void addTable();
        void removeTable();
        std::vector<std::unordered_map<std::string, SymInfo*>>& getTable();
        void printAllKeys();
        /* SymbolTable(); */

};

#endif
