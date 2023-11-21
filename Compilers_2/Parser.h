#pragma once
#include "Addons.h"
#include <iomanip>

class Parser
{
private:
	// ���� ���������� �� ������ Lexer
	std::vector<Lexeme> lexStream;
	std::vector<std::string> reprIdTable;
	std::vector<std::string> constTable;
	std::vector<std::string> lexemClass;

	// ����������� ����
	size_t lexIndex;
	std::shared_ptr<Node> syntaxTree;
	std::vector<std::string> idType;

public:

	Parser(std::vector<Lexeme> lexStream) : lexStream(lexStream), lexIndex(0), syntaxTree(nullptr) {}
	Parser(std::vector<Lexeme> lexStream, std::vector<std::string> reprIdTable, 
		std::vector<std::string> constTable, std::vector<std::string> lexemClass) 
		: lexStream(lexStream), lexIndex(0), syntaxTree(nullptr), reprIdTable(reprIdTable), 
		constTable(constTable), lexemClass(lexemClass) {}

	void Parse(); // �����, ���������� ��������� ������� (���������� ��������� ��������� ������)
	std::string CurrentLexClass(); // ���������� ����� ������� �������
	std::shared_ptr<Node> GetSyntaxTree() { return syntaxTree; }
	std::shared_ptr<Node> ParseS(); // ��������� �������
	std::shared_ptr<Node> ParseBody();
	std::shared_ptr<Node> ParseOperator();
	std::shared_ptr<Node> ParseDeclaration();
	std::shared_ptr<Node> ParseAssignment();
	std::shared_ptr<Node> ParseExpression();
	std::shared_ptr<Node> ParseMult();
	std::shared_ptr<Node> ParseGroup();
	std::shared_ptr<Node> ParseCallF();
	std::shared_ptr<Node> ParseOperation();
	std::shared_ptr<Node> ParseConst();
	std::shared_ptr<Node> ParseIdentif();
	std::shared_ptr<Node> ParseConditionOperator();
	std::shared_ptr<Node> ParseCondition();
	std::shared_ptr<Node> ParseLoop();
	std::shared_ptr<Node> ParseReturn();
	std::shared_ptr<Node> ParseFunction();
	std::shared_ptr<Node> ParseArguments(); 
	std::shared_ptr<Node> Match(const std::string& expectedLex);
	std::shared_ptr<Node> Match(const std::vector<std::string>& expectedLexemes);
	bool IsMatch(const std::vector<std::string>& expectedLexemes);
	bool IsMatch(const std::string& expectedLex);
	void NextLexeme();
	void PrintTable();
};

