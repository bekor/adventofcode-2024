#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>
#include <regex>
#include <array>

std::vector<std::string> filterMulOperations(const std::string& input) {
    std::vector<std::string> operations;
    std::regex mulFinder("mul\\([0-9]{1,3},[0-9]{1,3}\\)|do\\(\\)|don\'t\\(\\)");
    std::smatch operationMatch;
    std::string::const_iterator searchStart(input.cbegin());
    while (std::regex_search(searchStart, input.cend(), operationMatch, mulFinder))
    {
        //std::cout << (searchStart == input.cbegin() ? "" : " ") << operationMatch[0];
        operations.push_back(operationMatch[0]);
        searchStart = operationMatch.suffix().first;
    }
    return operations;
}

static std::array<int, 2> extractNumbers(const std::string& operation) {
    int first = 0, second = 0;
    std::regex mulFinder("[0-9]{1,3}");
    std::smatch operationMatch;
    std::string::const_iterator searchStart(operation.cbegin());
    while (std::regex_search(searchStart, operation.cend(), operationMatch, mulFinder))
    {
        //std::cout << (searchStart == operation.cbegin() ? "" : " ") << operationMatch[0];
        if (searchStart == operation.cbegin()) {
            first = std::stoi(operationMatch[0]);
        }
        else {
            second = std::stoi(operationMatch[0]);
        }
        searchStart = operationMatch.suffix().first;
    }
    return { first, second };
}

int main()
{
    std::ifstream inputfile{};
    inputfile.open("input.txt");
    const std::string delim = " ";
    std::string str;
    std::vector<std::string> mulOperations;
    long result = 0;
    bool scipp = false;
    while (std::getline(inputfile, str))
    {
        mulOperations = filterMulOperations(str);
        for (auto operation : mulOperations) {
            
            if (operation == "don't()") {
                scipp = true;
                continue;
            }
            else if (operation == "do()") {
                scipp = false;
                continue;
            }
            if (!scipp) {
                auto [a, b] = extractNumbers(operation);
                result += a * b;
            }
        }
    }

    std::cout << "wee: " << result << std::endl;
}
