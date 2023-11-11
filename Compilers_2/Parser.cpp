#include "Parser.h"

void Parser::Parse()
{
    syntaxTree = ParseS();
}

std::string Parser::CurrentLexClass()
{
    return lexemClass[lexStream[lexIndex].lexClass];
}

std::shared_ptr<Node> Parser::ParseS()
{
    if (CurrentLexClass() == "main") {
        std::vector<std::shared_ptr<Node>> children;
        children.push_back(Match("main"));
        children.push_back(Match("("));
        children.push_back(Match(")"));
        children.push_back(Match("{"));
        children.push_back(ParseBody());
        children.push_back(Match("}"));
        children.push_back(ParseFunction());

        return std::make_shared<Node>("<S>", children);
    }
    else {
        std::cout << "Unexpected lexema: " + CurrentLexClass();
        throw std::runtime_error("Unexpected lexema: " + CurrentLexClass());
    }
}

std::shared_ptr<Node> Parser::ParseBody()
{
    std::vector<std::shared_ptr<Node>> children;
    while (IsMatch({ "integer", "string", "bool", "IDENTIF", "if", "while", "return" })) {
        children.push_back(ParseOperator());
    }
    return std::make_shared<Node>("<Body>", children);
}

std::shared_ptr<Node> Parser::ParseOperator()
{
    std::vector<std::shared_ptr<Node>> children;
    if (IsMatch({ "integer", "string", "bool" })) {
        children.push_back(ParseDeclaration());
    }
    else if (IsMatch("IDENTIF")) {
        children.push_back(ParseAssignment());
    }
    else if (IsMatch("if")) {
        children.push_back(ParseConditionOperator());
    }
    else if (IsMatch("while")) {
        children.push_back(ParseLoop());
    }
    else if (IsMatch("return")) {
        children.push_back(ParseReturn());
    }
    return std::make_shared<Node>("<Operator>", children);
}

std::shared_ptr<Node> Parser::ParseDeclaration()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match({ "integer", "string", "bool" }));
    children.push_back(ParseIdentif());
    children.push_back(Match(";"));
    return std::make_shared<Node>("<Declaration>", children);
}

std::shared_ptr<Node> Parser::ParseAssignment()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(ParseIdentif());
    children.push_back(Match("="));
    children.push_back(ParseExpression());
    children.push_back(Match(";"));
    return std::make_shared<Node>("<Assignment>", children);
}

std::shared_ptr<Node> Parser::ParseExpression()
{
    std::vector<std::shared_ptr<Node>> children;
    if (IsMatch("IDENTIF")) { // (������� ������� ��� �������� �� ���� ������� �����) 
        if (lexemClass[lexStream[lexIndex + 1].lexClass] == "(") {
            children.push_back(ParseCallF());
        }
        else if (lexemClass[lexStream[lexIndex + 1].lexClass] == "+" 
            || lexemClass[lexStream[lexIndex + 1].lexClass] == "-") {
            children.push_back(ParseOperation());
        }
        else {
            children.push_back(ParseIdentif());
        }
    }
    else if (IsMatch(std::vector<std::string>{"true", "false"})) {
        children.push_back(Match(std::vector<std::string>{ "true", "false" }));
    }
    else if (IsMatch(std::vector<std::string>{"\"", "CONST"})) {
        children.push_back(ParseConst());
    }
    else if (IsMatch({"CONST", "true", "false"})) {
        children.push_back(ParseOperation());
    }
    return std::make_shared<Node>("<Expression>", children);
}

std::shared_ptr<Node> Parser::ParseMult()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(ParseGroup());
    while (IsMatch(std::vector<std::string>{"*", "/"})) {
        children.push_back(Match(std::vector<std::string>{"*", "/"}));
        children.push_back(ParseGroup());
    }
    return std::make_shared<Node>("<Mult>", children);
}

std::shared_ptr<Node> Parser::ParseGroup()
{
    std::vector<std::shared_ptr<Node>> children;
    if (IsMatch("(")) {
        children.push_back(Match("("));
        children.push_back(ParseOperation());
        children.push_back(Match(")"));
    }
    else if (IsMatch("CONST")) {
        children.push_back(ParseConst());
    }
    else if (IsMatch("IDENTIF")) {
        if (lexemClass[lexStream[lexIndex + 1].lexClass] == "(") {
            children.push_back(ParseCallF());
        }
        else {
            children.push_back(ParseIdentif());
        }
    }
    return std::make_shared<Node>("<Group>", children);
}

std::shared_ptr<Node> Parser::ParseCallF()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(ParseIdentif());
    children.push_back(Match("("));
    while (IsMatch("IDENTIF")) {
        children.push_back(ParseIdentif());
        if (!IsMatch(")")) 
            children.push_back(Match(","));
    }
    children.push_back(Match(")"));
    return std::make_shared<Node>("<CallFunction>", children);
}

std::shared_ptr<Node> Parser::ParseOperation()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(ParseMult());
    while (IsMatch(std::vector<std::string>{"+", "-"})) {
        children.push_back(Match(std::vector<std::string>{"+", "-"}));
        children.push_back(ParseMult());
    }
    return std::make_shared<Node>("<Operation>", children);
}

std::shared_ptr<Node> Parser::ParseConst()
{
    std::vector<std::shared_ptr<Node>> children;
    if (IsMatch("CONST")) {
        children.push_back(std::make_shared<Node>(constTable[lexStream[lexIndex].numInTable]));
        NextLexeme();
    }
    else {
        children.push_back(Match("\""));
        children.push_back(std::make_shared<Node>(constTable[lexStream[lexIndex].numInTable]));
        NextLexeme();
        children.push_back(Match("\""));
    }
    return std::make_shared<Node>("<CONST>", children);
}

std::shared_ptr<Node> Parser::ParseIdentif()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(std::make_shared<Node>(reprIdTable[lexStream[lexIndex].numInTable]));
    NextLexeme();
    return std::make_shared<Node>("<IDENTIF>", children);
}

std::shared_ptr<Node> Parser::ParseConditionOperator() // ��������
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match("if"));
    children.push_back(Match("("));
    children.push_back(ParseCondition());
    children.push_back(Match(")"));
    children.push_back(Match("then"));
    children.push_back(ParseBody());
    children.push_back(Match("else"));
    children.push_back(ParseBody());
    children.push_back(Match("endif"));
    children.push_back(Match(";"));
    return std::make_shared<Node>("<ConditionOper>", children);
}

std::shared_ptr<Node> Parser::ParseCondition()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(ParseExpression());
    children.push_back(Match({"==", "!=", ">", "<", ">=", "<="}));
    children.push_back(ParseExpression());
    return std::make_shared<Node>("<Condition>", children);
}

std::shared_ptr<Node> Parser::ParseLoop()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match("while"));
    children.push_back(ParseCondition());
    children.push_back(Match(";"));
    children.push_back(ParseBody());
    children.push_back(Match("endwhile"));
    children.push_back(Match(";"));
    return std::make_shared<Node>("<Loop>", children);
}

std::shared_ptr<Node> Parser::ParseReturn()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match("return"));
    children.push_back(ParseIdentif());
    children.push_back(Match(";"));
    return std::make_shared<Node>("<Return>", children);
}

std::shared_ptr<Node> Parser::ParseFunction()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match({ "integer", "string", "bool" }));
    children.push_back(Match("proc"));
    children.push_back(ParseIdentif());
    children.push_back(Match("("));
    while (IsMatch({ "integer", "string", "bool" })) {
        children.push_back(ParseArguments());
    }
    children.push_back(Match(")"));
    children.push_back(Match("{"));
    children.push_back(ParseBody());
    children.push_back(Match("}"));
    return std::make_shared<Node>("<Function>", children);
}

std::shared_ptr<Node> Parser::ParseArguments()
{
    std::vector<std::shared_ptr<Node>> children;
    children.push_back(Match({ "integer", "string", "bool" }));
    children.push_back(ParseIdentif());
    if (!IsMatch(")")) {
        children.push_back(Match(","));
    }
    return std::make_shared<Node>("<Arguments>", children);
}

std::shared_ptr<Node> Parser::Match(const std::string& expectedLex)
{
    if (lexIndex < lexStream.size() && CurrentLexClass() == expectedLex) {
        std::shared_ptr<Node> node = std::make_shared<Node>(CurrentLexClass());
        NextLexeme();
        return node;
    }
    else {
        std::cout << "Unexpected lexema: " + CurrentLexClass();
        throw std::runtime_error("Unexpected lexema: " + CurrentLexClass());
    }
}

std::shared_ptr<Node> Parser::Match(const std::vector<std::string>& expectedLexemes)
{
    if (lexIndex >= lexStream.size()) {
        std::cout << "No more lexema available.";
        throw std::runtime_error("No more lexema available.");
    }

    for (int i = 0; i < expectedLexemes.size(); i++) {
        if (CurrentLexClass() == expectedLexemes[i]) {
            std::shared_ptr<Node> node = std::make_shared<Node>(CurrentLexClass());
            NextLexeme();
            return node;
        }
    }

    return nullptr;
}

bool Parser::IsMatch(const std::vector<std::string>& expectedLexemes)
{
    for (int i = 0; i < expectedLexemes.size(); i++) {
        if (CurrentLexClass() == expectedLexemes[i]) {
            return true;
        }
    }
    return false;
}

bool Parser::IsMatch(const std::string& expectedLex)
{
    return CurrentLexClass() == expectedLex;
}

void Parser::NextLexeme()
{
    if (lexIndex < lexStream.size()) {
        lexIndex++;
    }
    else if (lexemClass[lexStream[lexIndex - 1].lexClass] != "}") {
        std::cout << "No more lexema available.";
        throw std::runtime_error("No more lexema available.");
    }
}
