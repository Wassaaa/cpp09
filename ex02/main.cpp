#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc == 1) {
        // Temporary default with integers
        std::vector<int> args = {5, 2, 3, 43, 12, 9, 7, 23, 131, 55, 1};
        PmergeMe pmergeMe;
        for (const auto &arg : args) {
            pmergeMe.addNumber(std::to_string(arg)); // Convert int to string
        }

        pmergeMe.mergeInsert(1);
        return 0;
    }
    if (argc > 1) {
        PmergeMe pmergeMe;
        for (int i = 1; i < argc; ++i) {
            pmergeMe.addNumber(argv[i]);
        }

        pmergeMe.mergeInsert(1);
    }
    else {
        std::cerr << "usage: " << argv[0] << " <list of integers>" << std::endl;
    }
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <list of integers>" << std::endl;
    }
    return 0;
}
