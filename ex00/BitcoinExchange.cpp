#include "BitcoinExchange.hpp"
#include <fstream>
#include <regex>
#include <iostream>

BitcoinExchange::BitcoinExchange()
{}

BitcoinExchange::BitcoinExchange(BitcoinExchange &other)
    : _priceData(other._priceData)
{}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange &other)
{
    if (this != &other) {
        this->_priceData = other._priceData;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{}

void BitcoinExchange::parseData()
{
    const std::string dataFilePath = "../../ex00/data.csv";
    std::ifstream dataFile(dataFilePath);
    if (!dataFile.is_open())
        throw std::runtime_error("Error: Could not open file " + dataFilePath);

    std::string line;
    std::smatch matches;
    // YYYY-MM-DD,value
    std::regex linePattern(R"(^(\d{4}-\d{1,2}-\d{1,2}),([+]?(?:\d+\.?\d*|\.\d+))$)");
    while (getline(dataFile, line)) {
        if (line == "date,exchange_rate" || line.empty())
            continue;
        if (std::regex_match(line, matches, linePattern)) {
            std::string date = matches[1].str();
            double price = std::stod(matches[2].str());
            if (_priceData.find(date) != _priceData.end())
                throw std::runtime_error("duplicate data, fix your data");
            _priceData[date] = price;
        }
    }
}

double BitcoinExchange::getPrice(const std::string &date) const
{
    auto it = _priceData.find(date);
    if (it != _priceData.end()) {
        return it->second;
    }

    it = _priceData.lower_bound(date);
    if (it == _priceData.begin() && date < it->first) {
        throw std::runtime_error("Error: no data for date " + date);
    }

    if (it == _priceData.end() || date < it->first) {
        --it;
    }

    return it->second;
}

void BitcoinExchange::parseInput(std::string &file)
{
    std::ifstream inputFile(file);
    if (!inputFile.is_open())
        throw std::runtime_error("Error: Could not open file " + file);

    std::string line;
    std::smatch matches;
    // YYYY-MM-DD | coins
    std::regex linePattern(R"(^(\d{4}-\d{1,2}-\d{1,2}) \| ([-+]?(?:\d+\.?\d*|\.\d+))$)");
    while (getline(inputFile, line)) {
        if (line == "date | value" || line.empty())
            continue;
        if (std::regex_match(line, matches, linePattern)) {
            std::string date = matches[1].str();
            try {

                double coins = std::stod(matches[2].str());
                if (coins < 0) {
                    std::cout << "Error: not a positive number" << std::endl;
                    continue;
                }
                if (coins > 1000) {
                    std::cout << "Error: too large a number." << std::endl;
                    continue;
                }
                try {
                    double price = getPrice(date);
                    double sum = coins * price;
                    std::cout << date << " => " << price << " * " << coins << " = " << sum
                              << std::endl;
                }
                catch (const std::exception &e) {
                    std::cout << e.what() << std::endl;
                }
            }
            catch (const std::exception &e) {
                std::cout << "Error: invalid number format." << std::endl;
            }
        }
        else {
            std::cout << "Error: bad input => " << line << std::endl;
        }
    }
}
