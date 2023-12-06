#pragma once
#include "Addons.h"
#include <fstream>

class Generator
{
private:
	// Поля получаемые от класса Parser
	std::vector<std::string> reprIdTable;
	std::vector<std::string> constTable;
	std::vector<std::string> idType;
	std::shared_ptr<Node> syntaxTree;

	// Собственные поля
	std::string data;
	std::string code;
	unsigned short ifId;
	unsigned short whileId;

public:

	Generator(std::vector<std::string> reprIdTable, std::vector<std::string> constTable,
		std::vector<std::string> idType, std::shared_ptr<Node> syntaxTree)
		: reprIdTable(reprIdTable), constTable(constTable), idType(idType), 
		syntaxTree(syntaxTree), ifId(0), whileId(0) {}

	void Start();
	void generateBodyCode(const std::shared_ptr<Node>& node);
	void generateExpressionCode(const std::shared_ptr<Node>& node);
	void generateOperandCode(const std::shared_ptr<Node>& node);
	void generateConditionCode(const std::shared_ptr<Node>& node);
	std::string getOperand(const std::shared_ptr<Node>& node);
	void printAsmCode();
};

