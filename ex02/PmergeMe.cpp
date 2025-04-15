#include "PmergeMe.hpp"
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <chrono>

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
    vecComparisons++;
    return *left < *right;
}

bool PmergeMe::comp(dequeIt left, dequeIt right)
{
    deqComparisons++;
    return *left < *right;
}

void PmergeMe::sortAndTime()
{
    std::cout << "before sort: ";
    for (auto it : _vec) {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    mergeInsertVector(1);
    auto end = std::chrono::high_resolution_clock::now();
    auto vec_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    mergeInsertDeque(1);
    end = std::chrono::high_resolution_clock::now();
    auto deq_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "after sort: ";
    for (auto it : _vec) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
    std::cout << "Time to process a range of " << _vec.size() << " elements with std::vector : " << vec_elapsed.count()
              << " nanoseconds (" << vecComparisons << " comparisons)" << std::endl;
    std::cout << "Time to process a range of " << _deque.size() << " elements with std::deque : " << deq_elapsed.count()
              << " nanoseconds (" << deqComparisons << " comparisons)" << std::endl;
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

void PmergeMe::swapPair(dequeIt it, int level)
{
    dequeIt start = it - level + 1;
    dequeIt end = start + level;
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
    _deque.push_back(nbr);
}

void PmergeMe::mergeInsertVector(int level)
{
    std::vector<int> &container = _vec;
    int pairs = container.size() / level;
    if (pairs < 2)
        return;
    int jump = level * 2;
    int is_odd = pairs % 2 == 1;

    printAlgorithmPhase("COMPARING AND SORTING PAIRS", level);
    printArrayState("Before pair comparisons");
    printGroupStructure(level);

    vectorIt start = container.begin();
    vectorIt last = container.begin() + (pairs * level);
    vectorIt end = last - (is_odd * level);
    for (vectorIt it = start; it != end; it += jump) {
        vectorIt thisPair = it + level - 1;
        vectorIt nextPair = it + level * 2 - 1;
        bool willSwap = comp(nextPair, thisPair);
        printPairComparison(thisPair, nextPair, willSwap);

        if (willSwap) {
            swapPair(thisPair, level);
        }
    }
    printArrayState("After pair comparisons");
    printGroupStructure(level);

    printAlgorithmPhase("RECURSIVE CALL", level * 2);
    mergeInsertVector(level * 2);

    printAlgorithmPhase("BUILDING MAIN AND PEND CHAINS", level);
    std::vector<vectorIt> main;
    main.reserve(pairs);
    std::vector<vectorIt> pend;
    pend.reserve(pairs / 2 + is_odd);

    std::cout << "\n  Adding first two elements to main chain\n";
    main.insert(main.end(), container.begin() + level - 1);
    main.insert(main.end(), container.begin() + level * 2 - 1);

    std::cout << "\n  Distributing remaining elements between main and pend\n";
    for (int i = 4; i <= pairs; i += 2) {
        pend.insert(pend.end(), container.begin() + level * (i - 1) - 1);
        main.insert(main.end(), container.begin() + level * i - 1);
    }

    if (is_odd) {
        std::cout << "\n  Adding odd final element to pend\n";
        pend.insert(pend.end(), end + level - 1);
    }

    printMainAndPendDetailed(main, pend, level);

    printAlgorithmPhase("JACOBSTHAL INSERTIONS", level);
    std::cout << "\n  Jacobsthal sequence determines insertion order\n";
    std::cout << "  This allows us to minimize comparisons by intelligently\n";
    std::cout << "  choosing which elements to insert first\n\n";

    size_t prev_jacobsthal = jacobsthal_number(1);
    size_t inserted_numbers = 0;
    for (int k = 2;; k++) {
        size_t curr_jacobsthal = jacobsthal_number(k);
        size_t jacobsthal_diff = curr_jacobsthal - prev_jacobsthal;

        std::cout << "  Jacobsthal step: J(" << k << ") = " << curr_jacobsthal << ", inserting " << jacobsthal_diff
                  << " elements\n";

        if (jacobsthal_diff > pend.size())
            break;

        int nbr_of_times = jacobsthal_diff;
        auto pend_it = pend.begin() + jacobsthal_diff - 1;
        auto bound_it = main.begin() + curr_jacobsthal + inserted_numbers;

        while (nbr_of_times) {
            std::cout << "    Inserting element: " << *(*pend_it) << "\n";
            std::cout << "    Search boundary: up to position " << std::distance(main.begin(), bound_it) << "\n";

            auto idx = std::upper_bound(main.begin(), bound_it, *pend_it, [this](vectorIt element, vectorIt value) {
                return this->comp(element, value);
            });

            std::cout << "    Found insertion point at position: " << std::distance(main.begin(), idx) << "\n";

            auto inserted = main.insert(idx, *pend_it);
            nbr_of_times--;
            pend_it = pend.erase(pend_it);
            pend_it--;

            // handle offset if insertion lands exactly where the bound is
            bool adjustedBoundary = (inserted == bound_it);
            bound_it = main.begin() + curr_jacobsthal + inserted_numbers;
            bound_it -= adjustedBoundary;

            if (adjustedBoundary) {
                std::cout << "    Adjusted boundary because insertion was at boundary\n";
            }

            printMainAndPendDetailed(main, pend, level);
        }
        prev_jacobsthal = curr_jacobsthal;
        inserted_numbers += jacobsthal_diff;
    }

    printAlgorithmPhase("FINAL INSERTIONS", level);
    std::cout << "\n  Inserting remaining elements from pend\n";

    for (ssize_t i = pend.size() - 1; i >= 0; i--) {
        auto curr_pend = pend.begin() + i;
        auto curr_bound = main.begin() + main.size() - pend.size() + i + is_odd;

        std::cout << "  Inserting " << *(*curr_pend) << " with search bound up to "
                  << std::distance(main.begin(), curr_bound) << "\n";

        auto idx = std::upper_bound(main.begin(), curr_bound, *curr_pend,
                                    [this](vectorIt element, vectorIt value) { return this->comp(element, value); });

        std::cout << "  Found insertion point at position " << std::distance(main.begin(), idx) << "\n";

        main.insert(idx, *curr_pend);
        printMainAndPendDetailed(main, pend, level);
    }

    printAlgorithmPhase("FINAL RECONSTRUCTION", level);
    std::cout << "\n  Building the final sorted array from main chain references\n";

    /* temp copy container for sorted main */
    std::vector<int> copy;
    copy.reserve(container.size());

    for (auto it : main) {
        for (int i = 0; i < level; i++) {
            auto pair_start = it;
            std::advance(pair_start, -level + i + 1);
            copy.push_back(*pair_start);
        }
    }

    /* Replace values in the original container. */
    std::copy(copy.begin(), copy.end(), container.begin());

    printArrayState("Final sorted array");
}

void PmergeMe::mergeInsertDeque(int level)
{
    std::deque<int> &container = _deque;
    int pairs = container.size() / level;
    if (pairs < 2)
        return;
    int jump = level * 2;
    int is_odd = pairs % 2 == 1;

    dequeIt start = container.begin();
    dequeIt last = container.begin() + (pairs * level);
    dequeIt end = last - (is_odd * level);
    for (dequeIt it = start; it != end; it += jump) {
        dequeIt thisPair = it + level - 1;
        dequeIt nextPair = it + level * 2 - 1;
        if (comp(nextPair, thisPair)) {
            swapPair(thisPair, level);
        }
    }

    mergeInsertDeque(level * 2);
    // printVerbose(level);
    std::vector<dequeIt> main;
    main.reserve(pairs);
    std::vector<dequeIt> pend;
    pend.reserve(pairs / 2 + is_odd);

    main.insert(main.end(), container.begin() + level - 1);
    main.insert(main.end(), container.begin() + level * 2 - 1);

    for (int i = 4; i <= pairs; i += 2) {
        pend.insert(pend.end(), container.begin() + level * (i - 1) - 1);
        main.insert(main.end(), container.begin() + level * i - 1);
    }

    if (is_odd) {
        pend.insert(pend.end(), end + level - 1);
    }

    size_t prev_jacobsthal = jacobsthal_number(1);
    size_t inserted_numbers = 0;
    for (int k = 2;; k++) {
        // printMainAndPend(main, pend, level);
        size_t curr_jacobsthal = jacobsthal_number(k);
        size_t jacobsthal_diff = curr_jacobsthal - prev_jacobsthal;
        if (jacobsthal_diff > pend.size())
            break;
        int nbr_of_times = jacobsthal_diff;
        auto pend_it = pend.begin() + jacobsthal_diff - 1;
        auto bound_it = main.begin() + curr_jacobsthal + inserted_numbers;
        while (nbr_of_times) {
            auto idx = std::upper_bound(main.begin(), bound_it, *pend_it,
                                        [this](dequeIt element, dequeIt value) { return this->comp(element, value); });
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
                                    [this](dequeIt element, dequeIt value) { return this->comp(element, value); });
        main.insert(idx, *curr_pend);
    }

    /* temp copy container for sorted main */
    std::vector<int> copy;
    copy.reserve(container.size());

    for (auto it : main) {
        for (int i = 0; i < level; i++) {
            auto pair_start = it;
            std::advance(pair_start, -level + i + 1);
            copy.push_back(*pair_start);
        }
    }

    /* Replace values in the original container. */
    std::copy(copy.begin(), copy.end(), container.begin());
}

void PmergeMe::printMainAndPend(const std::vector<vectorIt> &main, const std::vector<vectorIt> &pend, int level)
{
    (void)level;
    std::cout << "Main vector: [ ";
    for (const auto &it : main) {
        std::cout << *it << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Pend vector: [ ";
    for (const auto &it : pend) {
        std::cout << *it << " ";
    }
    std::cout << "]" << std::endl;
}

void PmergeMe::printMainAndPend(const std::vector<dequeIt> &main, const std::vector<dequeIt> &pend, int level)
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

void PmergeMe::printAlgorithmPhase(const std::string &phase, int level)
{
    std::cout << "\n=== " << phase << " (Level " << level << ") ===\n";
}

void PmergeMe::printArrayState(const std::string &message)
{
    std::cout << message << ": [ ";
    for (size_t i = 0; i < _vec.size(); ++i) std::cout << _vec[i] << " ";
    std::cout << "]\n";
}

void PmergeMe::printGroupStructure(int level)
{
    std::cout << "Groups (level " << level << "): ";
    for (size_t i = 0; i < _vec.size(); i += level) {
        std::cout << "[";
        for (size_t j = i; j < i + level && j < _vec.size(); ++j)
            std::cout << _vec[j] << (j + 1 < i + level && j + 1 < _vec.size() ? " " : "");
        std::cout << "] ";
    }
    std::cout << "\n";
}

void PmergeMe::printPairComparison(vectorIt left, vectorIt right, bool swapped)
{
    std::cout << "Comparing " << *left << " and " << *right << (swapped ? " -> swapped\n" : " -> not swapped\n");
}

void PmergeMe::printMainAndPendDetailed(const std::vector<vectorIt> &main, const std::vector<vectorIt> &pend, int level)
{
    (void)level;
    std::cout << "Main vector: [ ";
    for (const auto &it : main) {
        std::cout << *it << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Pend vector: [ ";
    for (const auto &it : pend) {
        std::cout << *it << " ";
    }
    std::cout << "]" << std::endl;
}

void PmergeMe::printJacobsthalInsert(vectorIt pendElem, vectorIt insertPos, size_t jacNum)
{
    std::cout << "Insert: " << *pendElem << " before " << *insertPos << " (Jacobsthal: " << jacNum << ")\n";
}
