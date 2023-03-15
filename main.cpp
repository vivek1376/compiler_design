#include "lexicalAnalyzer.hpp"
/* #include "symbolTable.h" */



int main() {

    std::string filename = "../project_description/testPgms/correct/source.src";
    /* std::string filename = "ghfghhfgfgh"; */

    /* LexicalAnalyzer* lexer = LexicalAnalyzer::getInstance(); */

    /* lexer->runLexer(filename); */

    /* runLexer(filename); */
    LEXER->initSymbolTable();
    LEXER->runLexer(filename);

    return 0;
}
