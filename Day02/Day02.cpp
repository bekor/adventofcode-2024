#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>

std::vector<int> spliToInt(const std::string& s, const std::string& delimiter) {
    std::vector<int> tokens;
    size_t last = 0;
    size_t next = 0;
    while ((next = s.find(delimiter, last)) != std::string::npos) {
        tokens.push_back(std::stoi(s.substr(last, next - last)));
        last = next + 1;
    }
    tokens.push_back(std::stoi(s.substr(last)));
    return tokens;
}

using matrix = std::vector<std::vector<int>>;

bool isStepSmallerThanThree(int a, int b) {
    return std::abs(a - b) > 3;
}

bool isMovingInDirection(const auto& report, std::function<bool(int, int)> compare) {
    int previous = report.at(0);
    for (size_t i = 1; i < report.size(); ++i) {
        int current = report.at(i);
        if (compare(previous, current)) {
            if (isStepSmallerThanThree(previous, current)) {
                return false;
            }
        }
        else {
            return false;
        }
        previous = current;
    }
    return true;
}

bool isMovingInDirectionWDampening(const auto& report, std::function<bool(int, int)> compare) {
    int previous = report.at(0);
    int countDump = 0;
    for (size_t i = 1; i < report.size(); ++i) {
        int current = report.at(i);
        if (compare(previous, current)) {
            if (isStepSmallerThanThree(previous, current)) {
                countDump++;
                if (i < 2) {
                    previous = report.at(i - 1);
                }
                else {
                    previous = report.at(i - 2);
                }
                
            }
            else {
                previous = current;
            }
        }
        else if (previous == current) {
            countDump++;
        }
        else {
            countDump++;
            previous = report.at(i-2);
        }
    }
    return 1 >= countDump ;
}


uint32_t countSafeReport(const auto& reports) {
    uint32_t safereports = 0;
    for (const auto& report : reports) {
        if (report.at(0) < report.at(1)) {
            if (isMovingInDirection(report, std::less<int>{})) {
                safereports++;
            }
        }
        else if(report.at(0) > report.at(1)){
            if (isMovingInDirection(report, std::greater<int>{})) {
                safereports++;
            }
        }
    }
    return safereports;
}

uint32_t countSafeReportDampened(const auto& reports) {
    uint32_t safereports = 0;
    for (const auto& report : reports) {
        auto first = report.at(0);
        auto second = report.at(1);
        if (first == second) {
            second = report.at(3);
        }

        if (first < second) {
            if (isMovingInDirectionWDampening(report, std::less<int>{})) {
                safereports++;
            }
        }
        else if (first > second) {
            if (isMovingInDirectionWDampening(report, std::greater<int>{})) {
                safereports++;
            }
        }
    }
    return safereports;
}

int main()
{
    std::ifstream inputfile{};
    inputfile.open("input.txt");
    const std::string delim = " ";
    std::string str;
    matrix reports;
    while (std::getline(inputfile, str))
    {
        reports.push_back(spliToInt(str, delim));
    }

    auto safeReportcount = countSafeReport(reports);
    auto dmapenedReportcount = countSafeReportDampened(reports);
    std::cout << "Safe count " << safeReportcount << std::endl;
    std::cout << "Dampened Safe count " << dmapenedReportcount << std::endl;
}