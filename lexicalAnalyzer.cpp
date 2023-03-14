#include "lexicalAnalyzer.hpp"
#include <cstdio>
#include <cctype>

LexicalAnalyzer* LexicalAnalyzer::instance_ = nullptr;


LexicalAnalyzer* LexicalAnalyzer::getInstance() {

    if (!instance_) {
        instance_ = new LexicalAnalyzer();
    }

    return instance_;

}

bool inFile::isgood() {
    // see https://stackoverflow.com/a/4533102/9894266
    return srcFile.good(); 
    /* return srcFile.good() && !this->isEOF(); */
}

/* bool inFile::isEOF() { */
/*     return srcFile.eof(); */
/* } */


bool inFile::isEOF() {
    // see https://stackoverflow.com/a/6283787/9894266

    int c = srcFile.peek();
    if (c == EOF) {
        if (srcFile.eof())
            return true;
        else
            // error ?
            // throw exception ?
            return true;
    } else {
        return false;
    }
}

inFile::inFile(std::string fileName)
    : fileName{fileName} {
        /* this.fileName = fileName; */
        srcFile.open(fileName, std::ios_base::in);
    }

void inFile::ungetCh() {
    srcFile.unget();
}

void LexicalAnalyzer::runLexer(std::string filename) {

    inFile srcfile("testfile");

    while (srcfile.isgood()) {
    // scan(srcfile);
        std::cerr << "scanning..." << std::endl;
        auto tok = scan(srcfile);
        tok->printTokenString();
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


bool LexicalAnalyzer::ifStartBlockComment(inFile& srcFile) {

    char ch = srcFile.getChar();

    if (ch != '/') {
        srcFile.ungetCh();
        return false;
    }

    ch = srcFile.getChar();

    if (ch != '*') {
        srcFile.ungetCh();
        srcFile.ungetCh();
        return false;
    }

    srcFile.ungetCh();
    srcFile.ungetCh();

    return true;
}

void LexicalAnalyzer::initSymbolTable() {
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

    for (auto p: vec_rwToktype) {
        tok = symTab.lookupTokenString(p.first);
        tok->setTokenType(p.second);
        /* tok->setTokenType(tokenType:: */
    }
}

bool LexicalAnalyzer::ifEndBlockComment(inFile& srcFile) {

    char ch = srcFile.getChar();

    if (ch != '*') {
        srcFile.ungetCh();
        return false;
    }

    ch = srcFile.getChar();

    if (ch != '/') {
        srcFile.ungetCh();
        srcFile.ungetCh();
        return false;
    }

    srcFile.ungetCh();
    srcFile.ungetCh();

    return true;
}

void LexicalAnalyzer::consumeLineComment(inFile& srcFile) {

    char ch;

    while (srcFile.isgood()) {

        // TODO comment this out ?
        /* if (srcFile.isEOF()) {std::cerr << "isEOF"; return; } */

        std::cerr << "h3" << std::endl;
        ch = srcFile.getChar();
        if (ch == '\n') {
            // TODO unget ? no, since its a whitespace char, consider it a
            // part of the line comment
            return;
        }
        std::cerr << "ch[" << ch << "]" << std::endl;
    }

}




void LexicalAnalyzer::processComments(inFile& srcFile) {
    prerr("pc_");
    // next char onwards we have line or block comments
    //
    // for line comment read until newline
    // for block comment keep a count to match nested comments

    // discard first char, since we know it is / for both block and line comment
    srcFile.getChar();

    // we need to read 2nd char to determine if its line or block comment

    // first process line comments
    if (srcFile.getChar() == '/') {
        consumeLineComment(srcFile);
        std::cerr << "h4" << std::endl;
        // the file pointer is pointing to the first char of the next line
        // after the line comment
        return;
    }

    // now we need to process block comment
    // only process matching pairs, i.e. /* */*/ or /* /* */ won't be handled

    // reset two times to get to start of comment block
    srcFile.ungetCh();
    srcFile.ungetCh();

    int cnt = 0;  // to match open/close block comment

    char ch;
    bool ifInc;  // in case we didnt find open/close block comment from current
                 // position in file, use this flag to increment file pos by 1 character

    while (srcFile.isgood()) {
        prerr("pc2");
        ch = srcFile.getChar();
        srcFile.ungetCh();
        std::cerr << "<" << ch << ">" << std::endl;

        ifInc = true;

        if (ifStartBlockComment(srcFile)) {
            srcFile.getChar();
            srcFile.getChar();
            prerr("start block");
            cnt++;
            ifInc = false;  // since file pos has moved, do not increment explicitly
        }

        if (ifEndBlockComment(srcFile)) {
            srcFile.getChar();
            srcFile.getChar();
            prerr("end block");
            cnt--;
            ifInc = false;
        }

        if (cnt == 0) break;

        if (ifInc) srcFile.getChar();  // read a char and discard
    }

    prerr("_pc");
}



void LexicalAnalyzer::removeWhitespace(inFile& srcFile) {

    char ch;

    while (srcFile.isgood()) {
        ch = srcFile.getChar();

        if (!std::isspace(ch))
            break;

        if (ch == '\n')
            srcFile.incLineCnt();
    }

    // we are pointing to a non-whitespace character; or EOF ?

    srcFile.ungetCh();

    return;
}

bool LexicalAnalyzer::isWhitespace(inFile& srcFile) {
    prerr("iw");
    std::cerr << srcFile.isgood() << std::endl;

    char ch = srcFile.getChar();
    srcFile.ungetCh();

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


Token* LexicalAnalyzer::scan(inFile& srcFile) {

    SymbolTable symTab;

    while (srcFile.isgood()) {  // TODO or just true
        prerr("sc1");

        std::cerr << "h1" << std::endl;

        if (isWhitespace(srcFile)) removeWhitespace(srcFile);

        if (isComment(srcFile)) processComments(srcFile);

        // what if multiple line comments or multiple block-comments
        // so, use while loop

        if (!isWhitespace(srcFile) && !isComment(srcFile)) break;
    }

    // TODO check isgood() ?
    return buildToken(srcFile, symTab);
}




Token* LexicalAnalyzer::buildToken(inFile& srcFile, SymbolTable& symTab) {

    prerr("bt");

    char ch = srcFile.getChar(), nextCh;
    /* std::cerr << "ch: " << ch; */
    /* prerr("isspace?"); */
    /* /1* std::cerr << srcFile.isgood() ; *1/ */
    /* std::cerr << std::isspace(ch) << std::endl; */
    std::cerr << "ch is: [" << ch << "]" << std::endl;
    std::string tokenStr;

    switch (ch) {
        // NOTE chars such as / . can be part of something else too. / can be start of a 
        // comment, which is being handled separately outside of this function
        case ';' : case '(' : case ')' : case ',' : case '[' : case ']' : case '_' :
        case '&' : case '+' : case '-' : case '*' : case '/' : case '.':
            return new Token(static_cast<tokenType>(ch), std::string{ch});
            /* ch = srcFile.getChar(); */
            /* if (ch */ 
        case '<':
            ch = srcFile.getChar();
            if (ch == '=') return new Token(tokenType::LESS_EQUAL, NULL);

            srcFile.ungetCh();
            return new Token(tokenType::LESS_THAN, NULL);

        case '0' ... '9':  // TODO check if correct syntax
            tokenStr = ch;

            ch = srcFile.getChar();
            while (ch >= '0' && ch <= '9') {
                tokenStr.push_back(ch);
                ch = srcFile.getChar();
            }

            if (ch == '.') {
                tokenStr.push_back('.');
                ch = srcFile.getChar();
                /* bool dotAppended = false;  // TODO use ? */

                while (ch >= '0' && ch <= '9') {

                    tokenStr.push_back(ch);
                    ch = srcFile.getChar();
                }

                // TODO add handling for invalid strings such as 1.

                srcFile.ungetCh();
                /* return new Token(tokenType::FLOAT, tokenStr); */
                // NOTE: not bothering with tables for now
                /* return new Token(tokenType::FLOAT, symTab.lookupTokenString(tokenStr));  // TODO ask */
                return new Token(tokenType::FLOAT, tokenStr);
            } else {
                srcFile.ungetCh();
                /* return new Token(tokenType::INTEGER, tokenStr); */
                /* return new Token(tokenType::INTEGER, symTab.lookupTokenString(tokenStr)); */
                return new Token(tokenType::INTEGER, tokenStr);
            }

        case 'a' ... 'z':

            tokenStr = ch;

            ch = srcFile.getChar();

            while ((ch >= 'a' && ch <= 'z') ||
                    (ch >= 'A' && ch <= 'Z') ||
                    (ch >= '0' && ch <= '9') ||
                    ch == '_') {
                tokenStr.push_back(ch);
                ch = srcFile.getChar();
            }

            srcFile.ungetCh();

            // identifier
            return symTab.lookupTokenString(tokenStr);  // TODO no need to use new() since 
                                                        // lookup..() creates IDENTIFIER token 
                                                        // by default ?

        case '"':
            tokenStr = "";

            ch = srcFile.getChar();

            // TODO permit " in string ?

            while (ch != '"') {
                tokenStr.push_back(ch);
                ch = srcFile.getChar();
            }

            srcFile.ungetCh();

            /* return symTab.lookupTokenString(tokenStr); */
            /* return new Token(tokenType::STRING, symTab.lookupTokenString(tokenStr)); */
            // NOTE: not bothering with tables for now
            return new Token(tokenType::STRING, tokenStr);

        default:
            std::cerr << "isgood? " << srcFile.isgood() << std::endl;
            std::cerr << "illegal character" << std::endl;

            if (srcFile.isEOF()) {
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


bool LexicalAnalyzer::isComment(inFile& srcFile) {

    char ch = srcFile.getChar();

    if (ch != '/') {
        srcFile.ungetCh();
        return false;
    }

    ch = srcFile.getChar();

    prerr("isc2");
    if (ch != '/' && ch != '*') {
        srcFile.ungetCh();
        srcFile.ungetCh();
        return false;
    }

    prerr("isc3");
    // reset to start of comment

    srcFile.ungetCh();
    srcFile.ungetCh();

    /* prerr("this_is_comment"); */
    return true;
}
