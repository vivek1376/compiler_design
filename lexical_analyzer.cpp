#include "lexical_analyzer.hpp"
#include <cstdio>
#include <cctype>


void scan(inFile&);
void runLexer(std::string);
bool ifStartBlockComment(inFile&);
bool ifEndBlockComment(inFile&);
void consumeLineComment(inFile&);
void processComments(inFile&);
bool isComment(inFile&);
bool isWhitespace(inFile&);
void removeWhitespace(inFile&);
Token* buildToken(inFile&, SymbolTable&);

void prerr(std::string st) {
    std::cerr << st << std::endl;
}


int main() {

    std::string filename = "testfile";

    runLexer(filename);



    return 0;
}

void runLexer(std::string filename) {

    inFile srcfile("testfile");

    // scan(srcfile);
    scan(srcfile);

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




    /* dsfsdfsdf /* sdfsdf */
    // sdfsdfsdf */ char weq;

bool ifStartBlockComment(inFile& srcFile) {

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


bool ifEndBlockComment(inFile& srcFile) {

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


void consumeLineComment(inFile& srcFile) {

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


void processComments(inFile& srcFile) {
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

void removeWhitespace(inFile& srcFile) {

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

bool isWhitespace(inFile& srcFile) {
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


void scan(inFile& srcFile) {

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
    buildToken(srcFile, symTab);
}


Token* buildToken(inFile& srcFile, SymbolTable& symTab) {

    prerr("bt");

    char ch = srcFile.getChar(), nextCh;
    /* std::cerr << "ch: " << ch; */
    /* prerr("isspace?"); */
    /* /1* std::cerr << srcFile.isgood() ; *1/ */
    /* std::cerr << std::isspace(ch) << std::endl; */

    std::string tokenStr;

    switch (ch) {
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
            return symTab.lookupTokenString(tokenStr);  // TODO no need to use new since lookup..()
                                                        // creates IDENTIFIER token by default ?

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
            std::cerr << "illegal character" << std::endl;
            // TODO try again ?
            return new Token(tokenType::INVALID, std::string(""));
    }
    /* char ch = srcFile.getChar(); */

    /* while(!std::isspace(ch)) { */
    /*     prerr("bt2"); */
    /*     std::cout << ch; */
    /*     ch = srcFile.getChar(); */
    /* } */
}


bool isComment(inFile& srcFile) {

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
