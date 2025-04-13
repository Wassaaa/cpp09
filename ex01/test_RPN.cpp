#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

struct TestCase
{
    std::string name;
    std::string expression;
    std::string expectedOutput;
    bool shouldThrow;
};

bool runTest(const TestCase &test)
{
    std::cout << "Running test: " << test.name << " - ";

    RPN rpn;
    std::string expression = test.expression;
    bool isPassed = false;

    try {
        // Capture cout output
        std::streambuf *old = std::cout.rdbuf();
        std::stringstream ss;
        std::cout.rdbuf(ss.rdbuf());

        rpn.calculateRPN(expression);

        // Restore cout
        std::cout.rdbuf(old);

        std::string result = ss.str();
        // Remove newline if present
        if (!result.empty() && result[result.length() - 1] == '\n') {
            result.erase(result.length() - 1);
        }

        if (test.shouldThrow) {
            std::cout << "FAILED: Expected exception but got result: " << result << std::endl;
            isPassed = false;
        }
        else if (result == test.expectedOutput) {
            std::cout << "PASSED" << std::endl;
            isPassed = true;
        }
        else {
            std::cout << "FAILED: Expected " << test.expectedOutput << ", got " << result << std::endl;
            isPassed = false;
        }
    }
    catch (const std::exception &e) {
        if (test.shouldThrow) {
            std::cout << "PASSED (Expected exception: " << e.what() << ")" << std::endl;
            isPassed = true;
        }
        else {
            std::cout << "FAILED: Unexpected exception: " << e.what() << std::endl;
            isPassed = false;
        }
    }

    return isPassed;
}

int main()
{
    std::vector<TestCase> tests = {{"Simple addition", "3 4 +", "7", false},
                                   {"Simple subtraction", "3 4 -", "-1", false},
                                   {"Simple multiplication", "3 4 *", "12", false},
                                   {"Simple division", "12 3 /", "4", false},
                                   {"Complex expression", "3 4 + 5 *", "35", false},
                                   {"Negative numbers", "3 -4 +", "-1", false},
                                   {"Division by zero", "3 0 /", "", true},
                                   {"Insufficient operands", "3 +", "", true},
                                   {"Too many operands", "3 4 5 +", "", true},
                                   {"Invalid token", "3 4 &", "", true},
                                   {"Empty expression", "", "", true}};

    int passed = 0;
    int total = tests.size();

    std::cout << "Running " << total << " tests...\n\n";

    for (const auto &test : tests) {
        if (runTest(test)) {
            passed++;
        }
    }

    std::cout << "\nTest run completed. " << passed << " of " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1; // Return non-zero exit code if any tests failed
}