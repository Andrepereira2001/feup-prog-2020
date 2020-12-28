#include "Board.h"
#define MAXWORDSMULTIPLIER 0.2 // value that multiplied by number os tiles will give an average maxWords number
#define MAXLETTERSMULTIPLIER 0.45 // value that multiplied by number os tiles will give an average maxLetters numbe
// codes for setColor() function
#define WHITE 15 // letter white / background black
#define BLACK_LGREY 112 // letter black / backgroud light grey
#define STD_OUTPUT_HANDLE ((DWORD)-11)

int Board::nWords = 0, Board::nLetters = 0;

void setColor(unsigned int color)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, color);
}

/*constructor to creat board from zero
	@nLines - board lines
	@nColumns - board columns
*/
Board::Board(int nLines, int nColumns)
{
	this->nLines = nLines; this->nColumns = nColumns;
	houses.resize(nLines, std::vector<House>(nColumns)); // create vector with board houses 
	maxWords = (int)ceil((double)nLines * (double)nColumns * MAXWORDSMULTIPLIER);	// expected number of words 
	maxLetters = (int)ceil((double)nLines * (double)nColumns * MAXLETTERSMULTIPLIER);	// expected number of letters 
	writePossibelWords(); // get words in dictionary
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

int Board::getMaxWords() const
{
	return maxWords;
}

int Board::getMaxLetters() const
{
	return maxLetters;
}

int Board::getNLetters() const
{
	return nLetters;
}

int Board::getNWords() const
{
	return nWords;
}

/*Board method that give us the word that is in one specified house
	@line - line of the house
	@column - column of the house
	@orientation - orientation of the word that we want
	@return - a copy of the word if there is one, else return a word ""
*/
Word Board::getHouseWord(int line, int column, char orientation) const
{
	Word word;
	if (houses.at(line).at(column).letter != ' ')
	{
		for (size_t i = 0; i < houses.at(line).at(column).wordIndex.size(); i++)
		{
			if (words.at(houses.at(line).at(column).wordIndex.at(i).first).getOrientation() == orientation)
				word = words.at(houses.at(line).at(column).wordIndex.at(i).first);
		}
	}
	return word;
}

/*Board method that add a word to the board
	@word - word that will be added
	*/
void Board::addWord(Word word) //Creat board useful function
{
	int line = word.getLine();
	int column = word.getColumn();
	std::pair <int, int> wordRepresentation;

	words.push_back(word);
	for (size_t i = 0; i < word.getWord().length(); i++) // go trow all the given word
	{
		if (houses.at(line).at(column).letter == ' ')
			nLetters += 1;
		houses.at(line).at(column).letter = word.getWord().at(i);
		wordRepresentation.first = (int)words.size() - 1;
		wordRepresentation.second = i;
		houses.at(line).at(column).wordIndex.push_back(wordRepresentation);
		if (word.getOrientation() == 'V')
			line += 1;
		else if (word.getOrientation() == 'H')
			column += 1;
	}
	nWords += 1;

}

/*Board method that removes a word from the board
	@word - word that must be removed
	*/
void Board::removeWord(Word word)
{
	int wordIndex = 1; // word index in vector words
	for (size_t i = 0; i < words.size(); i++)
	{
		if (words.at(i).getWord() == word.getWord())
			wordIndex = i;
	}

	// start removing word from the 2d vector houses
	if (word.getOrientation() == 'V')
	{
		for (size_t i = word.getLine(); i < word.getLine() + word.getWord().length(); i++)
		{
			if (houses.at(i).at(word.getColumn()).wordIndex.size() == 2)
			{
				for (size_t l = 0; l < 2; l++)
				{
					if (houses.at(i).at(word.getColumn()).wordIndex.at(l).first == wordIndex)
					{
						houses.at(i).at(word.getColumn()).wordIndex.erase(houses.at(i).at(word.getColumn()).wordIndex.begin() + l);
						break;
					}
				}

			}
			else if (houses.at(i).at(word.getColumn()).wordIndex.size() == 1)
			{
				houses.at(i).at(word.getColumn()).letter = ' ';
				houses.at(i).at(word.getColumn()).wordIndex.erase(houses.at(i).at(word.getColumn()).wordIndex.begin());
				nLetters -= 1;
			}
		}
	}
	else if (word.getOrientation() == 'H')
	{
		for (size_t i = word.getColumn(); i < word.getColumn() + word.getWord().length(); i++)
		{
			if (houses.at(word.getLine()).at(i).wordIndex.size() == 2)
			{
				for (size_t l = 0; l < 2; l++)
				{
					if (houses.at(word.getLine()).at(i).wordIndex.at(l).first == wordIndex)
					{
						houses.at(word.getLine()).at(i).wordIndex.erase(houses.at(word.getLine()).at(i).wordIndex.begin() + l);
						break;
					}
				}
			}
			else
			{
				houses.at(word.getLine()).at(i).letter = ' ';
				houses.at(word.getLine()).at(i).wordIndex.erase(houses.at(word.getLine()).at(i).wordIndex.begin());
				nLetters -= 1;
			}
		}
	}

	words.at(wordIndex).setWord("");
	nWords -= 1;
}

/*Board method to check if is valid to add a word to the board
	@word - word that going to be added
	@return - 0 if it is not valid
	1 - if it is valid and there are no alterations of words
	2 - if is valid and there are alterations in the words (HARD TESTS RETURN)
	*/
int Board::isValidAddWord(const Word& word)
{
	int line = word.getLine();
	int column = word.getColumn();
	int isValid = 1; // "return value works like a 'bool' with 3 possibel values"
	std::string totalWord;
	if (word.getWord().length() <= 1)
	{
		std::cout << "Word is to small" << std::endl;
		isValid = 0;
	}
	if (word.getOrientation() != 'V' && word.getOrientation() != 'H')
	{
		std::cout << "Word orientation not valid" << std::endl;
		isValid = 0;
	}
	else if (column >= nColumns || line >= nLines) // invalid position
	{
		isValid = 0;
		std::cout << "Word position out of range" << std::endl;
	}
	else if (column < 0 || line < 0) // invalid position
	{
		isValid = 0;
		std::cout << "Word position out of range" << std::endl;
	}
	else if (word.getOrientation() == 'H') // word bigger then the board
	{
		if (column + word.getWord().length() > (size_t) nColumns)
		{
			isValid = 0;
			std::cout << "Word bigger then the board" << std::endl;
		}
		else if (getHouseWord(column, line, 'H').getWord() != "") // given position already have a horizontal word
		{
			if (getHouseWord(column, line, 'H').getWord().length() + getHouseWord(column, line, 'H').getColumn() > column + word.getWord().length()) // word given to test is small then the word that take that place
			{
				std::cout << "The selected houses already have a bigger word" << std::endl;
				isValid = 0;
			}
		}
	}
	else if (word.getOrientation() == 'V')
	{
		if (line + word.getWord().length() > (size_t)nLines)
		{
			isValid = 0;
			std::cout << "Word bigger then the board" << std::endl;
		}
		else if (getHouseWord(column, line, 'V').getWord() != "") // given position already have a vertical word
		{
			if (getHouseWord(column, line, 'V').getWord().length() + getHouseWord(column, line, 'V').getColumn() > column + word.getWord().length()) // word given to test is small then the word that take that place
			{
				std::cout << "The selected houses already have a bigger word" << std::endl;
				isValid = 0;
			}
		}
	}
	if (std::find(possibelWords.begin(), possibelWords.end(), word.getWord()) == possibelWords.end()) // word not in the dictionary
	{
		isValid = 0;
		std::cout << "Word not in the dictionary" << std::endl;
	}
	for (size_t i = 0; i < words.size(); i++)
	{
		if (word.getWord() == words.at(i).getWord())
		{
			isValid = 0;
			std::cout << "Word already in the board" << std::endl;
		}

	}
	if (isValid == 1)
	{
		for (size_t i = 0; i < word.getWord().length(); i++)   // check all the houses where the new word will be placed
		{
			if (houses.at(line).at(column).letter != word.getWord().at(i) && houses.at(line).at(column).letter != ' ')// diferent letters in the same house
			{
				isValid = 0;
			}
			else // the new word will be placed in top a existing word in the board
			{
				for (size_t l = 0; l < houses.at(line).at(column).wordIndex.size(); l++)
				{
					if (words.at(houses.at(line).at(column).wordIndex.at(l).first).getOrientation() == word.getOrientation())
					{
						if (houses.at(line).at(column).wordIndex.at(l).second == 0)
						{
							if (word.getOrientation() == 'H')
							{
								if (words.at(houses.at(line).at(column).wordIndex.at(l).first).getWord().length() + column <= word.getColumn() + word.getWord().length())
									isValid = 2;
							}
							else if (word.getOrientation() == 'V')
							{
								if (words.at(houses.at(line).at(column).wordIndex.at(l).first).getWord().length() + line <= word.getLine() + word.getWord().length())
									isValid = 2;
							}

						}
					}
				}
			}
			if (word.getOrientation() == 'H')
				column += 1;
			else if (word.getOrientation() == 'V')
				line += 1;
		}
	}
	// BEGIN THE HARDER TESTS
	// test YXXXXXXXXXZZZZ   // x being the tested word. z and y words that are in the board
	//		Y		  
	if (isValid == 1)
	{
		if (word.getOrientation() == 'H') //horizontal words
		{
			if ((int)word.getColumn() - 1 >= 0) // if behind the first position still have board 
			{
				if (houses.at(word.getLine()).at((size_t)word.getColumn() - 1).letter != ' ')
				{
					if (houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.size() == 2) // if letter represent 2 words
					{
						for (size_t i = 0; i < 2; i++) // get word in horizontal that is in that house
						{
							if (words.at(houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.at(i).first).getOrientation() == 'H')
								totalWord = (words.at(houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.at(i).first).getWord()).
								std::string::substr(words.at(houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.at(i).first).getColumn(),
								(size_t)word.getColumn() - words.at(houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.at(i).first).getColumn());
						}
					}
					else if (words.at(houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.at(0).first).getOrientation() == 'H')// if letter represent 1 word horizontal
					{
						totalWord = (words.at(houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.at(0).first).getWord()).
							std::string::substr(words.at(houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.at(0).first).getColumn(),
							(size_t)word.getColumn() - words.at(houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.at(0).first).getColumn());
					}
					else if (words.at(houses.at(word.getLine()).at((size_t)word.getColumn() - 1).wordIndex.at(0).first).getOrientation() == 'V')// if letter represent 1 word vertical
					{
						totalWord = houses.at(word.getLine()).at((size_t)word.getColumn() - 1).letter;
					}

				}
				else totalWord = "";
			}
			else totalWord = "";

			totalWord += word.getWord();

			if (word.getColumn() + word.getWord().length() < (size_t)nColumns) // if after last position still have board
			{
				if (houses.at(word.getLine()).at(word.getColumn() + word.getWord().length()).letter != ' ')
				{
					totalWord = word.getWord(); // first half of the word
					if (houses.at(word.getLine()).at(word.getColumn() + word.getWord().length()).wordIndex.size() == 2) // if that letter represent 2 words
					{
						for (size_t i = 0; i < 2; i++) // get word in horizontal that is in that house
						{
							if (words.at(houses.at(word.getLine()).at(word.getColumn() + word.getWord().length()).wordIndex.at(i).first).getOrientation() == 'H')
								totalWord += words.at(houses.at(word.getLine()).at(word.getColumn() + word.getWord().length()).wordIndex.at(i).first).getWord().
								std::string::substr(word.getColumn() + word.getWord().length()); // join both halfs
						}
					}
					else if (words.at(houses.at(word.getLine()).at(word.getColumn() + word.getWord().length()).wordIndex.at(0).first).getOrientation() == 'H') // if that letter represent 1 word horizontal
					{
						totalWord += words.at(houses.at(word.getLine()).at(word.getColumn() + word.getWord().length()).wordIndex.at(0).first).getWord().
							std::string::substr(word.getColumn() + word.getWord().length()); // join word + remaining board word
					}
					else if (words.at(houses.at(word.getLine()).at(word.getColumn() + word.getWord().length()).wordIndex.at(0).first).getOrientation() == 'V') // if that letter represent 1 word vertical
					{
						totalWord += houses.at(word.getLine()).at(word.getColumn() + word.getWord().length()).letter; // join word + letter
					}
				}
			}
			if (totalWord != word.getWord())
			{
				if (std::find(possibelWords.begin(), possibelWords.end(), totalWord) == possibelWords.end()) // check if the word exist
					isValid = 0;
				else isValid = 2;
			}
		}
		if (word.getOrientation() == 'V') // vertical words
		{
			if ((int)word.getLine() - 1 >= 0) // it is in the board ranges
			{
				if (houses.at((size_t)word.getLine() - 1).at(word.getColumn()).letter != ' ')// if behind the first position there is a letter
				{
					if (houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.size() == 2)
					{
						for (size_t i = 0; i < 2; i++) // get word in vertical
						{
							if (words.at(houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.at(i).first).getOrientation() == 'V')
								totalWord = ((words.at(houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.at(i).first).getWord()).
									std::string::substr(words.at(houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.at(i).first).getLine(),
									(size_t)word.getLine() - words.at(houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.at(i).first).getLine())); // substring to join the words correctly
						}
					}
					else if (words.at(houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.at(0).first).getOrientation() == 'V')// if letter represent 1 word vertical
					{
						totalWord = ((words.at(houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.at(0).first).getWord()).
							std::string::substr(words.at(houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.at(0).first).getLine(),
							(size_t)word.getLine() - words.at(houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.at(0).first).getLine())); // join word + remaining board word
					}
					else if (words.at(houses.at((size_t)word.getLine() - 1).at(word.getColumn()).wordIndex.at(0).first).getOrientation() == 'H')// if letter represent 1 word horizontal
					{
						totalWord = houses.at((size_t)word.getLine() - 1).at(word.getColumn()).letter;// join word + letter
					}
				}
				else totalWord = "";
			}
			else totalWord = "";

			totalWord += word.getWord();

			if (word.getLine() + word.getWord().length() < (size_t)nLines)
			{
				if (houses.at(word.getLine() + word.getWord().length()).at(word.getColumn()).letter != ' ')
				{
					totalWord = word.getWord(); // first half of the word
					if (houses.at(word.getLine() + word.getWord().length()).at(word.getColumn()).wordIndex.size() == 2)
					{
						for (size_t i = 0; i < 2; i++) // get word in vertical
						{
							if (words.at(houses.at(word.getLine() + word.getWord().length()).at(word.getColumn()).wordIndex.at(i).first).getOrientation() == 'V')
								totalWord += words.at(houses.at(word.getLine() + word.getWord().length()).at(word.getColumn()).wordIndex.at(i).first).getWord().
								std::string::substr(word.getLine() + word.getWord().length()); // join both halfs
						}
					}
					else if (words.at(houses.at(word.getLine() + word.getWord().length()).at(word.getColumn()).wordIndex.at(0).first).getOrientation() == 'V') // if that letter represent 1 word vertical
					{
						totalWord += words.at(houses.at(word.getLine() + word.getWord().length()).at(word.getColumn()).wordIndex.at(0).first).getWord().
							std::string::substr(word.getLine() + word.getWord().length()); // join both halfs
					}
					else if (words.at(houses.at(word.getLine() + word.getWord().length()).at(word.getColumn()).wordIndex.at(0).first).getOrientation() == 'H') // if that letter represent 1 word horizontal
					{
						totalWord += houses.at(word.getLine() + word.getWord().length()).at(word.getColumn()).letter;
					}
				}
			}
			if (totalWord != word.getWord())
			{
				if (std::find(possibelWords.begin(), possibelWords.end(), totalWord) == possibelWords.end()) // check if the word exist
					isValid = 0;
				else isValid = 2;
			}
		}
	}
	// HARDER TESTS 2
	// test xxxxxxx     // x being the tested word. z and y words that are in the board
	//		zzz   y
	//			  y
	if (isValid != 0)
	{
		if (word.getOrientation() == 'H')
		{
			for (size_t i = word.getColumn(); i < word.getColumn() + word.getWord().length(); i++) // check all the word
			{
				if (houses.at(word.getLine()).at(i).letter == ' ') // there isn't any letter in the position we are checking
				{
					if ((int)word.getLine() - 1 >= 0) // is in the range of the board
					{
						if (houses.at((size_t)word.getLine() - 1).at(i).letter != ' ') // there is a letter in upper house
						{
							if (houses.at((size_t)word.getLine() - 1).at(i).wordIndex.size() == 2)
							{
								for (int l = 0; l < 2; l++)
								{
									if (words.at(houses.at((size_t)word.getLine() - 1).at(i).wordIndex.at(l).first).getOrientation() == 'V')
										totalWord = words.at(houses.at((size_t)word.getLine() - 1).at(i).wordIndex.at(l).first).getWord();
								}
							}
							else if (words.at(houses.at((size_t)word.getLine() - 1).at(i).wordIndex.at(0).first).getOrientation() == 'V')
								totalWord = words.at(houses.at((size_t)word.getLine() - 1).at(i).wordIndex.at(0).first).getWord();
							else if (words.at(houses.at((size_t)word.getLine() - 1).at(i).wordIndex.at(0).first).getOrientation() == 'H')
								totalWord = houses.at((size_t)word.getLine() - 1).at(i).letter;
						}
						else totalWord = "";
					}
					else totalWord = "";

					totalWord += word.getWord().at(i - (size_t)word.getColumn());

					if ((int)word.getLine() + 1 < nLines)
					{
						if (houses.at((size_t)word.getLine() + 1).at(i).letter != ' ') // there is a letter in the lower house
						{
							if (houses.at((size_t)word.getLine() + 1).at(i).wordIndex.size() == 2)
							{
								for (int l = 0; l < 2; l++)
								{
									if (words.at(houses.at((size_t)word.getLine() + 1).at(i).wordIndex.at(l).first).getOrientation() == 'V')
										totalWord += words.at(houses.at((size_t)word.getLine() + 1).at(i).wordIndex.at(l).first).getWord();

								}
							}
							else if (words.at(houses.at((size_t)word.getLine() + 1).at(i).wordIndex.at(0).first).getOrientation() == 'V')
								totalWord += words.at(houses.at((size_t)word.getLine() + 1).at(i).wordIndex.at(0).first).getWord();
							else if (words.at(houses.at((size_t)word.getLine() + 1).at(i).wordIndex.at(0).first).getOrientation() == 'H')
								totalWord += houses.at((size_t)word.getLine() + 1).at(i).letter;
						}
					}
					if (totalWord.length() != 1)
					{
						if (std::find(possibelWords.begin(), possibelWords.end(), totalWord) == possibelWords.end()) // check if the word exist
							isValid = 0;
						else isValid = 2;
					}
				}
			}
		}
		if (word.getOrientation() == 'V')
		{
			for (size_t i = word.getLine(); i < word.getLine() + word.getWord().length(); i++) // check all the word
			{
				if (houses.at(i).at(word.getColumn()).letter == ' ') // there isn't any letter in the position we are checking
				{
					if ((int)word.getColumn() - 1 >= 0) // is in the range of the board
					{
						if (houses.at(i).at((size_t)word.getColumn() - 1).letter != ' ') // there is a letter in the left house
						{
							if (houses.at(i).at((size_t)word.getColumn() - 1).wordIndex.size() == 2)
							{
								for (int l = 0; l < 2; l++)
								{
									if (words.at(houses.at(i).at((size_t)word.getColumn() - 1).wordIndex.at(l).first).getOrientation() == 'H')
										totalWord = words.at(houses.at(i).at((size_t)word.getColumn() - 1).wordIndex.at(l).first).getWord();
								}
							}
							else if (words.at(houses.at(i).at((size_t)word.getColumn() - 1).wordIndex.at(0).first).getOrientation() == 'H')
								totalWord = words.at(houses.at(i).at((size_t)word.getColumn() - 1).wordIndex.at(0).first).getWord();
							else if (words.at(houses.at(i).at((size_t)word.getColumn() - 1).wordIndex.at(0).first).getOrientation() == 'V')
								totalWord = houses.at(i).at((size_t)word.getColumn() - 1).letter;
						}
						else totalWord = "";
					}
					else totalWord = "";

					totalWord += word.getWord().at(i - (size_t)word.getLine());

					if ((int)word.getColumn() + 1 < nColumns)
					{
						if (houses.at(i).at((size_t)word.getColumn() + 1).letter != ' ') // there is a letter in the lower house
						{
							if (houses.at(i).at((size_t)word.getColumn() + 1).wordIndex.size() == 2)
							{
								for (int l = 0; l < 2; l++)
								{
									if (words.at(houses.at(i).at((size_t)word.getColumn() + 1).wordIndex.at(l).first).getOrientation() == 'V')
										totalWord += words.at(houses.at(i).at((size_t)word.getColumn() + 1).wordIndex.at(l).first).getWord();

								}
							}
							else if (words.at(houses.at(i).at((size_t)word.getColumn() + 1).wordIndex.at(0).first).getOrientation() == 'V')
								totalWord += words.at(houses.at(i).at((size_t)word.getColumn() + 1).wordIndex.at(0).first).getWord();
							else if (words.at(houses.at(i).at((size_t)word.getColumn() + 1).wordIndex.at(0).first).getOrientation() == 'H')
								totalWord += houses.at(i).at((size_t)word.getColumn() + 1).letter;
						}
					}

					if (totalWord.length() != 1)
					{
						if (std::find(possibelWords.begin(), possibelWords.end(), totalWord) == possibelWords.end()) // check if the word exist
							isValid = 0;
						else isValid = 2;
					}

				}
			}
		}
	}
	return isValid;
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

/*Constructor to load a created board
	@out - opened file to save the board
*/
void Board::saveBoard(std::ofstream& out) const
{
	out << nLines << " X " << nColumns;
	for (size_t i = 0; i < words.size(); i++)
	{
		if (words.at(i).getWord() != "")
		{
			out << std::endl << words.at(i).getStringPosition() << " " << words.at(i).getOrientation() << " " << words.at(i).getWord();
		}
	}
}

/*Board method that give us the words in the dictionary
(there must be a WORDS.TXT in the program file)
*/
void Board::writePossibelWords()
{
	std::ifstream validWords("WORDS.TXT");
	std::string word;
	if (!validWords.is_open())
	{
		std::cerr << "WORDS.TXT not found" << std::endl;
		exit(1);
	}
	while (std::getline(validWords, word))
	{
		std::transform(word.begin(), word.end(), word.begin(), toupper); // make string upper case
		possibelWords.push_back(word);
	}
}