#pragma once
#include <string>
#include <vector>

class PmergeMe
{
public:
    PmergeMe();
    PmergeMe(PmergeMe &other);
    PmergeMe &operator=(PmergeMe &other);
    ~PmergeMe();

    typedef std::vector<int>::iterator vectorIt;

    void swapPair(vectorIt it, int level);
    void addNumber(const std::string &number);
    void mergeInsert(int level);
    void printVector();
    void printVerbose(int level);

private:
    std::vector<int> _vec;
};
