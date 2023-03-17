#include "lexicalAnalyzer.hpp"
/* #include "symbolTable.h" */



int main() {

    std::string filename = "../project_description/testPgms/correct/iterativeFib.src";
    /* std::string filename = "ghfghhfgfgh"; */

    /* LexicalAnalyzer* lexer = LexicalAnalyzer::getInstance(); */

    /* lexer->runLexer(filename); */

    /* runLexer(filename); */
    LEXER->initSymbolTable();
    /* LEXER->getSymbolTable().printTable(); */

    std::cout << "done printing init symbols..\n\n";

/*     auto tok = LEXER->getSymbolTable().lookupTokenString("program"); */
/*     tok->printToken(); */

    /* tok = LEXER->getSymbolTable().lookupTokenString("program"); */
    /* tok->printToken(); */

    std::cout << "starting lexer...\n";
    LEXER->runLexer(filename);

    std::cout << "done lexer...\n";

    /* LEXER->getSymbolTable().lookupTokenString("program")->printToken(); */

    /* std::cout << "print all keys:\n"; */

    /* LEXER->getSymbolTable().printAllKeys(); */


    /* std::cout << "printing table again..\n\n"; */
    /* LEXER->getSymbolTable().printTable(); */

    return 0;
}
