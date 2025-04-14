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
    void printMainAndPend(const std::vector<vectorIt> &main, const std::vector<vectorIt> &pend, int level);
    void printVerbose(int level);
    size_t jacobsthal_number(long n);
    bool comp(vectorIt left, vectorIt right);

    int comparisons = 0;

private:
    std::vector<int> _vec;
};
