#include "Word.h"

Word::Word(std::string word, std::string position, char orientation)
{
	this->word = word;
	this->position = position;
	this->orientation = orientation;
	for (int i = 0; i < word.size(); i++)
	{
		coloredLetters.push_back(0);
	}
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

std::string Word::getWord() const
{
	return word;
}

char Word::getOrientation() const
{
	return orientation;
}

/*WORDMETHOD by painting the letter puts the indx of the letter in the word in vector coloredLetters=1*/
void Word::paintLetter(int indxOfLetterInWord)
{
	coloredLetters.at(indxOfLetterInWord) = 1;
}

/*WORDMETHOD
RETURNS index of the next available letter on the word that is not played (=that can be played)*/
int Word::nextAvailableLetterIndex()
{
	int indx = -2;
	for (int i = 0; i < coloredLetters.size(); i++)
	{
		if (coloredLetters.at(i) == 0)
		{
			indx = i;
			break;
		}
	}
	return indx;
}

/*WORDMETHOD
RETURNS true if the indx given corresponds to the last available letter of the word
RETURNS false if don't*/
bool Word::isItLastLetter(int indx)
{
	bool lastAvailable = true;
	for (int i = indx + 1; i < coloredLetters.size(); i++)
	{
		if (coloredLetters.at(i) == 1)
		{
			lastAvailable = true;
		}
		else
		{
			lastAvailable = false;
			break;
		}
	}
	return lastAvailable;
}
