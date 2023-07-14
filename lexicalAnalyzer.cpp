#include "lexicalAnalyzer.hpp"

LexicalAnalyzer* LexicalAnalyzer::instance_ = nullptr;


LexicalAnalyzer* LexicalAnalyzer::getInstance() {

    if (!instance_) {
        instance_ = new LexicalAnalyzer();
    }

    return instance_;

}


/* bool inFile::isEOF() { */
/*     return srcFile.eof(); */
/* } */


void LexicalAnalyzer::runLexer() {

    while (srcFile->isgood()) {
        std::cerr << "scanning..." << std::endl;
        auto tok = scan(nullptr, nullptr);
        tok->printToken();
    }

    std::cerr << "h8" << std::endl;

    /*    std::ifstream infile(filename); */

    /*    char nextCh; */

    /*    while (infile.good()) { */
    /*       readCharacterFromStream(infile, nextCh); */

    /*       std::cout << nextCh; */
    /*       // std::cout << nextCh << std::endl; */
    /*    } */

}
/* inline void readCharacterFromStream(std::ifstream& infile, char &ch) { */

/*    infile.get(ch); */
/* } */


bool LexicalAnalyzer::ifStartBlockComment() {

    char ch = srcFile->getChar();

    if (ch != '/') {
        srcFile->ungetCh();
        return false;
    }

    ch = srcFile->getChar();

    if (ch != '*') {
        srcFile->ungetCh();
        srcFile->ungetCh();
        return false;
    }

    srcFile->ungetCh();
    srcFile->ungetCh();

    return true;
}

void LexicalAnalyzer::initSymbolTable() {

    // TODO seeding as suggested in slides, why ??
    symTab.addTable();
    Token *tok;

    std::vector<std::pair<std::string, tokenType>> vec_rwToktype {
        {"program", tokenType::PROGRAM_RW},
        {"is", tokenType::IS_RW},
        {"begin", tokenType::BEGIN_RW},
        {"end", tokenType::END_RW},
        {"global", tokenType::GLOBAL_RW},
        {"procedure", tokenType::PROCEDURE_RW},
        {"variable", tokenType::VARIABLE_RW},
        {"integer", tokenType::INTEGER_RW},
        {"float", tokenType::FLOAT_RW},
        {"string", tokenType::STRING_RW},
        {"bool", tokenType::BOOL_RW},
        {"if", tokenType::IF_RW},
        {"then", tokenType::THEN_RW},
        {"else", tokenType::ELSE_RW},
        {"for", tokenType::FOR_RW},
        {"return", tokenType::RETURN_RW},
        {"not", tokenType::NOT_RW},
        {"true", tokenType::TRUE_RW},
        {"false", tokenType::FALSE_RW}
    };
    std::cout << "IS1" << std::endl;
    for (auto p: vec_rwToktype) {
        std::cout << "calling lookupTokenString() for RWs" << std::endl;
        tok = symTab.lookupTokenString(p.first, nullptr);
        std::cout << "IS2" << std::endl;
        tok->setTokenType(p.second);

        std::cout << "IS3" << std::endl;
        /* tok->printTokenString(); */
        /* tok->setTokenType(tokenType:: */
    }

    /* std::cout << "done init..\n"; */
}

bool LexicalAnalyzer::ifEndBlockComment() {

    char ch = srcFile->getChar();

    if (ch != '*') {
        srcFile->ungetCh();
        return false;
    }

    ch = srcFile->getChar();

    if (ch != '/') {
        srcFile->ungetCh();
        srcFile->ungetCh();
        return false;
    }

    srcFile->ungetCh();
    srcFile->ungetCh();

    return true;
}

void LexicalAnalyzer::consumeLineComment() {

    char ch;

    while (srcFile->isgood()) {

        // TODO comment this out ?
        /* if (srcFile.isEOF()) {std::cerr << "isEOF"; return; } */

        std::cerr << "h3" << std::endl;
        ch = srcFile->getChar();
        if (ch == '\n') {
            // TODO unget ? no, since its a whitespace char, consider it a
            // part of the line comment
            return;
        }
        std::cerr << "ch[" << ch << "]" << std::endl;
    }

}




void LexicalAnalyzer::processComments() {
    prerr("pc_");
    // next char onwards we have line or block comments
    //
    // for line comment read until newline
    // for block comment keep a count to match nested comments

    // eat up first char; we know it is / for both block and line comment
    srcFile->getChar();

    // we need to read 2nd char to determine if it is line or block comment

    // first process line comments
    if (srcFile->getChar() == '/') {
        consumeLineComment();
        std::cerr << "h4" << std::endl;
        // the file pointer is pointing to the first char of the next line
        // after the line comment
        return;
    }

    // now we need to process block comment
    // only process matching pairs, i.e. /* */*/ or /* /* */ won't be handled

    // reset two times to get to start of comment block
    srcFile->ungetCh();
    srcFile->ungetCh();

    int cnt = 0;  // to match open/close block comment

    char ch;
    bool ifInc;  // in case we didnt find open/close block comment from current
                 // position in file, use this flag to increment file pos by 1 character

    while (srcFile->isgood()) {
        prerr("pc2");
        ch = srcFile->getChar();
        srcFile->ungetCh();
        std::cerr << "<" << ch << ">" << std::endl;

        ifInc = true;

        if (ifStartBlockComment()) {
            srcFile->getChar();
            srcFile->getChar();
            prerr("start block");
            cnt++;
            ifInc = false;  // since file pos has moved, do not increment explicitly
        }

        if (ifEndBlockComment()) {
            srcFile->getChar();
            srcFile->getChar();
            prerr("end block");
            cnt--;
            ifInc = false;
        }

        if (cnt == 0) break;

        if (ifInc) srcFile->getChar();  // read a char and discard
    }

    prerr("_pc");
}



void LexicalAnalyzer::removeWhitespace() {

    char ch;

    while (srcFile->isgood()) {
        ch = srcFile->getChar();

        if (!std::isspace(ch))
            break;

        if (ch == '\n')
            srcFile->incLineCnt();
    }

    // we are pointing to a non-whitespace character; or EOF ?

    srcFile->ungetCh();

    return;
}

bool LexicalAnalyzer::isWhitespace() {
    prerr("iw");
    std::cerr << srcFile->isgood() << std::endl;

    char ch = srcFile->getChar();
    srcFile->ungetCh();

    std::cerr << "[" << ch << "]" << std::endl;

    // TODO just return directly
    if (std::isspace(ch)) {
        prerr("yes_space");
        return true;
    } else {
        prerr("no_space");
        return false;
    }
}


Token* LexicalAnalyzer::scan(bool *inCurrentScope, SymInfo** ptr_syminfo) {

    /* SymbolTable symTab; */

    /* std::cout << "filepos: " << srcFile->getPos() << std::endl; */

    while (srcFile->isgood()) {  // TODO or just true
        prerr("sc1");

        std::cerr << "h1" << std::endl;

        if (isWhitespace()) removeWhitespace();

        if (isComment()) processComments();

        // what if multiple line comments or multiple block-comments
        // so, use while loop

        if (!isWhitespace() && !isComment()) break;
    }

    // TODO check isgood() ?
    return buildToken(inCurrentScope, ptr_syminfo);
}




SymbolTable& LexicalAnalyzer::getSymbolTable() {
    return symTab;
}

/* Token* LexicalAnalyzer::buildToken(SymbolScopeInfo* symscopeinfo) { */
Token* LexicalAnalyzer::buildToken(bool* ptr_inCurrentScope, SymInfo** ptr_syminfo) {

    prerr("bt");

    char ch = srcFile->getChar(), nextCh;
    /* std::cerr << "ch: " << ch; */
    /* prerr("isspace?"); */
    /* /1* std::cerr << srcFile.isgood() ; *1/ */
    /* std::cerr << std::isspace(ch) << std::endl; */
    std::string tokenStr;

    /* std::cout << "\nch is: [" << ch << "]" << std::endl; */

    std::cout << "\n";
    Token *tok;
    switch (ch) {
        // NOTE chars such as / . can be part of something else too. / can be start of a 
        // comment, which is being handled separately outside of this function
        case ';' : case '(' : case ')' : case ',' : case '[' : case ']' : case '_' :
        case '&' : case '+' : case '-' : case '*' : case '/' : case '.':
            /* std::cout << "1st case\n"; */
            // see https://stackoverflow.com/a/18222927/9894266
            return new Token(static_cast<tokenType>(ch), std::string{ch});
            /* ch = srcFile.getChar(); */
            /* if (ch */ 
        case ':':
            ch = srcFile->getChar();

            /* if (ch == ' ' || ch == '\t') */
            /*     return new Token(tokenType::COLON, ":"); */
            if (ch == '=') 
                return new Token(tokenType::ASSIGN_OP, ":=");

            srcFile->ungetCh();
            return new Token(tokenType::COLON, ":");
            /* srcFile.ungetCh(); */
            /* return new Token(tokenType::INVALID, std::string{ch}); */ 

        case '<':
            ch = srcFile->getChar();
            if (ch == '=') return new Token(tokenType::LESS_EQUAL, "<=");

            srcFile->ungetCh();
            return new Token(tokenType::LESS_THAN, "<");

        case '>':
            ch = srcFile->getChar();
            if (ch == '=') return new Token(tokenType::GREATER_EQUAL, ">=");

            srcFile->ungetCh();
            return new Token(tokenType::GREATER_THAN, ">");

        case '=':
            ch = srcFile->getChar();

            if (ch == '=') return new Token(tokenType::EQUALS, "==");

            srcFile->ungetCh();
            return new Token(tokenType::INVALID, std::string{ch});

        case '0' ... '9':  // TODO check if correct syntax
            tokenStr = ch;

            ch = srcFile->getChar();
            while (ch >= '0' && ch <= '9') {
                tokenStr.push_back(ch);
                ch = srcFile->getChar();
            }

            if (ch == '.') {
                tokenStr.push_back('.');
                ch = srcFile->getChar();
                /* bool dotAppended = false;  // TODO use ? */

                while (ch >= '0' && ch <= '9') {

                    tokenStr.push_back(ch);
                    ch = srcFile->getChar();
                }

                // TODO add handling for invalid strings such as 1.

                srcFile->ungetCh();
                /* return new Token(tokenType::FLOAT, tokenStr); */
                // NOTE: not bothering with tables for now
                /* return new Token(tokenType::FLOAT, symTab.lookupTokenString(tokenStr));  // TODO ask */
                return new Token(tokenType::FLOAT, tokenStr);
            } else {
                srcFile->ungetCh();
                /* return new Token(tokenType::INTEGER, tokenStr); */
                /* return new Token(tokenType::INTEGER, symTab.lookupTokenString(tokenStr)); */
                return new Token(tokenType::INTEGER, tokenStr);
            }
        
        case 'A' ... 'Z':
            ch = std::tolower(ch);
            /* ch = 3; */
            // and, fall through, since identifiers, rw are case-insensitive
        case 'a' ... 'z':
            std::cerr << "6th case\n";
            tokenStr = ch;

            ch = std::tolower(srcFile->getChar());

            while ((ch >= 'a' && ch <= 'z') ||
                    /* (ch >= 'A' && ch <= 'Z') || */
                    (ch >= '0' && ch <= '9') ||
                    ch == '_') {
                tokenStr.push_back(ch);
                ch = std::tolower(srcFile->getChar());
            }

            srcFile->ungetCh();

            // identifier
            /* std::cout << "going to lookup..\n"; */
            /* std::cout << "buildToken() - calling lookupTokenStr" << std::endl; */
            return symTab.lookupTokenString(tokenStr, ptr_inCurrentScope);
                                                        // lookup..() creates IDENTIFIER token 
                                                        // by default ?
            /* tok->setTokenType(tokenType::PROGRAM_RW); */
            /* std::cout << "token string is: " << tokenStr << std::endl; */
            /* symTab.lookupTokenString("program")->printToken(); */
            /* symTab.printAllKeys(); */
            /* tok->printTokenString(); */

        case '"':
            tokenStr = "";

            ch = srcFile->getChar();

            // TODO permit " in string ?
            // unmatched opening quote can lead to infinite loop, so check if reached EOF etc.
            while (ch != '"' && srcFile->isgood()) {
                tokenStr.push_back(ch);
                ch = srcFile->getChar();
            }

            if (ch == '"') {
                return new Token(tokenType::STRING, tokenStr);
            } else {
                srcFile->ungetCh();
                return new Token(tokenType::INVALID, tokenStr);
            }

            /* return symTab.lookupTokenString(tokenStr); */
            /* return new Token(tokenType::STRING, symTab.lookupTokenString(tokenStr)); */
            // NOTE: not bothering with tables for now

        default:
            std::cerr << "isgood? " << srcFile->isgood() << std::endl;
            std::cerr << "illegal character" << std::endl;

            if (srcFile->isEOF()) {
                return new Token(tokenType::EOFILE, std::string{ch});
            }
            // TODO try again ?
            return new Token(tokenType::INVALID, std::string{ch});
    }
    /* char ch = srcFile.getChar(); */

    /* while(!std::isspace(ch)) { */
    /*     prerr("bt2"); */
    /*     std::cout << ch; */
    /*     ch = srcFile.getChar(); */
    /* } */
}


bool LexicalAnalyzer::isComment() {

    char ch = srcFile->getChar();

    if (ch != '/') {
        srcFile->ungetCh();
        return false;
    }

    ch = srcFile->getChar();

    prerr("isc2");
    if (ch != '/' && ch != '*') {
        srcFile->ungetCh();
        srcFile->ungetCh();
        return false;
    }

    prerr("isc3");
    // reset to start of comment

    srcFile->ungetCh();
    srcFile->ungetCh();

    /* prerr("this_is_comment"); */
    return true;
}

void LexicalAnalyzer::setinFile(inFile* infile) {
    this->srcFile = infile;
}


void LexicalAnalyzer::setinFilepos(int pos) {
    srcFile->setPos(pos);
}

int LexicalAnalyzer::getPos() {
    return srcFile->getPos();
}

Token* LexicalAnalyzer::getlookahead() {
    int pos = getPos();

    Token *tk = scan(nullptr, nullptr);

    // resetting ?
    setinFilepos(pos);

    return tk;

}
