#include "lexicalAnalyzer.hpp"
#include "parser.hpp"
/* #include "symbolTable.h" */


int main() {

    /* std::string filename = "../project_description/testPgms/correct/recursiveFib.src"; */
    std::string filename = "testparseprog.src";

    /* LexicalAnalyzer* lexer = LexicalAnalyzer::getInstance(); */

    /* lexer->runLexer(filename); */

    /* runLexer(filename); */
    /* std::cout << "h1" << std::endl; */
    LEXER->setinFile(new inFile(filename));
 
    /* std::cout << "h2" << std::endl; */
    LEXER->addSymbolTable();
    /* LEXER->getSymbolTable().printTable(); */
    /* throw std::runtime_error("testing exception!"); */

    /* std::cout << "h3" << std::endl; */
    PARSER->initLexer(LEXER);
    PARSER->initLogger(LOGGER);

    PARSER->parse_program();

    /* PARSER->getLexer()->runLexer(); */

    /* std::cout << "file pos: " << PARSER->getLexer()->getPos(); */

    /* Token *lktok = PARSER->getLexer()->getlookahead(); */
    /* lktok->printToken(); */

    /* lktok = PARSER->getLexer()->getlookahead(); */
    /* lktok->printToken(); */

    /* lktok = PARSER->getLexer()->getlookahead(); */
    /* lktok->printToken(); */

/*     std::cout << "resetting...\n\n\n"; */

/*     PARSER->getLexer()->setinFilepos(-1); */
/*     PARSER->getLexer()->runLexer(); */






/*     auto tok = LEXER->getSymbolTable().lookupTokenString("program"); */
/*     tok->printToken(); */

    /* tok = LEXER->getSymbolTable().lookupTokenString("program"); */
    /* tok->printToken(); */

/*     std::cout << "starting lexer...\n"; */
/*     LEXER->runLexer(); */

/*     std::cout << "done lexer...\n"; */

    /* LEXER->getSymbolTable().lookupTokenString("program")->printToken(); */

    /* std::cout << "print all keys:\n"; */

    /* LEXER->getSymbolTable().printAllKeys(); */


    /* std::cout << "printing table again..\n\n"; */
    /* LEXER->getSymbolTable().printTable(); */

    return 0;
}
