#pragma once
#include "Addons.h"
#include <fstream>

class Generator
{
private:
	// ���� ���������� �� ������ Parser
	std::vector<std::string> reprIdTable;
	std::vector<std::string> constTable;
	std::vector<std::string> idType;
	std::shared_ptr<Node> syntaxTree;

	// ����������� ����
	std::string data;
	std::string code;

public:

	Generator(std::vector<std::string> reprIdTable, std::vector<std::string> constTable,
		std::vector<std::string> idType, std::shared_ptr<Node> syntaxTree)
		: reprIdTable(reprIdTable), constTable(constTable), idType(idType), 
		syntaxTree(syntaxTree) {}

	void Start();
	void generateBodyCode(const std::shared_ptr<Node>& node);
	void generateExpressionCode(const std::shared_ptr<Node>& node);
	void generateOperandCode(const std::shared_ptr<Node>& node);
	void generateCondition(const std::shared_ptr<Node>& node);
	std::string getOperand(const std::shared_ptr<Node>& node);
};

