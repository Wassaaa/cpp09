#pragma once
#include <stack>
#include <string>

typedef enum e_operators
{
    OP_ADD = '+',
    OP_SUB = '-',
    OP_MUL = '*',
    OP_DIV = '/'
} operators;

class RPN
{
public:
    RPN();
    RPN(RPN &other);
    RPN &operator=(RPN &other);
    ~RPN();

    bool isOperator(char token);
    operators getOperator(char token);

    void calculate(operators op);
    void calculateRPN(std::string &polish);

private:
    std::stack<int> _stack;
};
