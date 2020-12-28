#pragma once
#include <vector>
#include <string>

class Word
{
public:
	Word(std::string word, std::string position, char orientation);
	Word();
	int getLine() const;
	int getColumn() const;
	char getOrientation() const;
	std::string getWord() const;
	void paintLetter(int indxOfLetterInWord);
	int nextAvailableLetterIndex(); //index of the next availabre letter in a word (the next one to be played)
	bool isItLastLetter(int indx);
private:
	std::string position;   // position of the first letter of the word (position[0] = LINE, position[1] = column)
	char orientation; //vertical and horizontal
	std::string word;
	std::vector<int> coloredLetters; //in the indx of the letter in the vector assumes 0 if it is not played and 1 if it is played
};
