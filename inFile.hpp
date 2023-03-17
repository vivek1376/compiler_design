#ifndef __INFILE_H__
#define __INFILE_H__


#include <string>
#include <fstream>

class inFile {
    private:
        std::ifstream srcFile;
        std::string fileName;
        int lineCnt = 0;  // the line count; initialized to zero

    public:
        inFile() = delete;

        inFile(std::string fileName);

        // bool attachFile(std::string);  // open the named file

        // NOTE place fn defn here in order for it to be inlined. 
        // see https://stackoverflow.com/a/7883046/9894266
        inline char getChar() {
            return srcFile.get();
        }

        bool isgood();

        void incLineCnt() {
        }

        void ungetCh();

        bool isEOF();

        // void incLineCnt();
        // void getLineCnt();
};

#endif
