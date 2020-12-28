#pragma once
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
	std::string getStringPosition() const;
	void setPosition(int line, int column);
	void setOrientation(char orientation);
	void setWord(std::string word);
private:
	std::string position; // position of the first letter of the word (position[0] = LINE, position[1] = column)
	char orientation; //vertical and horizontal
	std::string word;
};
