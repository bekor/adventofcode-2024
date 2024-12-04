#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <span>
#include <algorithm>
#include <map>

std::pair<int, int> spliToIntPair(const std::string& s, const std::string& delimiter) {
    std::pair<int, int> tokens;
    size_t last = 0; 
    size_t next = 0; 
    while ((next = s.find(delimiter, last)) != std::string::npos) {
        tokens.first = std::stoi(s.substr(last, next - last));
        last = next + 1; 
    }
    tokens.second = std::stoi(s.substr(last));
    return tokens;
}
//
//template <typename T>
//concept HasSwap = requires (T a, T b) {
//
//}

void calculateDifference(auto& leftList, auto& rightList) {
    std::sort(leftList.begin(), leftList.end());
    std::sort(rightList.begin(), rightList.end());
    int difference = 0;
    for (size_t i = 0; i < leftList.size(); ++i) {
        difference += std::abs(leftList.at(i) - rightList.at(i));
    }
    std::cout << "Difference: " << difference << std::endl;
}

void calculateSimilarityScore(auto& leftList, auto& rightList) {
    std::map<int, int> occurances;
    for (const auto& i : rightList) {
        occurances[i]++;
    }
    long score = 0;
    for (const auto i : leftList) {
        if (occurances.find(i) != occurances.end()) {
            score += i * occurances[i];
        }
    }
    std::cout << "Similarity Score: " << score << std::endl;
}

int main()
{
    std::ifstream inputfile{};
    inputfile.open("input.txt");
    const std::string delim = "   ";
    std::string str;
    std::vector<int> leftList;
    std::vector<int> rightList;
    while (std::getline(inputfile, str))
    {
        auto [l, r] = spliToIntPair(str, delim);
        leftList.push_back(l);
        rightList.push_back(r);
    }
    calculateSimilarityScore(leftList, rightList);
    calculateDifference(leftList, rightList);
    
}

