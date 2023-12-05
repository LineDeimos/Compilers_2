#include "Generator.h"

void Generator::Start()
{
    std::ofstream outputFile("output.asm");
    
    data = "%include \"io.inc\"\nsection .data\n";
    code = "section .text\n    global main\nmain:\n";

    generateBodyCode(syntaxTree);

    code += "    xor eax, eax\n    ret";

    outputFile << data;
    outputFile << code;

    outputFile.close();

    std::cout << "\nAssembly code generated successfully.\n";
}

void Generator::generateBodyCode(const std::shared_ptr<Node>& node) {
    if (!node) {
        return;
    }

    if (node->value == "<Declaration>") {
        if (node->children[0]->value == "string") {
            data += "    " + node->children[1]->children[0]->value + " db \"\"\n";
        } 
        else {
            data += "    " + node->children[1]->children[0]->value + " db 0\n";
        }
        return;
    }
    else if (node->value == "<Assignment>") {
        std::string variableName = node->children[0]->children[0]->value;
        std::shared_ptr<Node> expressionNode = node->children[2];

        // Генерация кода для присваивания значения переменной
        code += "    ; Assignment to " + variableName + "\n";
        code += "    mov ebx, " + variableName + " ; Get the address of the variable\n";

        generateExpressionCode(expressionNode);

        code += "    mov [ebx], eax ; Store the result in the variable\n";

        return;
    }
    else if (node->value == "<ConditionOper>") {
        code += "    ; Condition Operator\n";

        bool elseFlag = node->children.size() > 6;

        generateCondition(node->children[2]);

        code += elseFlag ? "else\n" : "endif\n";

        generateBodyCode(node->children[5]);

        if (elseFlag) {
            code += "    jmp endif:\n";
            code += "else:\n";
            generateBodyCode(node->children[7]);
        }
        
        code += "endif:\n";

        return;
    }
    else if (node->value == "<Loop>") {
        code += "    ; Loop\n";
        generateCondition(node->children[2]);
        return;
    }

    for (const auto& child : node->children) {
        generateBodyCode(child);
    }
}

void Generator::generateExpressionCode(const std::shared_ptr<Node>& node)
{
    if (node->children[0]->value == "<CONST>" || node->children[0]->value == "<IDENTIF>") {
        std::string operand = node->children[0]->children[0]->value;

        if (operand == "false") operand = "0";
        if (operand == "true") operand = "1";

        if (std::isdigit(operand[0])) {
            // Операнд - число
            code += "    mov eax, " + operand + " ; Load constant " + operand + " to eax\n";
        }
        else if (operand[0] == '\"') {
            // Операнд - строка
            std::string str = node->children[0]->children[1]->value;
            code += "    mov eax, \"" + str + "\" ; String content\n";
        }
        else {
            // Операнд - переменная
            code += "    mov eax, [" + operand + "] ; Load variable " + operand + " to eax\n";
        }
        return;
    }

    std::shared_ptr<Node> curNode = node->children[0];

    generateOperandCode(curNode->children[0]);

    code += "    mov eax, ebx\n";

    for (int i = 2; i < curNode->children.size(); i += 2) {
        
        generateOperandCode(curNode->children[i]);

        if (curNode->children[i - 1]->value == "+") {
            code += "    add eax, edx ; Add the result to eax\n";
        }
        else if (curNode->children[i - 1]->value == "-") {
            code += "    sub eax, edx ; Subtract the result from eax\n";
        }
        //code += curNode->children[i]->value + '\n';
    }
    
}

void Generator::generateOperandCode(const std::shared_ptr<Node>& node)
{
    std::string operand = getOperand(node);

    if (operand == "false") operand = "0";
    if (operand == "true") operand = "1";

    if (std::isdigit(operand[0])) {
        // Операнд - число
        code += "    mov edx, " + operand + " ; Load constant " + operand + " to edx\n";
    }
    else if (operand[0] == '\"') {
        // Операнд - строка
        std::string str = node->children[0]->children[1]->value;
        code += "    mov edx, \"" + str + "\" ; String content\n";
    }
    else {
        // Операнд - переменная
        code += "    mov edx, [" + operand + "] ; Load variable " + operand + " to edx\n";
    }
    return;
}

void Generator::generateCondition(const std::shared_ptr<Node>& node)
{
    std::string operation = node->children[1]->value;

    generateExpressionCode(node->children[0]);

    code += "    cmp\n";

    if (operation == "==") {
        code += "    jne ";
    }
    else if (operation == "!=") {

    }
    else if (operation == ">") {

    }
    else if (operation == "<") {

    }
    else if (operation == ">=") {

    }
    else if (operation == "<=") {

    }

    return;
}

std::string Generator::getOperand(const std::shared_ptr<Node>& node)
{

    return node->children[0]->children[0]->children[0]->value;
}
