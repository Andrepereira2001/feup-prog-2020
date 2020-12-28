#include "Board.h"

using namespace std;

void addWordToBoard(Board& board, Word& word, const int validationValue);

Board creatBoard();

void askWordsToUser(Board& board);

void saveBoardInFile(const Board& board);

int main()
{
	Board board;
	board = creatBoard();
	askWordsToUser(board);
	saveBoardInFile(board);
	Sleep(2000);
	return 0;
}

/*This function add a word to the board if possibel
@&board - reference of the board
@&word - reference of the word that will be added
@validationValue - return value of the method Board::isValidAddWord 
*/
void addWordToBoard(Board &board, Word &word,const int validationValue)
{
	Word removeWord,addedWord,analizeWord;
	string userValidation, trash;
	bool isValid = false, makeChanges = false;

	if (validationValue == 1) // case 1 isValidAddWord
	{
		board.addWord(word);
	}
	// case 2 isValidAddWord
	if (validationValue == 2)
	{
		// check if user wants to make chages
		cout << "In order to add '" << word.getWord() << "' some words will be removed or added\nDo you accept this changes?(yes)" << endl;
		isValid = false;
		while (!isValid) // input error handler
		{
			cin >> userValidation;
			getline(cin, trash, '\n');
			istringstream trashSS(trash);
			trashSS >> trash;
			if (cin.fail())
			{
				if (cin.eof())
				{
					cin.clear();
					isValid = true;
				}
				else
				{
					cin.clear();
					cin.ignore(1000000, '\n');
				}

			}
			else if (trash != "")
			{
				cout << "Invalid Input" << endl;
			}
			else isValid = true;
		}
		std::transform(userValidation.begin(), userValidation.end(), userValidation.begin(), tolower);
		if (userValidation == "yes" || userValidation == "y")
		{
			makeChanges = true;
		}
	}
	if(makeChanges == true)
	{

		if (word.getOrientation() == 'H')
		{
			// words type:  YXXXXXXZZZZZ
			//				Y		
			if ((int)word.getColumn() - 1 >= 0) // if behind the first position still have board
			{
				if (board.getHouseWord(word.getLine(), word.getColumn() - 1, 'H').getWord() != "") // X,Z case
				{
					addedWord = board.getHouseWord(word.getLine(), word.getColumn() - 1, 'H');
					addedWord.setWord(board.getHouseWord(word.getLine(), word.getColumn() - 1, 'H').getWord().
						substr(0, (size_t) word.getColumn() - board.getHouseWord(word.getLine(), word.getColumn() - 1, 'H').getColumn()) + word.getWord());

				}
				else if (board.getHouseWord(word.getLine(), word.getColumn() - 1, 'V').getWord() != "") // X,Y case
				{
					string letter(1, board.getHouseLetter(word.getLine(), word.getColumn() - 1)); // redifine the word from zero
					addedWord.setWord(letter + word.getWord());
					addedWord.setPosition(word.getLine(), word.getColumn() - 1);
					addedWord.setOrientation('H');
				}
				else addedWord = word;

			}
			else addedWord = word;
			if (word.getColumn() + word.getWord().length() < board.getNColumns()) // if after the last position still have board
			{
				if (board.getHouseWord(word.getLine(), word.getColumn() + (int)word.getWord().length(), 'H').getWord() != "")
				{
					addedWord.setWord(board.getHouseWord(word.getLine(), word.getColumn() + (int)word.getWord().length(), 'H').getWord().
						substr(word.getColumn() + word.getWord().length()));
				}
				else if (board.getHouseWord(word.getLine(), word.getColumn() + (int)word.getWord().length(), 'V').getWord() != "") // redifine the word from zero
				{
					string letter(1, board.getHouseLetter(word.getLine(), word.getColumn() + (int)word.getWord().length()));
					addedWord.setWord(addedWord.getWord() + letter);
				}
			}

			//remove all horizontal words in the scope of the new one
			for (int i = word.getColumn() ; i < word.getColumn() + word.getWord().length() ; i++) // check all the word
			{
				removeWord = board.getHouseWord(word.getLine(), i, 'H');
				if (removeWord.getWord() != "") // when find the word
				{
					board.removeWord(removeWord); // remove the existing one
				}
			}
			board.addWord(addedWord);


			//words Type YYYYYY
			//			 XXX  Z
			//				  Z
			for (int i = word.getColumn(); i < word.getColumn() + word.getWord().length(); i++)
			{
				if ((int)word.getLine() - 1 >= 0) // words in the upper houses
				{ 
					if (board.getHouseWord(word.getLine() - 1,i, 'V').getWord() != "") // Y,Z case
					{
						analizeWord = board.getHouseWord(word.getLine() - 1, i, 'V');

						if (analizeWord.getLine() + analizeWord.getWord().length() == word.getLine()) // vertical word don't intercept the existing word
						{
							addedWord = analizeWord;
							addedWord.setWord(analizeWord.getWord() + board.getHouseLetter(word.getLine(), i));
							board.removeWord(analizeWord); // remove vertical word
						}
						else 
						{
							string letter(1, board.getHouseLetter(word.getLine(), i));
							addedWord.setWord(letter);
							addedWord.setPosition(word.getLine(), i);
							addedWord.setOrientation('V');
						}
					}
					else if (board.getHouseWord(word.getLine() - 1, i, 'H').getWord() != "") // Y,X case
					{
						string letter(1, board.getHouseLetter(word.getLine() - 1, i));// redifine the word from zero (creat a new word)
						addedWord.setWord(letter + board.getHouseLetter(word.getLine(), i));
						addedWord.setPosition(word.getLine() - 1, i);
						addedWord.setOrientation('V');
					}
					else
					{
						string letter(1, board.getHouseLetter(word.getLine(), i));
						addedWord.setWord(letter);
						addedWord.setPosition(word.getLine(),i);
						addedWord.setOrientation('V');
					}
				}
				else
				{
					string letter(1, board.getHouseLetter(word.getLine(), i));
					addedWord.setWord(letter);
					addedWord.setPosition(word.getLine(), i);
					addedWord.setOrientation('V');
				}

				if (word.getLine() + 1 < board.getNLines()) //words in the lower houses
				{
					if (board.getHouseWord(word.getLine() + 1, i, 'V').getWord() != "") // Y,Z case
					{
						analizeWord = board.getHouseWord(word.getLine() + 1, i, 'V');
						if (analizeWord.getLine()  == word.getLine() + 1) // vertical word don't intercept the existing word
						{
							addedWord.setWord(addedWord.getWord() + analizeWord.getWord());
							board.removeWord(analizeWord); // remove vertical word
						}
					}
					else if (board.getHouseWord(word.getLine() + 1, i, 'H').getWord() != "") // Y,X case
					{
						addedWord.setWord(addedWord.getWord() + board.getHouseLetter(word.getLine() + 1, i));
					}
				}
				if(addedWord.getWord().length() > 1)
					board.addWord(addedWord);
			}
		}

		if (word.getOrientation() == 'V')
		{
			// words type:  X
			//				X
			//				X
			//				X
			//				YYYYYY		
			if ((int)word.getLine() - 1 >= 0) // if behind the first position still have board
			{
				if (board.getHouseWord(word.getLine() - 1, word.getColumn(), 'V').getWord() != "")
				{
					addedWord = board.getHouseWord(word.getLine() - 1, word.getColumn(), 'V');
					addedWord.setWord(board.getHouseWord(word.getLine() - 1, word.getColumn(), 'V').getWord().
						substr(0,(size_t) word.getLine() - board.getHouseWord(word.getLine() - 1, word.getColumn(), 'V').getLine()) + word.getWord());

				}
				else if (board.getHouseWord(word.getLine() - 1, word.getColumn(), 'H').getWord() != "") // X,Y case
				{
					string letter(1, board.getHouseLetter(word.getLine() - 1, word.getColumn())); // redifine the word from zero
					addedWord.setWord(letter + word.getWord());
					addedWord.setPosition(word.getLine() - 1, word.getColumn());
					addedWord.setOrientation('V');
				}
				else addedWord = word;

			}
			else addedWord = word;

			if (word.getLine() + word.getWord().length() < board.getNLines()) // if after the last position still have board
			{
				if (board.getHouseWord(word.getLine() + (int)word.getWord().length(), word.getColumn(), 'V').getWord() != "")
				{
					addedWord.setWord(board.getHouseWord(word.getLine() + (int)word.getWord().length(), word.getColumn(), 'V').getWord().
						substr(word.getLine() + word.getWord().length()));
				}
				else if (board.getHouseWord(word.getLine() + (int)word.getWord().length(), word.getColumn(), 'H').getWord() != "") // redifine the word from zero
				{
					string letter(1, board.getHouseLetter(word.getLine() + (int)word.getWord().length(), word.getColumn()));
					addedWord.setWord(addedWord.getWord() + letter);
				}
			}

			//remove all vertical words in the scope of the new one
			for (int i = word.getLine(); i < word.getLine() + word.getWord().length(); i++) // check all the word
			{
				removeWord = board.getHouseWord(i, word.getColumn(), 'V');
				if (removeWord.getWord() != "") // when find the word
				{
					board.removeWord(removeWord); // remove the existing one
				}
			}
			board.addWord(addedWord);

			//words Type: YZZZ 
			//			  Y
			//			  YX
			//			  YX
			for (int i = word.getLine(); i < word.getLine() + word.getWord().length(); i++)
			{
				if ((int)word.getColumn() - 1 >= 0) // words in the left house
				{
					if (board.getHouseWord(i, word.getColumn() - 1, 'H').getWord() != "") // Y,Z case
					{
						analizeWord = board.getHouseWord(i, word.getColumn() - 1, 'H');
						if (analizeWord.getColumn() + analizeWord.getWord().length() == word.getColumn()) // if vertical word don't intercept the existing word (XXYXX)
						{																				  //												    (  Y  )
							addedWord = analizeWord;
							addedWord.setWord(analizeWord.getWord() + board.getHouseLetter(i, word.getColumn()));
							board.removeWord(analizeWord); // remove horizontal word
						}
						else
						{
							string letter(1, board.getHouseLetter(i, word.getColumn()));
							addedWord.setWord(letter);
							addedWord.setPosition(i, word.getColumn());
							addedWord.setOrientation('H');
						}
					}
					else if (board.getHouseWord(i, word.getColumn() - 1, 'V').getWord() != "") // Y,X case
					{
						string letter(1, board.getHouseLetter(i, word.getColumn() - 1));// redifine the word from zero (creat a new word)
						addedWord.setWord(letter + board.getHouseLetter(i, word.getColumn()));
						addedWord.setPosition(i, word.getColumn());
						addedWord.setOrientation('H');
					}
					else
					{
						string letter(1, board.getHouseLetter(i, word.getColumn()));
						addedWord.setWord(letter);
						addedWord.setPosition(i, word.getColumn());
						addedWord.setOrientation('H');
					}
				}
				else
				{
					string letter(1, board.getHouseLetter(i, word.getColumn()));
					addedWord.setWord(letter);
					addedWord.setPosition(i, word.getColumn());
					addedWord.setOrientation('H');
				}
				if (word.getColumn() + 1 < board.getNColumns()) //words in the right houses
				{
					if (board.getHouseWord(i, word.getColumn() + 1, 'H').getWord() != "") // Y,Z case
					{
						analizeWord = board.getHouseWord(i, word.getColumn() + 1, 'H');
						if (analizeWord.getColumn() == word.getColumn() + 1) // vertical word don't intercept the existing word
						{
							addedWord.setWord(addedWord.getWord() + analizeWord.getWord());
							board.removeWord(analizeWord); // remove vertical word
						}
					}
					else if (board.getHouseWord(i, word.getColumn() + 1, 'V').getWord() != "") // Y,X case
					{
						addedWord.setWord(addedWord.getWord() + board.getHouseLetter(i, word.getColumn() + 1));
					}
				}
				if (addedWord.getWord().length() > 1 )
					board.addWord(addedWord);
			}
		}
	}
}
 
/*This function ask user the board dimensions and creat it
@return - board with given dimensions
*/
Board creatBoard()
{
	int lines, columns;
	string trash;
	bool isValid = false; // isValid used in input validation

	cout << "What is the size of the board that you want to creat? (Lines  Columns) min = 5, max = 20" << endl;
	while (!isValid)
	{
		cin >> lines >> columns;
		getline(cin, trash, '\n');
		istringstream trashSS(trash);
		trashSS >> trash;
		if (cin.fail())
		{
			if (cin.eof())
			{
				cin.clear();
				cout << "Not valid" << endl;
			}
			else
			{
				cin.clear();
				cin.ignore(1000000, '\n');
				cout << "Can't read that" << endl;
			}
		}
		else if (trash != "")
		{
			cout << "Invalid Input" << endl;
		}
		else if (lines < 5 || lines > 20) // 5 and 20 board limits
		{
			cout << "Lines value out of given limit" << endl;
		}
		else if (columns < 5 || columns > 20) // 5 and 20 board limits
		{
			cout << "Column value out of given limit" << endl;
		}
		else isValid = true;
	}

	Board board(lines, columns); // creat board

	return board;

}

/*This function ask user the words that he want to add to the board
@board - board with all is space alocated
*/
void askWordsToUser(Board& board)
{
	bool limitReach = false, isValid = false;
	string position, word, trash;
	char orientation;
	
	// read user input words to place in board
	cout << "Expected max words= " << board.getMaxWords() << endl << "Expected max letter= " << board.getMaxLetters() << endl;
	while (!limitReach)
	{
		cout << endl;
		board.showBoard();
		cout << "Letters: " << board.getNLetters() << endl << "Words: " << board.getNWords() << endl;
		cout << endl << "Let's add a new word to the board \n(Ctrl+Z when word is asked to end input)\nWhat word do you want to write? ";
		isValid = false;
		while (!isValid) // get string of word
		{
			cin >> word;
			getline(cin, trash, '\n');
			istringstream trashSS(trash);
			trashSS >> trash;
			if (cin.fail())
			{
				if (cin.eof())
				{
					cin.clear();
					limitReach = true;
					isValid = true;
				}
				else
				{
					cin.clear();
					cin.ignore(1000000, '\n');
				}

			}
			else if (trash != "")
			{
				cout << "Invalid Input" << endl;
			}
			else
			{
				isValid = true;
				std::transform(word.begin(), word.end(), word.begin(), toupper); // make string upper case
			}
		}
		if (!limitReach)
		{
			cout << "House of the first letter of the word? "; // get position of the word
			isValid = false;
			while (!isValid)
			{
				cin >> position;
				getline(cin, trash, '\n');
				istringstream trashSS(trash);
				trashSS >> trash;
				if (cin.fail())
				{
					if (cin.eof())
					{
						cin.clear();
					}
					else
					{
						cin.clear();
						cin.ignore(1000000, '\n');
					}

				}
				else if (trash != "")
				{
					cout << "Input error! (Exemple: 'Aa')" << endl;
				}
				else if (position.length() <= 1)
				{
					cout << "Input error! Syntax: (LINEcolumn)" << endl;
				}
				else if (position.length() >= 3)
				{
					cout << "Input error! Syntax: (LINEcolumn)" << endl;
				}
				else isValid = true;
			}

			cout << "Orientation of the word? (V/H) ";  // get orientation of the word
			isValid = false;
			while (!isValid)
			{
				cin >> orientation;
				getline(cin, trash, '\n');
				istringstream trashSS(trash);
				trashSS >> trash;
				if (cin.fail())
				{
					if (cin.eof())
					{
						cin.clear();
					}
					else
					{
						cin.clear();
						cin.ignore(1000000, '\n');
					}

				}
				else if (trash != "")
				{
					cout << "Invalid Input" << endl;
				}
				else
				{
					isValid = true;
					orientation = toupper(orientation);
				}
			}

			Word w1(word, position, orientation);
			addWordToBoard(board, w1, board.isValidAddWord(w1));
			if (board.getNLetters() >= board.getMaxLetters() || board.getNWords() >= board.getMaxWords()) // check if the expected limit was reached
			{
				limitReach = true;
				cout << "Limit reach!" << endl;
			}
		}
	}
}

/*This function save the created board in a file
@board - board with all is words in it
*/
void saveBoardInFile(const Board& board)
{
	string boardName;
	bool isValid = false, boardSaved = false;

	// save created board as a file
	cout << "Time to save our new board!\nWhat name would you like to give him?" << endl;
	while (!boardSaved)
	{
		isValid = false;
		while (!isValid)
		{
			getline(cin, boardName);
			if (cin.eof())
			{
				cin.clear();
				cout << "Not valid. I need a name" << endl;
			}
			else isValid = true;
		}
		// make sure it have .txt in the end
		if (boardName.length() > 4) // given name have at least 4 letters if have less it is not possibel to have .txt writen
		{
			if (boardName.substr(boardName.length() - 4) != ".txt")
			{
				boardName += ".txt";
				cout << ".txt added to the end of the name" << endl;
			}
		}

		ofstream file(boardName);
		if (!file.is_open())
		{
			cout << "Opening file erro" << endl;
		}
		else 
		{
			boardSaved = true;
			board.saveBoard(file);
			file << endl << endl;
			board.showBoard(file); // save 2d board at the end of the .txt file
			file.close();
			cout << "SAVED" << endl;
		}
	}
}

