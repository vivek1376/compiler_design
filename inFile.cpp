#include "inFile.hpp"



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

bool inFile::isgood() {
    // see https://stackoverflow.com/a/4533102/9894266
    return srcFile.good(); 
    /* return srcFile.good() && !this->isEOF(); */
}

int inFile::getPos() {
    return srcFile.tellg();
}

void inFile::clear() {
    srcFile.clear();
}

void inFile::setPos(int pos) {
    clear();
    srcFile.seekg(pos);
}
