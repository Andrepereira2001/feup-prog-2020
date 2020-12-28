#include "Pool.h"
#include "Board.h"
#include "Player.h"
#include <ctime>
#define LIGHTBLUE 11 // letter light blue / background black
#define WHITE 15 // letter white / background black
#define NUMBEROFLINESANDCOLUMNSMULTIPLIER 0.0195 // value that multiplied by number os board lines and columns helps to find number of letters in hand
#define NUMBERADDEDnLETTERSINHAND 2.7 // value that added to a multiplication gives a reasonable number to number of letters in hand

using namespace std;

const int askNumberPlayers();

Letter askPlayerLetterPlay();

void atributeFirstHand(Pool& pool, const int nLettersForEachPlayerBegin, Player& player);

void atributeFirstHand(Pool& pool, const int nLettersForEachPlayerBegin, Player& player);

void takeLetterfromPool(Pool& pool, Player& player);

void showPlayerInformation(Player& player);

void fillPlayers(vector <Player>& players, const int nPlayers, Pool& pool, const Board& board);

bool validPositionLetterOnBoard(const Board& board, const Letter& letter);

Letter choosingValidLetter(Board& board, Player& player);

void fillPool(Pool& pool, Board& board);

const int numberOfMovesPlayerCanPlay(Player& player, Board& board);

const char askSingleLetter();

void game(vector <Player>& players, Board& board, Pool& pool);

void anouncingWinners(const vector <Player>& players);

ifstream gettingFilename();

int main()
{
	srand((unsigned int)time(NULL));
	ifstream in = gettingFilename();
	Board board(in);
	setColor(WHITE);
	board.showBoard();
	Pool pool;
	fillPool(pool, board);
	pool.show();
	int nPlayers = askNumberPlayers();
	vector <Player> players;
	fillPlayers(players, nPlayers, pool, board);
	board.showBoard();
	game(players, board, pool);
	anouncingWinners(players);
	Sleep(2000);
	return 0;
}

/*FUNCTION asks the user how many players are going to play
RETURNS number of players chosen (between 2-4)*/
const int askNumberPlayers()
{
	setColor(WHITE);
	int numberPlayers;
	bool validInput = false;
	while (!validInput)
	{
		cout << "How many Players? ";
		cin >> numberPlayers;
		cout << endl;
		if (cin.fail())
		{
			if (cin.eof())
			{
				cout << "You've entered an invalid number of players (it must be between 2-4)." << endl;
				cin.clear();
			}
			else
			{
				cout << "You've entered an invalid number of players (it must be between 2-4)." << endl;
				cin.clear();
				cin.ignore(1000, '\n');
			}
		}
		else if (numberPlayers >= 2 && numberPlayers <= 4)
		{
			cin.ignore(1000, '\n');
			validInput = true;
		}
		else
		{
			cout << "You've entered an invalid number of players (it must be between 2-4)." << endl;
			cin.clear();
			cin.ignore(1000, '\n');
		}
	}
	return numberPlayers;
}

/*FUNCTION this function is called when the player can play one of the letters in the hand
and asks the letter and the position in the board
RETURN letter (with its parametres letter, line and column)*/
Letter askPlayerLetterPlay()
{
	setColor(WHITE);
	char letter;
	cout << "Please choose a letter from your hand: ";
	cin >> letter;
	while (cin.fail() || cin.peek() != '\n' || (letter - (int)'A' < 0) || (letter - (int)'z' > 0) || ((letter - (int)'Z' > 0) && (letter - (int)'a' < 0)))
	{
		if (cin.eof())
		{
			cout << "You've entered an invalid letter." << endl;
			cin.clear();
		}
		else
		{
			cout << "You've entered an invalid letter." << endl;
			cin.clear();
			cin.ignore(1000, '\n');
		}
		cout << "Please choose a letter from your hand: ";
		cin >> letter;
	}
	Letter l{};
	l.letter = toupper(letter);
	cin.clear();
	cin.ignore(1000, '\n');
	string position;
	cout << "Please indicate the position for the letter (Example: Ab (Line:A, Column:b)): ";
	getline(cin, position);
	while (cin.fail() || position.size() != 2 || (position.at(0) - (int)'A' < 0) || (position.at(0) - (int)'z' > 0) || ((position.at(0) - (int)'Z' > 0) && (position.at(0) - (int)'a' < 0)) || (position.at(1) - (int)'A' < 0) || (position.at(1) - (int)'z' > 0) || ((position.at(1) - (int)'Z' > 0) && (position.at(1) - (int)'a' < 0)))
	{
		if (cin.eof())
		{
			cout << "You've entered an invalid position." << endl;
			cin.clear();
		}
		else
		{
			cout << "You've entered an invalid position." << endl;
			cin.clear();
		}
		cout << "Please indicate the position for the letter (Example: Ab (Line:A, Column:b)): ";
		getline(cin, position);
	}
	l.line = (toupper(position.at(0)) - (int)'A');
	l.column = (tolower(position.at(1)) - (int)'a');
	cin.clear();
	return l;
}

/*FUNCTION receives the pool and how many words are there for each player and gives them to the player hand
	@&pool
	@nLettersForEachPlayerBegin - defined in fillPlayers() this variable depends on how many letters are there in the pool
and the number of players
	@&player - the palyer currently playing
*/
void atributeFirstHand(Pool& pool, const int nLettersForEachPlayerBegin, Player& player)
{
	char l;
	for (int i = 0; i < nLettersForEachPlayerBegin; i++)
	{
		l = pool.removeRandomLetter();
		if (l != ' ')
		{
			player.addToHand(l);
		}
		else break;
	}
}

/*FUNCTION after each round the player takes a random word from the pool to his hand
	@&pool
	@&player - the palyer currently playing
*/
void takeLetterfromPool(Pool& pool, Player& player)
{
	char l;
	l = pool.removeRandomLetter();
	if (l != ' ')
	{
		player.addToHand(l);
	}
}

void showPlayerInformation(Player& player)
{
	setColor(player.getColor());
	cout << "[" << player.getName() << "] ";
	player.showPoints();
	player.showHand();
}

/*FUNCTION initializes each player by giving them a name asked to the user, a random color and a hand
	@&players - vector of players created with the amount of players chosen by the user
	@nPlayers - number of players asked to the user in the beginnig
	@&board
	@&pool
*/
void fillPlayers(vector <Player>& players, const int nPlayers, Pool& pool, const Board& board)
{
	int numberLettersInHand;  //this variable depends on  how many letters are there in the pool and the number of players
	if ((pool.getNumberLetters() >= 28 && nPlayers == 4) || (pool.getNumberLetters() >= 21 && nPlayers == 3) || (pool.getNumberLetters() >= 14 && nPlayers == 2))
	{
		numberLettersInHand = 7;
	}
	else
	{
		//founded this function in order to have a more reasonable number of letters in hand for the smaller boards
		numberLettersInHand = (int)round(NUMBEROFLINESANDCOLUMNSMULTIPLIER * board.getNLines() * board.getNColumns() + NUMBERADDEDnLETTERSINHAND);
		if (pool.getNumberLetters() % 2 != 0)
		{
			numberLettersInHand--;
		}
	}
	for (int i = 0; i < nPlayers; i++)
	{
		Player p;
		setColor(WHITE);
		p.setName();
		atributeFirstHand(pool, numberLettersInHand, p);
		p.setPlayerColor(LIGHTBLUE + i);
		players.push_back(p);
		setColor(p.getColor());
		showPlayerInformation(p);
		setColor(WHITE);
	}
}

/*FUNCTION confirms if the position of the letter given matches with the letter on board on that position
	@&board
	@&letter - the letter chosen by the user with its properties (letter, line and column)
RETURNS true if it matches
RETURNS false if it doesn't*/
bool validPositionLetterOnBoard(const Board& board, const Letter& letter)
{
	if (letter.line >= board.getNLines() || letter.column >= board.getNColumns())  //confirms if the position given is in board 
	{
		return false;
	}
	if (board.getHouseLetter(letter.line, letter.column) == letter.letter)
	{
		return true;
	}
	return false;
}

/*FUNCTION this function calls the one where the user gives a letter and a position and confirms these conditions:
1) if the position of the letter given matches with the letter on board on that position
2) if the player has the letter chosen in his hand
3) if the letter is not already played
4) if the letter is the next one to paint in the words that contain her
if not then asks another letter
	@&board
	@&player - the palyer currently playing
RETURNS the valid letter*/
Letter choosingValidLetter(Board& board, Player& player)
{
	setColor(WHITE);
	Letter l;
	bool valid = false;
	do
	{
		l = askPlayerLetterPlay();
		if (l.line >= board.getNLines() || l.column >= board.getNLines())
		{
			valid = false;
		}
		else
		{

			if (validPositionLetterOnBoard(board, l) && player.validLetterFromHand(l.letter) && board.validToPaint(l) == 0)
			{
				valid = true;

			}
			if (validPositionLetterOnBoard(board, l) && player.validLetterFromHand(l.letter) && board.validToPaint(l) == 2) //case when the  1 word ends
			{
				valid = true;
				//one more point to the player
				player.oneMorePoint();
			}
			else if (validPositionLetterOnBoard(board, l) && player.validLetterFromHand(l.letter) && board.validToPaint(l) == 3) //in case it ends two words
			{
				valid = true;
				//two more points to the player
				player.oneMorePoint();
				player.oneMorePoint();
			}
		}
		if (!valid)
		{
			cout << "You've entered an invalid letter and position." << endl;
		}
	} while (!valid);
	return l;
}

/*FUNCTION (only called one time in the first place after getting the board)
fills the pool with the letters that are not ' ' from the board vector houses
	@&board
	@&pool
*/
void fillPool(Pool& pool, Board& board)
{
	for (int i = 0; i < board.getNLines(); i++)
	{
		for (int j = 0; j < board.getNColumns(); j++)
		{
			pool.addLetter(board.getHouseLetter(i, j));
		}
	}
}

/*FUNCTION trys to play each letter on the players hand and checks if it can be played
	@&player - the palyer currently playing
	@&board
RETURNS 0 if he can't do any move
RETURNS 1 if he can only do one move
RETURNS 2 if he can do the two moves or more*/
const int numberOfMovesPlayerCanPlay(Player& player, Board& board)
{
	int nMoves = 0;
	if (player.getHand().size() == 0)  //no available letters in his hand so he can't play
	{
		nMoves = 0;
	}
	else
	{
		for (int i = 0; i < player.getHand().size(); i++)
		{
			if (board.validToPlay(player.getHand().at(i)) == true)
			{
				nMoves++;
			}
		}
	}
	if (nMoves > 2)  //because he can only do maximum 2 moves
	{
		nMoves = 2;
	}
	return nMoves;
}

/*FUNCTION only used for situations when player can't play a letter and so he will chose one from hand to draw to pool
RETURNS letter (in char) he chose*/
const char askSingleLetter()
{
	setColor(WHITE);
	char letter;
	cout << "Since you can't play any letter from your hand." << endl;
	cout << "Please choose a letter from your hand to draw to the pool: ";
	cin >> letter;
	while (cin.fail() || cin.peek() != '\n' || (toupper(letter) - (int)'A' < 0) || (tolower(letter) - (int)'z' > 0) || ((toupper(letter) - (int)'Z' > 0) && (tolower(letter) - (int)'a' < 0)))
	{
		if (cin.eof())
		{
			cout << "You've entered an invalid letter." << endl;
			cin.clear();
		}
		else
		{
			cout << "You've entered an invalid letter." << endl;
			cin.clear();
			cin.ignore(1000, '\n');
		}
		cout << "Please choose a letter from your hand to draw to the pool: ";
		cin >> letter;
	}
	return letter;
}

/*FUNCTION is the whole game including each round until the players have nothing left to play
	@&players - vector of players with whole of the information
	@&board
	@&pool
*/
void game(vector <Player>& players, Board& board, Pool& pool)
{
	bool valid = true; //evaluates in the end of each round if the game can continue
	//valid = true if the game can continue
	//valid = false if it can't 
	vector <bool> continuePlaying = {}; //for each player the indx is the same as for vector players
	//assumes continuePlaying.at(X)=true if he has letters to play and false if it doesn't
	//initializes with true for each player
	for (int i = 0; i < players.size(); i++)
	{
		continuePlaying.push_back(true);
	}
	int nMoves;  //number of moves the player can do in each round
	char letter, letter1; //letters chosen by the player when he can't play anything o board and has to draw to the pool
	Letter l;
	while (valid)
	{
		//starts a round 
		pool.show();
		for (int i = 0; i < players.size(); i++)
		{
			showPlayerInformation(players.at(i));
			if (players.at(i).getHand().size() == 0) //doesn't have letters to play
			{
				setColor(WHITE);
				cout << "The game has finished for you. Please wait until the anouncement of the winners." << endl << endl;
				continuePlaying.at(i) = false;
			}
			else
			{
				nMoves = numberOfMovesPlayerCanPlay(players.at(i), board);
				if (nMoves >= 1)
				{
					l = choosingValidLetter(board, players.at(i));
					board.paintHouse(l);
					board.showBoard();
					players.at(i).removeFromHand(l.letter);
					showPlayerInformation(players.at(i));
					nMoves = numberOfMovesPlayerCanPlay(players.at(i), board);
					//he plays two letters in the board
					if (nMoves >= 1)
					{
						l = choosingValidLetter(board, players.at(i));
						board.paintHouse(l);
						board.showBoard();
						players.at(i).removeFromHand(l.letter);
						takeLetterfromPool(pool, players.at(i));
						takeLetterfromPool(pool, players.at(i));
						showPlayerInformation(players.at(i));
					}
					//played 1 letter and draws one to the pool and takes other from the pool
					else if (nMoves == 0 && players.at(i).getHand().size() != 0 && pool.getNumberLetters() != 0)
					{
						do
						{
							letter = askSingleLetter();
						} while (!players.at(i).validLetterFromHand(toupper(letter)));
						takeLetterfromPool(pool, players.at(i));
						takeLetterfromPool(pool, players.at(i));
						players.at(i).removeFromHand(letter);
						pool.addLetter(toupper(letter));
						showPlayerInformation(players.at(i));
					}
					//played only 1 letter because the pool is empty
					else if (nMoves == 0 && players.at(i).getHand().size() != 0 && pool.getNumberLetters() == 0)
					{
						cout << "You can't do anything, pass and wait for the next round." << endl << endl;
					}
					//played only 1 letter because the pool is empty and he has no letters
					else if (nMoves == 0 && players.at(i).getHand().size() == 0 && pool.getNumberLetters() == 0)
					{
						showPlayerInformation(players.at(i));
						setColor(WHITE);
						cout << "The game has finished for you. Please wait until the anouncement of the winners." << endl << endl;
						continuePlaying.at(i) = false;
					}
				}
				else if (nMoves == 0 && pool.getNumberLetters() != 0 && players.at(i).getHand().size() != 0)
				{
					//draws only one chosen letters to the pool and picks other from the pool
					if (pool.getNumberLetters() == 1)
					{
						do
						{
							letter = askSingleLetter();
						} while (!players.at(i).validLetterFromHand(toupper(letter)));
						takeLetterfromPool(pool, players.at(i));
						players.at(i).removeFromHand(letter);
						pool.addLetter(toupper(letter));
						showPlayerInformation(players.at(i));
					}
					//has to draw two chosen letters to the pool and pick other two from the pool
					if (pool.getNumberLetters() >= 2)
					{
						do
						{
							letter = askSingleLetter();
						} while (!players.at(i).validLetterFromHand(toupper(letter)));
						players.at(i).removeFromHand(letter);
						showPlayerInformation(players.at(i));
						do
						{
							letter1 = askSingleLetter();
						} while (!players.at(i).validLetterFromHand(toupper(letter1)));
						players.at(i).removeFromHand(letter1);
						takeLetterfromPool(pool, players.at(i));
						takeLetterfromPool(pool, players.at(i));
						pool.addLetter(toupper(letter));
						pool.addLetter(toupper(letter1));
						showPlayerInformation(players.at(i));
					}
				}
				//can keep playing until he can play the letters that has in hand
				else if (nMoves == 0 && pool.getNumberLetters() == 0 && players.at(i).getHand().size() != 0) //isince the pool has no letters passes
				{
					cout << "You can't do anything, pass and wait for the next round." << endl << endl;
				}
				board.showBoard();
			}
		}
		valid = false;
		//at the end of each round if it happens to the players's hand are empty the game finishes 
		//else if at least one can play the game continues
		for (int i = 0; i < players.size(); i++)
		{
			if (continuePlaying.at(i) == true)
			{
				valid = true;
				break;
			}
		}
	}
}

/*FUNCTION shows the user the players with the most points
	@&players - vector of players with whole of the information
*/
void anouncingWinners(const vector <Player>& players)
{
	setColor(WHITE);
	int maxPoints = players.at(0).getPoints();
	for (int i = 1; i < players.size(); i++)
	{
		if (players.at(i).getPoints() > maxPoints)
		{
			maxPoints = players.at(i).getPoints();
		}
	}
	cout << "These are the winners: " << endl;
	for (int i = 0; i < players.size(); i++)
	{
		if (players.at(i).getPoints() == maxPoints)
		{
			setColor(players.at(i).getColor());
			cout << "[" << players.at(i).getName() << "]" << endl;
			setColor(WHITE);
		}
	}
}

/*FUNCTION asks and opens the file with the board
RETURNS the file*/
ifstream gettingFilename()
{
	ifstream in;
	string filename;
	setColor(WHITE);
	do
	{
		cout << "Please enter the filename of the board you want to use (followed by .txt): ";
		getline(cin, filename);
		in.open(filename, ios::in);
		if (in.fail())
		{
			cout << "You've entered an invalid filename for a board." << endl;
		}
	} while (in.fail());
	return in;
}

