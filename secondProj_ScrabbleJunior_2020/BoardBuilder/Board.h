#pragma once
#include "Word.h"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

void setColor(unsigned int color);

struct House
{
	char letter = ' ';
	bool isPlayed = false;
	//vector with the size of how many words that house belongs
	//pair (position of the word in the vector words, position of the letter in the word)
	std::vector< std::pair <int, int>> wordIndex;
};

class Board
{
public:
	Board(int nLines, int nColumns);
	Board();
	int getNLines() const;
	int getNColumns() const;
	char getHouseLetter(int line, int column) const;
	int getMaxWords() const;
	int getMaxLetters() const;
	int getNLetters() const;
	int getNWords() const;
	Word getHouseWord(int line,int column,char orientation) const;
	void addWord(Word word);
	void removeWord(Word word);
	int isValidAddWord(const Word& word);
	void showBoard(std::ostream& out = std::cout) const;
	void saveBoard(std::ofstream& out) const;
private:
	void writePossibelWords();
	std::vector<std::string> possibelWords; // dictionary words
	std::vector<std::vector<House>> houses; // 2d vector that represents the gaming board
	static int nWords, nLetters; // actual number of letters/words int the board 
	int  maxWords=0, maxLetters=0; // expected max/min letters of the created board
	std::vector<Word> words; // vector that contains all the words in the board
	int nLines, nColumns; // lines and columns of the board
};
