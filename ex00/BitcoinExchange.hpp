#pragma once
#include <string>
#include <map>

class BitcoinExchange
{
public:
    BitcoinExchange();
    BitcoinExchange(BitcoinExchange &other);
    BitcoinExchange &operator=(BitcoinExchange &other);
    ~BitcoinExchange();

    void parseData();
    double getPrice(const std::string &date) const;
    void parseInput(std::string &file);

private:
    std::map<std::string, double> _priceData;
};
