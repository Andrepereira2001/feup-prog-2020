#include "Board.h"
// codes for setColor() function
#define WHITE 15 // letter white / background black
#define BLACK_LGREY 112 // letter black / backgroud light grey
#define GREEN 10 // letter green / backgroud black
#define STD_OUTPUT_HANDLE ((DWORD)-11)

void setColor(unsigned int color)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, color);
}

/*Constructor to load a created board
	@in - opened board file
*/
Board::Board(std::ifstream& in)
{
	std::string fileLine, position, word;
	char trash, orientation;

	if (!in.is_open()) // read board from file 
	{
		std::cerr << "file error" << std::endl;
		exit(1);
	}
	getline(in, fileLine);
	std::istringstream fileLineSS(fileLine);
	fileLineSS >> nLines >> trash >> nColumns;

	while (std::getline(in, fileLine))
	{
		std::istringstream fileLineSS(fileLine);
		if (fileLineSS >> position >> orientation >> word)
		{
			Word word1(word, position, orientation);
			words.push_back(word1);
		}
		else break; // in case there is a 2d board at the end of the file it break the input
	}

	// write board in the class
	int line, column;
	std::pair <int, int> wordRepresentation;

	houses.resize(nLines, std::vector<House>(nColumns));  // create vector with board houses 
	for (int i = 0; i < words.size(); i++)				 // go trought all the words in the board
	{
		line = (int)words.at(i).getLine();
		column = (int)words.at(i).getColumn();
		for (int l = 0; l < words.at(i).getWord().length(); l++) // associate all letters from the word to a house
		{
			houses.at(line).at(column).letter = words.at(i).getWord().at(l);
			wordRepresentation.first = i;
			wordRepresentation.second = l;
			houses.at(line).at(column).wordIndex.push_back(wordRepresentation);
			if (words.at(i).getOrientation() == 'V')
				line += 1;
			else if (words.at(i).getOrientation() == 'H')
				column += 1;
		}
	}
}

Board::Board()
{
	nLines = 0;
	nColumns = 0;
}

int Board::getNLines() const
{
	return nLines;
}

int Board::getNColumns()const
{
	return nColumns;
}

char Board::getHouseLetter(int line, int column) const
{
	return houses.at(line).at(column).letter;
}

void Board::showBoard(std::ostream& out) const
{
	setColor(WHITE);
	out << "Gaming Board" << std::endl;
	setColor(BLACK_LGREY);
	out << "  ";
	for (int i = (int)'a'; i < (int)'a' + nColumns; i++)
	{
		out << (char)i << " ";
	}
	setColor(WHITE);
	for (int i = (int)'A'; i < (int)'A' + nLines; i++)
	{
		setColor(BLACK_LGREY);
		out << std::endl << (char)i << " ";
		setColor(WHITE);
		for (int l = 0; l < nColumns; l++)
		{
			if (houses.at(i - (size_t)'A').at(l).isPlayed)
			{
				setColor(10);
				out << houses.at(i - (size_t)'A').at(l).letter << " ";
				setColor(WHITE);
			}
			else
				out << houses.at(i - (size_t)'A').at(l).letter << " ";
		}
	}
	out << std::endl << std::endl;
}

/*BOARDMETHOD updates the variable is played and the vector coloredLetters of the words the letter belongs
	@letter - with the letter, line and column
*/
void Board::paintHouse(Letter letter)  //adicionei
{
	houses.at(letter.line).at(letter.column).isPlayed = true;
	words[houses.at(letter.line).at(letter.column).wordIndex.at(0).first].paintLetter(houses.at(letter.line).at(letter.column).wordIndex.at(0).second);
	if (houses.at(letter.line).at(letter.column).wordIndex.size() - 1 != 0) //if the letter belongs to two words updates the second word vector coloredLetters
	{
		words[houses.at(letter.line).at(letter.column).wordIndex.at(houses.at(letter.line).at(letter.column).wordIndex.size() - 1).first].paintLetter(houses.at(letter.line).at(letter.column).wordIndex.at(houses.at(letter.line).at(letter.column).wordIndex.size() - 1).second);
	}
}

/*BOARDMETHOD evaluates if the letter can be played
	@l - letter
RETURNS true if valid
RETURNS false if not valid*/
bool Board::validToPlay(char l)
{
	bool valid = false;
	for (int i = 0; i < nLines; i++)
	{
		for (int j = 0; j < nColumns; j++)
		{
			valid = false;
			//searches for the letters that are not played and are the same as the letter 
			if (houses.at(i).at(j).letter == l && houses.at(i).at(j).isPlayed == false)
			{
				valid = true;
			}
			//checks if the letter is the next one to be played in the words it belongs if it is not it can't be played
			if (valid)
			{
				if (houses.at(i).at(j).wordIndex.at(0).second == words[houses.at(i).at(j).wordIndex.at(0).first].nextAvailableLetterIndex()
					|| houses.at(i).at(j).wordIndex.at(houses.at(i).at(j).wordIndex.size() - 1).second == words[houses.at(i).at(j).wordIndex.at(houses.at(i).at(j).wordIndex.size() - 1).first].nextAvailableLetterIndex()
					|| words[houses.at(i).at(j).wordIndex.at(0).first].nextAvailableLetterIndex() == -1 //checks if is the last one (see on Word::nextAvailableLetterIndex)
					|| words[houses.at(i).at(j).wordIndex.at(houses.at(i).at(j).wordIndex.size() - 1).first].nextAvailableLetterIndex() == -1)
				{
					valid = true;
				}
				else
				{
					valid = false;
				}
			}
			//if it happens to be valid even if it is only one solution it can be played and it is returned true
			if (valid)
			{
				return valid;
			}
		}
	}
	return valid;
}


/*BOARDMETHOD evaluates if the letters of the word, before the letter,are already painted
or is the first one to be painted
or if it is already played
	@letter - letter, line and column
RETURNS 0 if it is not valid
RETURNS 1 if it is valid
RETURNS 2 if it is valid and is the last letter of the word
RETURNS 3 if the house belongs to two words and it is the last letter of those two words to be played*/
int Board::validToPaint(Letter letter)
{
	int valid = 1;
	if (houses.at(letter.line).at(letter.column).letter == ' ') //houses with no letter
	{
		valid = 0;
	}
	else if (houses.at(letter.line).at(letter.column).isPlayed == true) //verify if it is already played
	{
		valid = 1; //false
	}
	// verify if the letter is the next letter available of at least one of the words that it is included
	else if (houses.at(letter.line).at(letter.column).wordIndex.at(0).second ==
		words[houses.at(letter.line).at(letter.column).wordIndex.at(0).first].nextAvailableLetterIndex() ||
		houses.at(letter.line).at(letter.column).wordIndex.at(houses.at(letter.line).at(letter.column).wordIndex.size() - 1).second ==
		words[houses.at(letter.line).at(letter.column).wordIndex.at(houses.at(letter.line).at(letter.column).wordIndex.size() - 1).first].nextAvailableLetterIndex())
	{
		valid = 0; //true
		if (words[houses.at(letter.line).at(letter.column).wordIndex.at(0).first].isItLastLetter(words[houses.at(letter.line).at(letter.column).wordIndex.at(0).first].nextAvailableLetterIndex()) == true
			|| words[houses.at(letter.line).at(letter.column).wordIndex.at(houses.at(letter.line).at(letter.column).wordIndex.size() - 1).first].isItLastLetter(words[houses.at(letter.line).at(letter.column).wordIndex.at(houses.at(letter.line).at(letter.column).wordIndex.size() - 1).first].nextAvailableLetterIndex()) == true)
		{
			valid = 2; //if it is valid and is the last letter of the word
		}
		if (houses.at(letter.line).at(letter.column).wordIndex.size() - 1 != 0 && words[houses.at(letter.line).at(letter.column).wordIndex.at(0).first].isItLastLetter(words[houses.at(letter.line).at(letter.column).wordIndex.at(0).first].nextAvailableLetterIndex()) == true
			&& words[houses.at(letter.line).at(letter.column).wordIndex.at(houses.at(letter.line).at(letter.column).wordIndex.size() - 1).first].isItLastLetter(words[houses.at(letter.line).at(letter.column).wordIndex.at(houses.at(letter.line).at(letter.column).wordIndex.size() - 1).first].nextAvailableLetterIndex()) == true)
		{
			valid = 3;  //if the house belongs to two words and it is the last letter of those two words to be played
		}
	}
	return valid;
}
