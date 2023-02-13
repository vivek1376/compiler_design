#include "lexical_analyzer.hpp"
#include <cstdio>
#include <cctype>


void scan(inFile&);
void runLexer(std::string);
bool ifStartBlockComment(inFile&);
bool ifEndBlockComment(inFile&);
void consumeLineComment(inFile&);
void processComments(inFile&);

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
    
    if (srcFile.isgood() && srcFile.getChar() != '/') {
        srcFile.ungetCh();
        return false;
    }

    if (srcFile.isgood() && srcFile.getChar() != '*') {
        srcFile.ungetCh();
        return false;
    }

    return true;
}


bool ifEndBlockComment(inFile& srcFile) {
    
    if (srcFile.isgood() && srcFile.getChar() != '*') return false;
    if (srcFile.isgood() && srcFile.getChar() != '/') return false;

    return true;
}


void consumeLineComment(inFile& srcFile) {

    char ch;

    while (srcFile.isgood()) {

        if (srcFile.isEOF()) {std::cerr << "isEOF"; return; }

        std::cerr << "h3" << std::endl;
        ch = srcFile.getChar();
        if (ch == '\n') {
            // TODO unget ?
            return;
        }
        std::cerr << "ch[" << ch << "]" << std::endl;
    }

}


void processComments(inFile& srcFile) {
    prerr("processCmnts()");
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
        prerr("processCmnt_while");
        ifInc = true;

        if (ifStartBlockComment(srcFile)) {
            prerr("start block");
            cnt++;
            ifInc = false;  // since file pos has moved, do not increment explicitly
        }

        if (ifEndBlockComment(srcFile)) {
            prerr("end block");
            cnt--;
            ifInc = false;
        }

        if (cnt == 0) return;

        if (ifInc) srcFile.getChar();
    }

    prerr("end_processCmnt_while");
}


void scan(inFile& srcFile) {

    // return one token
    std::cerr << "h1" << std::endl;

    char ch, nextCh;

    while (srcFile.isgood()) {

        std::cerr << "in while" << std::endl;
        ch = srcFile.getChar();

        /* if (ch == '\n') srcFile.incLineCnt(); */

        if (std::isspace(ch)) {
            if (ch == '\n') srcFile.incLineCnt();

            continue;
        }

        // we have a non-whitespace character

        // unget ?
        /* srcFile.ungetCh(); */

        switch (ch) {

            case '/':  // could be comment or divide
                nextCh = srcFile.getChar();
                
                if (nextCh == '/' || nextCh == '*') {
                    // unget two tumes
                    srcFile.ungetCh();
                    srcFile.ungetCh();
                    processComments(srcFile);

                    std::cerr << "h5" << std::endl;
                } else {
                    srcFile.ungetCh();  
                }

                std::cerr << "h6" << std::endl;

                break;

            default:
                /* std::cout << srcFile.getChar(); */
                /* std::cout << ch; */ 
                std::cout << "<" << ch << ">" << std::endl;
            
                
        }

    }



    std::cerr << "h7" << std::endl;

}
