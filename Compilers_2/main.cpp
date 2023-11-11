#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main() {
    Lexer lex("test.txt");
    lex.Start();
    lex.DisplayTables();
    Parser parser(lex.GetLexStream(), lex.GetReprIdTable(), lex.GetConstTable(), lex.GetLexemClasses());
    parser.Parse();
    printTree(parser.GetSyntaxTree());
    return 0;
}