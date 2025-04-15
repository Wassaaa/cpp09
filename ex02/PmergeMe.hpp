#pragma once
#include <string>
#include <vector>
#include <deque>

class PmergeMe
{
public:
    PmergeMe();
    PmergeMe(PmergeMe &other);
    PmergeMe &operator=(PmergeMe &other);
    ~PmergeMe();

    typedef std::vector<int>::iterator vectorIt;
    typedef std::deque<int>::iterator dequeIt;

    void swapPair(vectorIt it, int level);
    void swapPair(dequeIt it, int level);
    void addNumber(const std::string &number);
    void mergeInsertVector(int level);
    void mergeInsertDeque(int level);
    void printMainAndPend(const std::vector<vectorIt> &main, const std::vector<vectorIt> &pend, int level);
    void printMainAndPend(const std::vector<dequeIt> &main, const std::vector<dequeIt> &pend, int level);
    void printVerbose(int level);
    size_t jacobsthal_number(long n);
    bool comp(vectorIt left, vectorIt right);
    bool comp(dequeIt left, dequeIt right);
    void sortAndTime();

    void printAlgorithmPhase(const std::string &phase, int level);
    void printArrayState(const std::string &message);
    void printPairComparison(vectorIt left, vectorIt right, bool swapped);
    void printMainAndPendDetailed(const std::vector<vectorIt> &main, const std::vector<vectorIt> &pend, int level);
    void printJacobsthalInsert(vectorIt pendElem, vectorIt insertPos, size_t jacNum);
    void printGroupStructure(int level);

    int vecComparisons = 0;
    int deqComparisons = 0;

private:
    std::vector<int> _vec;
    std::deque<int> _deque;
};
