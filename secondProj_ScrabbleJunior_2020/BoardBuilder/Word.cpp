#include "Word.h"

Word::Word(std::string word, std::string position, char orientation)
{
	this->word = word;
	this->position = position;
	this->orientation = orientation;
}

Word::Word()
{
	word = "";
	position = "";
	orientation = ' ';
}

int Word::getLine() const
{
	return (position.at(0) - (int)'A');
}

int Word::getColumn() const
{
	return (position.at(1) - (int)'a');
}

/*WORDMETHOD 
RETURNS the word position in string (example: Ab)*/
std::string Word::getStringPosition() const
{
	return position;
}

std::string Word::getWord() const
{
	return word;
}

char Word::getOrientation() const
{
	return orientation;
}

void Word::setWord(std::string word)
{
	this->word = word;
}

void Word::setPosition(int line, int column)
{
	char charLine = (char)(line + (int)'A');
	char charColumn = (char)(column + (int)'a');
	std::string finalPosition(1, charLine);
	position = finalPosition + charColumn;
}

void Word::setOrientation(char orientation)
{
	this->orientation = orientation;
}