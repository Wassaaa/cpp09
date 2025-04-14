#include "PmergeMe.hpp"
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <iostream>

PmergeMe::PmergeMe()
{}

PmergeMe::PmergeMe(PmergeMe &other)
    : _vec(other._vec)
{}

PmergeMe &PmergeMe::operator=(PmergeMe &other)
{
    if (this != &other) {
        _vec = other._vec;
    }
    return *this;
}
PmergeMe::~PmergeMe()
{}

void PmergeMe::swapPair(vectorIt it, int level)
{
    vectorIt start = it - level + 1;
    vectorIt end = start + level;
    while (start != end) {
        std::iter_swap(start, start + level);
        start++;
    }
}

void PmergeMe::addNumber(const std::string &number)
{
    if (number.length() > 11 || stol(number) > std::numeric_limits<int>::max())
        throw std::runtime_error("number too large");
    int nbr = stoi(number);
    if (nbr < 0)
        throw std::runtime_error("only positive numbers allowed");
    _vec.push_back(nbr);
}

void PmergeMe::mergeInsert(int level)
{
    printVerbose(level);
    int pairs = _vec.size() / level;
    if (pairs < 2)
        return;
    int jump = level * 2;
    int is_odd = pairs % 2 == 1;

    vectorIt start = _vec.begin();
    vectorIt last = _vec.begin() + pairs * level;
    vectorIt end = last - (is_odd * level);
    for (vectorIt it = start; it != end; it += jump) {
        vectorIt thisPair = it + level - 1;
        vectorIt nextPair = it + level * 2 - 1;
        if (*nextPair < *thisPair) {
            swapPair(thisPair, level);
        }
    }
    mergeInsert(level * 2);
}

void PmergeMe::printVector()
{
    for (auto &nbr : _vec) {
        std::cout << nbr << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::printVerbose(int level)
{
    int groupSize = level;
    int pairs = _vec.size() / groupSize;
    int leftover = _vec.size() % groupSize;

    std::cout << "Level " << level << ": Groups of size " << groupSize << std::endl;

    vectorIt start = _vec.begin();
    vectorIt last = _vec.begin() + pairs * groupSize;

    // Print groups
    for (vectorIt it = start; it != last; it += groupSize) {
        std::cout << "[ ";
        for (vectorIt groupIt = it; groupIt != it + groupSize && groupIt != _vec.end(); ++groupIt) {
            std::cout << *groupIt << " ";
        }
        std::cout << "]";
    }

    // Print leftover elements, if any
    if (leftover > 0) {
        std::cout << " | ";
        for (vectorIt it = last; it != _vec.end(); ++it) {
            std::cout << *it << " ";
        }
    }

    std::cout << std::endl;
}
