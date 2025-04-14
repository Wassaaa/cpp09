#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc == 1) {
        // Temporary default with integers
        std::vector<int> args = {11, 2, 17, 0, 16, 8, 6, 15, 10, 3, 21, 1, 18, 9, 14, 19, 12, 5, 4, 20, 13, 7};
        PmergeMe pmergeMe;
        for (const auto &arg : args) {
            pmergeMe.addNumber(std::to_string(arg)); // Convert int to string
        }

        pmergeMe.mergeInsert(1);
        // std::cout << "jnr: " << pmergeMe.jacobsthal_number(23) << std::endl;
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
