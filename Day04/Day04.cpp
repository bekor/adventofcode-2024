#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>
#include <set>

struct coords { size_t y; size_t x; };

std::vector<std::string> readFile(const std::string& name) {
	std::ifstream inputfile{};
	inputfile.open(name);
	const std::string delim = " ";
	std::string str;
	std::vector<std::string> input;
	while (std::getline(inputfile, str))
	{
		input.push_back(str);
	}
	return input;
}

auto collectCoordinatesOfCharacter(const auto& puzzle, char c) {
	std::vector<coords> positions;
	for (size_t row = 0; row < puzzle.size(); ++row) {
		for (size_t col = 0; col < puzzle.size(); ++col) {
			if (puzzle.at(row).at(col) == c) {
				positions.push_back({ row, col });
			}
		}
	}
	return positions;
}

const std::vector<std::tuple<int, int>> firstDiagonal{ {-1, -1}, {1, 1} };
const std::vector<std::tuple<int, int>> halfOfAllDirection{ {-3, 0}, {-3, 3}, {0, 3}, {3, 3} };
const std::string MASstr = "MAS";
const std::set<std::string> MSCross{ "MS", "SM" };

bool inBound(size_t current, int diff, int max) {
	int sum = diff + current;
	return sum >= 0 && sum < max;
}

bool isCharChainMatching(const coords& position,
	const std::tuple<int, int>& direction,
	const std::vector<std::string>& puzzle,
	std::string_view match)
{
	int yDir = std::get<0>(direction);
	int xDir = std::get<1>(direction);
	size_t nextY = position.y;
	size_t nextX = position.x;
	for (const auto& nextChar : match) {
		nextY += (yDir != 0 ? (yDir < 0 ? -1 : 1) : 0);
		nextX += (xDir != 0 ? (xDir < 0 ? -1 : 1) : 0);
		if (puzzle.at(nextY).at(nextX) != nextChar) {
			return false;
		}
	}
}

bool checkInBound(const coords& position,
	const std::tuple<int, int>& direction,
	const std::vector<std::string>& puzzle)
{
	int yDir = std::get<0>(direction);
	int xDir = std::get<1>(direction);
	if (!inBound(position.y, yDir, puzzle.size()) || !inBound(position.x, xDir, puzzle.at(0).size())) {
		return false;
	}
	return true;
}

int countValidWords(const coords& position, const std::vector<std::string>& puzzle) {
	int validWordCount = 0;
	for (auto direction : halfOfAllDirection) {
		if (checkInBound(position, direction, puzzle)){
			if (isCharChainMatching(position, direction, puzzle, MASstr)) {
				validWordCount++;
			}
		}
		auto reversDirection = std::make_tuple(std::get<0>(direction) * -1, std::get<1>(direction) * -1);
		if (checkInBound(position, reversDirection, puzzle)) {
			if (isCharChainMatching(position, reversDirection, puzzle, MASstr)) {
				validWordCount++;
			}
		}
	}
	return validWordCount;
}

bool checkDiagonal(const coords& position, 
				   const std::vector<std::string>& puzzle, 
				   const std::vector<std::tuple<int, int>>& diagonal) 
{
	std::string diagonalString = "";
	if (position.x == 0 || position.x == puzzle.at(0).size()
		|| position.y == 0 || position.y == puzzle.size())
		return false;
	for (auto direction : diagonal) {
		if (checkInBound(position, direction, puzzle)) {
			int yDir = std::get<0>(direction);
			int xDir = std::get<1>(direction);
			yDir += position.y;
			xDir += position.x;
			diagonalString += puzzle.at(yDir).at(xDir);
		}
	}
	if (MSCross.find(diagonalString) == MSCross.end()) {
		return false;
	}
	return true;
}

bool isCrossWord(const coords& position, const std::vector<std::string>& puzzle) {
	if (checkDiagonal(position, puzzle, firstDiagonal)) {
		std::vector<std::tuple<int, int>> reverseDiagonal;
		for (auto direction : firstDiagonal) {
			auto reverDirection = std::make_tuple(std::get<0>(direction) * -1, std::get<1>(direction));
			reverseDiagonal.push_back(reverDirection);
		}
		if (checkDiagonal(position, puzzle, reverseDiagonal)) {
			return true;
		}
	}
	return false;
}

int main()
{
	auto wordsPuzzle = readFile("input.txt");
	auto positionsOfX = collectCoordinatesOfCharacter(wordsPuzzle, 'X');
	auto positionsOfA = collectCoordinatesOfCharacter(wordsPuzzle, 'A');
	int countWords = 0;
	int crossWordCount = 0;
	for (auto position : positionsOfX) {
		countWords += countValidWords(position, wordsPuzzle);
	}
	for (auto position : positionsOfA) {
		if(isCrossWord(position, wordsPuzzle))
			crossWordCount++;
	}
	std::cout << "Xmas count: " << countWords << "\n";
	std::cout << "X-Mas count: " << crossWordCount << "\n";
}
