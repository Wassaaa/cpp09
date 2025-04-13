#include <iostream>
#include "BitcoinExchange.hpp"

int main(int argc, char **argv)
{
    std::string inputFile;
    if (argc == 1) {
        std::cout << "running with default input file input.txt" << std::endl;
        inputFile = "../../ex00/input.txt";
    }
    else if (argc == 2) {
        inputFile = argv[1];
    }
    else {
        std::cout << "usage: " << argv[0] << " [input file]" << std::endl;
        return 1;
    }
    BitcoinExchange btc;
    btc.parseData();
    btc.parseInput(inputFile);
    return 0;
}
