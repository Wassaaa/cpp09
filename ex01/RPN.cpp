#include "RPN.hpp"
#include <sstream>
#include <iostream>
#include <limits>

RPN::RPN()
{}

RPN::RPN(RPN &other)
    : _stack(other._stack)
{}

RPN &RPN::operator=(RPN &other)
{
    if (this != &other) {
        _stack = other._stack;
    }
    return *this;
}

RPN::~RPN()
{}

bool RPN::isOperator(char token)
{
    switch (token) {
    case OP_ADD:
    case OP_SUB:
    case OP_MUL:
    case OP_DIV:
        return true;
    default:
        return false;
    }
}

operators RPN::getOperator(char token)
{
    return static_cast<operators>(token);
}

void RPN::calculate(operators op)
{
    if (_stack.size() < 2) {
        throw std::runtime_error("Invalid RPN expression: insufficient operands");
    }
    int b = _stack.top();
    _stack.pop();
    int a = _stack.top();
    _stack.pop();

    switch (op) {
    case OP_ADD:
        if (a > 0 && b > std::numeric_limits<int>::max() - a) {
            throw std::overflow_error("Addition overflow");
        }
        if (a < 0 && b < std::numeric_limits<int>::min() - a) {
            throw std::underflow_error("Addition underflow");
        }
        _stack.push(a + b);
        break;
    case OP_SUB:
        if (a > 0 && b < std::numeric_limits<int>::min() + a) {
            throw std::underflow_error("Subtraction underflow");
        }
        if (a < 0 && b > std::numeric_limits<int>::max() + a) {
            throw std::overflow_error("Subtraction overflow");
        }
        _stack.push(a - b);
        break;
    case OP_MUL:
        if (a != 0 && (b > std::numeric_limits<int>::max() / a || b < std::numeric_limits<int>::min() / a)) {
            throw std::overflow_error("Multiplication overflow");
        }
        _stack.push(a * b);
        break;
    case OP_DIV:
        if (b == 0) {
            throw std::runtime_error("Division by zero");
        }
        if (a == std::numeric_limits<int>::min() && b == -1) {
            throw std::overflow_error("Division overflow");
        }
        _stack.push(a / b);
        break;
    default:
        throw std::runtime_error("Unknown operator");
    }
}

void RPN::calculateRPN(std::string &polish)
{
    std::string token;
    std::istringstream iss(polish);

    while (iss >> token) {
        if ((token.size() == 1 && std::isdigit(token[0])) || (token.size() > 1 && token[0] == '-' && isdigit(token[1])))
            _stack.push(std::stoi(token));
        else if (token.size() == 1 && isOperator(token[0]))
            calculate(getOperator(token[0]));
        else
            throw std::runtime_error("Invalid token: " + token);
    }
    if (_stack.size() != 1) {
        throw std::runtime_error("Invalid RPN expression");
    }
    std::cout << _stack.top() << std::endl;
}
