#include "PmergeMe.hpp"
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <cmath>

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

size_t PmergeMe::jacobsthal_number(long n)
{
    return round((pow(2, n + 1) + pow(-1, n)) / 3);
}

bool PmergeMe::comp(vectorIt left, vectorIt right)
{
    comparisons++;
    return *left < *right;
}

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
    int pairs = _vec.size() / level;
    if (pairs < 2)
        return;
    int jump = level * 2;
    int is_odd = pairs % 2 == 1;

    vectorIt start = _vec.begin();
    vectorIt last = _vec.begin() + (pairs * level);
    vectorIt end = last - (is_odd * level);
    for (vectorIt it = start; it != end; it += jump) {
        vectorIt thisPair = it + level - 1;
        vectorIt nextPair = it + level * 2 - 1;
        if (comp(nextPair, thisPair)) {
            swapPair(thisPair, level);
        }
    }

    mergeInsert(level * 2);
    printVerbose(level);
    std::vector<vectorIt> main;
    main.reserve(pairs);
    std::vector<vectorIt> pend;
    pend.reserve(pairs / 2 + is_odd);

    main.insert(main.end(), _vec.begin() + level - 1);
    main.insert(main.end(), _vec.begin() + level * 2 - 1);

    for (int i = 4; i <= pairs; i += 2) {
        pend.insert(pend.end(), _vec.begin() + level * (i - 1) - 1);
        main.insert(main.end(), _vec.begin() + level * i - 1);
    }

    if (is_odd) {
        pend.insert(pend.end(), end + level - 1);
    }

    size_t prev_jacobsthal = jacobsthal_number(1);
    size_t inserted_numbers = 0;
    for (int k = 2;; k++) {
        printMainAndPend(main, pend, level);
        size_t curr_jacobsthal = jacobsthal_number(k);
        size_t jacobsthal_diff = curr_jacobsthal - prev_jacobsthal;
        if (jacobsthal_diff > pend.size())
            break;
        int nbr_of_times = jacobsthal_diff;
        auto pend_it = pend.begin() + jacobsthal_diff - 1;
        auto bound_it = main.begin() + curr_jacobsthal + inserted_numbers;
        while (nbr_of_times) {
            auto idx = std::upper_bound(main.begin(), bound_it, *pend_it, [this](vectorIt element, vectorIt value) {
                return this->comp(element, value);
            });
            auto inserted = main.insert(idx, *pend_it);
            nbr_of_times--;
            pend_it = pend.erase(pend_it);
            pend_it--;
            // handle offset if insertion lands exactly where the bound is.
            // offset += (inserted - main.begin()) == static_cast<int>(curr_jacobsthal + inserted_numbers);
            bound_it = main.begin() + curr_jacobsthal + inserted_numbers;
            bound_it -= (inserted == bound_it);
        }
        prev_jacobsthal = curr_jacobsthal;
        inserted_numbers += jacobsthal_diff;
    }

    for (ssize_t i = pend.size() - 1; i >= 0; i--) {
        auto curr_pend = pend.begin() + i;
        auto curr_bound = main.begin() + main.size() - pend.size() + i + is_odd;
        auto idx = std::upper_bound(main.begin(), curr_bound, *curr_pend,
                                    [this](vectorIt element, vectorIt value) { return this->comp(element, value); });
        main.insert(idx, *curr_pend);
    }

    /* temp copy container for sorted main */
    std::vector<int> copy;
    copy.reserve(_vec.size());

    for (auto it : main) {
        for (int i = 0; i < level; i++) {
            auto pair_start = it;
            std::advance(pair_start, -level + i + 1);
            copy.push_back(*pair_start);
        }
    }

    /* Replace values in the original container. */
    std::copy(copy.begin(), copy.end(), _vec.begin());
}

void PmergeMe::printMainAndPend(const std::vector<vectorIt> &main, const std::vector<vectorIt> &pend, int level)
{
    std::cout << "Level " << level << ":\n";

    std::cout << "  Main vector: [ ";
    for (const auto &it : main) {
        std::cout << *it << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << "  Pend vector: [ ";
    for (const auto &it : pend) {
        std::cout << *it << " ";
    }
    std::cout << "]" << std::endl;
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
