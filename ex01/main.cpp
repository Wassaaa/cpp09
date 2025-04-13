#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <RPN expression>" << std::endl;
        return 1;
    }
    if (argv[1][0] == '\0') {
        std::cerr << "Error: Empty expression" << std::endl;
        return 1;
    }
    std::string polish = argv[1];

    try {
        RPN rpn;
        rpn.calculateRPN(polish);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}