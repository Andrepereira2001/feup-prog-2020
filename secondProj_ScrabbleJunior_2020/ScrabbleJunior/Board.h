#pragma once
#include "Word.h"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <iostream>

void setColor(unsigned int color);

struct Letter
{
	char letter = ' ';
	int line = 0;
	int column = 0;
};

struct House
{
	char letter = ' ';
	bool isPlayed = false;
	//vector with the size of how many words that the house belongs
	//pair (position of the word in the vector words, position of the letter in the word)
	std::vector< std::pair <int, int>> wordIndex;
};

class Board
{
public:
	Board(std::ifstream& in);
	Board();
	int getNLines() const;
	int getNColumns() const;
	char getHouseLetter(int line, int column) const;
	void showBoard(std::ostream& out = std::cout) const;
	void paintHouse(Letter letter);
	bool validToPlay(char l);
	int validToPaint(Letter letter);
private:
	std::vector<std::vector<House>> houses; // 2d vector that represents the gaming board
	std::vector<Word> words; // vector that contains all the words in the board
	int nLines, nColumns; // lines and columns of the board
};
