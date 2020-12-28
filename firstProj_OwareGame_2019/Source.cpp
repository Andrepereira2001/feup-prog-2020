/* 
Made by: André Pereira
Last update: 02/04/2020
*/

#include <iostream>
#include <iomanip> 
#include <vector>
#include <windows.h>
#include <ctime>

using namespace std;

struct BoardHouse
{
	char name; // name of the house
	int nSeeds; // number of seeds in the house
	char playerOwner; // player who owns the house
	bool scoreHouse; // the house is a score house or a gaming house
};

void creatBoard(vector<BoardHouse>& board);

void printBoard(const vector<BoardHouse>& board);

bool victoryChecker(vector <BoardHouse>& board, bool vsComputer);

int houseIndex(char house);

int scoringHouseIndex(char player);

void scoringMaker(vector<BoardHouse>& board, char player, int lastIndex);

bool scoringChecker(vector<BoardHouse> board, char player, int index);

void moveMaker(vector<BoardHouse>& board, char house);

char nextMove(vector<BoardHouse>& board, int player, bool firstTime);

bool haveMoves(vector <BoardHouse>& board, int player);

void runGameMultiplayer(vector <BoardHouse>& board);

char nextMoveBot(vector<BoardHouse>& board, int player, bool hardMode);

void helpCommand(vector<BoardHouse>& board);

vector<char> validHouses(vector<BoardHouse>& board, int player);

bool mustGiveSeeds(vector<BoardHouse>& board, int player);

int bestMoveBot(vector<BoardHouse>& board);

void runGameBot(vector <BoardHouse>& board);

void clrscr();

int main()
{
	bool validInput = false, validString = false, endProgram = false , firstTime = true;
	string choseGame;
	vector<BoardHouse> board;
	char inputAnaliser;
	cout << "Welcome to the Oware Game" << endl;
	do
	{
		if (!firstTime == true)
		{
			clrscr(); // clear screen
		}
		firstTime = false;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
		cout << setw(20) <<"MENU" << endl; // starting menu
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		cout << "To play agains a friend, type 'play'" << endl;
		cout << "To play agains the computer, type 'computer'" << endl;
		cout << "To get some help about the game, type 'help'" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		cout << "(ctrl+Z to close game)" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		creatBoard(board);
		do
		{
			cin >> choseGame; // if crtl+Z
			if (cin.eof())
			{
				cin.clear();
				endProgram = true;
				break;
			}
			validString = false; // checking valid input
			do
			{
				inputAnaliser = cin.get();
				if (inputAnaliser == '\n')
					validString = true;
				else if (inputAnaliser == ' ');
				else if (inputAnaliser == '\t');
				else
				{
					cout << "Write something i can read  :(" << endl;
					cin.ignore(1000000, '\n');
					break;
				}

			} while (!validString);

			if (validString) // take action
			{
				if (choseGame == "play") // run game multiplayer
				{
					cout << "Let the game begin!\nGood fortune to both players!" << endl;
					runGameMultiplayer(board);
					validInput = true;
				}
				else if (choseGame == "computer") // run game agains computer
				{
					runGameBot(board);
					validInput = true;
				}
				else if (choseGame == "help") // see the helping guide
				{
					helpCommand(board);
					validInput = true;
				}
				else // not valid string
				{
					cout << "I can't do nothing with this! \nPlease write something valid :)" << endl;
				}
			}

		} while (!validInput);
	} while (!endProgram);
	cout << "Thanks for playing :) \nHave a nice day!" << endl;
	Sleep(1000);
	return 0;

}

/*function to clear the screen
*/
void clrscr()
{
	COORD coordScreen = { 0, 0 }; // upper left corner
	DWORD cCharsWritten;
	DWORD dwConSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	// fill with spaces
	FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	// cursor to upper left corner
	SetConsoleCursorPosition(hCon, coordScreen);
}
/**	This function fills the board with is starter values
@board - vector who contains the BoardHouses
*/
void creatBoard(vector<BoardHouse>& board)
{
	board.resize(14); // number of houses

	//scoring houses
	board.at(0) = { 'P',0,'1',true }; // player 1
	board.at(7) = { 'P',0,'2',true }; // player 2

	// gaming houses
	// player 1
	board.at(1) = { 'a',4,'1',false };
	board.at(2) = { 'b',4,'1',false };
	board.at(3) = { 'c',4,'1',false };
	board.at(4) = { 'd',4,'1',false };
	board.at(5) = { 'e',4,'1',false };
	board.at(6) = { 'f',4,'1',false };
	// player 2
	board.at(8) = { 'g',4,'2',false };
	board.at(9) = { 'h',4,'2',false };
	board.at(10) = { 'i',4,'2',false };
	board.at(11) = { 'j',4,'2',false };
	board.at(12) = { 'k',4,'2',false };
	board.at(13) = { 'l',4,'2',false };
}

/** This function outputs the current state of the recived board
@board - vector who contains the BoardHouses
*/
void printBoard(const vector<BoardHouse>& board)
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 1);
	cout << setw(44) << "GAME BOARD" << endl;
	SetConsoleTextAttribute(hConsole, 15);
	// upper house names
	SetConsoleTextAttribute(hConsole, 11);
	cout << setw(5) << board.at(0).name << board.at(0).playerOwner;
	SetConsoleTextAttribute(hConsole, 4);
	cout << setw(9) << board.at(13).name << setw(10) << board.at(12).name <<
		setw(10) << board.at(11).name << setw(10) << board.at(10).name << setw(10) << board.at(9).name <<
		setw(10) << board.at(8).name;
	SetConsoleTextAttribute(hConsole, 12);
	cout << setw(10) << board.at(7).name << board.at(7).playerOwner << endl;
	SetConsoleTextAttribute(hConsole, 15);
	cout << string(80, '-') << endl;
	for (int i = 0; i < 7; i++)
		cout << setw(10) << '|';
	cout << endl;
	// player 2 gaming house values
	cout << setw(10) << '|';
	SetConsoleTextAttribute(hConsole, 2);
	cout << setw(5) << board.at(13).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 2);
	cout << setw(5) << board.at(12).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 2);
	cout << setw(5) << board.at(11).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 2);
	cout << setw(5) << board.at(10).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 2);
	cout << setw(5) << board.at(9).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 2);
	cout << setw(5) << board.at(8).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|' << endl;
	for (int i = 0; i < 7; i++)
		cout << setw(10) << '|';
	cout << endl;
	// player 1 and 2 scoring house values
	SetConsoleTextAttribute(hConsole, 6);
	cout << setw(5) << board.at(0).nSeeds << setw(4) << ' ';
	SetConsoleTextAttribute(hConsole, 15);
	cout << string(61, '-');
	SetConsoleTextAttribute(hConsole, 6);
	cout << setw(6) << board.at(7).nSeeds << endl;
	SetConsoleTextAttribute(hConsole, 15);
	for (int i = 0; i < 7; i++)
		cout << setw(10) << '|';
	cout << endl;
	// player 1 gaming house value
	cout << setw(10) << '|';
	SetConsoleTextAttribute(hConsole, 10);
	cout << setw(5) << board.at(1).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 10);
	cout << setw(5) << board.at(2).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 10);
	cout << setw(5) << board.at(3).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 10);
	cout << setw(5) << board.at(4).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 10);
	cout << setw(5) << board.at(5).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|';
	SetConsoleTextAttribute(hConsole, 10);
	cout << setw(5) << board.at(6).nSeeds;
	SetConsoleTextAttribute(hConsole, 15);
	cout << setw(5) << '|' << endl;
	for (int i = 0; i < 7; i++)
		cout << setw(10) << '|';
	cout << endl;
	cout << string(80, '-') << endl;
	// lower house names
	SetConsoleTextAttribute(hConsole, 11);
	cout << setw(5) << board.at(0).name << board.at(0).playerOwner;
	SetConsoleTextAttribute(hConsole, 3);
	cout << setw(9) << board.at(1).name << setw(10) << board.at(2).name <<
		setw(10) << board.at(3).name << setw(10) << board.at(4).name << setw(10) << board.at(5).name <<
		setw(10) << board.at(6).name;
	SetConsoleTextAttribute(hConsole, 12);
	cout << setw(10) << board.at(7).name << board.at(7).playerOwner << endl;
	SetConsoleTextAttribute(hConsole, 15);

}

/** this function check if we have a victorius player
@board - vector who contains the board houses
@vsComputer - True if computer is playing
return - true if some player wins otherwise returns false
*/
bool victoryChecker(vector <BoardHouse>& board, bool vsComputer = false)
{
	bool endGame = false;
	if (board.at(0).nSeeds > 24) // player 1 wins
	{
		printBoard(board);
		cout << "Player 1 Wins!!!" << endl;
		cout << "CONGRATULATIONS" << endl;
		endGame = true;
	}
	else if (board.at(7).nSeeds > 24) // player 2 wins
	{

		printBoard(board);
		if (vsComputer == false) // vs player
		{
			cout << "Player 2 Wins!!!" << endl;
			cout << "CONGRATULATIONS" << endl;
		}
		else if (vsComputer == true) // vs computer
		{
			cout << "It's a victory for the computer" << endl;
			cout << "Good luck next time!" << endl;
		}
		endGame = true;
	}
	else if (board.at(0).nSeeds == 24 && board.at(7).nSeeds == 24) // game draw
	{
		printBoard(board);
		cout << "Its a draw!" << endl;
		cout << "Both playes win :)" << endl;
		endGame = true;
	}
	if (endGame == true)
	{
		// just a little music to end with style
		Beep(329, 300);
		Beep(493, 300);
		Beep(698, 300);
		Beep(659, 600);

		Beep(783, 300);
		Beep(698, 300);
		Beep(659, 600);

		Beep(329, 100);
		Beep(493, 300);
		Beep(698, 300);
		Beep(659, 600);

		Beep(392, 250);
		Beep(440, 200);
		Beep(587, 300);

		Beep(349, 250);
		Beep(587, 500);
	}

	return endGame;
}


/** this function return the index of the house
@ house - letter of the house
@ board - the vector that contains the boar houses
return - the house index in the vector board
*/
int houseIndex(char house)
{
	int index;
	switch (house)
	{
	case 'a':
		index = 1;
		break;
	case 'b':
		index = 2;
		break;
	case 'c':
		index = 3;
		break;
	case 'd':
		index = 4;
		break;
	case 'e':
		index = 5;
		break;
	case 'f':
		index = 6;
		break;
	case 'g':
		index = 8;
		break;
	case 'h':
		index = 9;
		break;
	case 'i':
		index = 10;
		break;
	case 'j':
		index = 11;
		break;
	case 'k':
		index = 12;
		break;
	case 'l':
		index = 13;
		break;
	}

	return index;
}

/* this function return the index of the player scoring house
	@player - player that make the move
	return - player scoring house index
	*/
int scoringHouseIndex(char player)
{
	int index;
	if (player == '1')
		index = 0;
	else if (player == '2')
		index = 7;

	return index;
}

/* this function checks if the player score some points
	and if he does the function add the points to his score house
	@board - vector who contains the board houses
	@player - player that makes the move
	@lastIndex - the index of the last house that received a seed
	*/
void scoringMaker(vector<BoardHouse>& board, char player, int lastIndex)
{
	bool getPoints = true;

	if (board.at(lastIndex).playerOwner == player)	// rules to score
		getPoints = false;
	else if (board.at(lastIndex).nSeeds > 3)
		getPoints = false;
	else if (board.at(lastIndex).nSeeds < 2)
		getPoints = false;
	else if (board.at(lastIndex).scoreHouse)
		getPoints = false;

	if (getPoints)
	{
		board.at(scoringHouseIndex(player)).nSeeds += board.at(lastIndex).nSeeds; // add the seeds to the scoring house
		board.at(lastIndex).nSeeds = 0; // remove the seeds from the gaming house
		scoringMaker(board, player, lastIndex - 1); // if possibel colect the previous seeds
	}

}

/* this function objective is to detect the sowing
	@board - the vector that contains the board houses
	@player - the player who makes the move
	@index - the index of the chosen house
	return - True if sowing appens, False if it doesn´t append
	*/
bool scoringChecker(vector<BoardHouse> board, char player, int index)
{
	bool makeScoring = false;
	int firstHouse = 0, lastHouse = 0;
	vector<BoardHouse> testBoard = board;// creat a new board equal to board

	if (player == '2') // if player 1 plays see player 2 houses
	{
		firstHouse = 1;
		lastHouse = 6;
	}
	else if (player == '1') // if player 2 plays see player 1 houses
	{
		firstHouse = 8;
		lastHouse = 13;
	}

	scoringMaker(testBoard, player, index); // make the scoring in the test board
	for (int i = firstHouse; i <= lastHouse; i++) // checks if there is some house that have seeds
		if (testBoard.at(i).nSeeds > 0)
			makeScoring = true;

	return makeScoring;
}

/**this function distribute the seeds of the given house
	@board - vector that cointains the board houses
	@house - the house whose seeds will be distributed
	*/
void moveMaker(vector<BoardHouse>& board, char house)
{
	int index = houseIndex(house);
	int seeds;
	bool validAdd = true; // is valid to add a seed to that house
	char player;

	player = board.at(index).playerOwner;
	seeds = board.at(index).nSeeds;
	board.at(index).nSeeds = 0;

	while (seeds > 0) // make the seeds destribution
	{
		index = (index + 1) % 14;
		if (board.at(index).scoreHouse)
			validAdd = false;
		else if (board.at(index).name == house)
			validAdd = false;
		if (validAdd)
		{
			board.at(index).nSeeds += 1;
			seeds -= 1;
		}
		else validAdd = true;
	}
	if (scoringChecker(board, player, index) == true) // checks if we can colect the points
		scoringMaker(board, player, index);

}

/* this function asks the user what house he want to move
	and checks if the input is valid, if not asks again
	@board - vector that contains the board houses
	@player - player turn
	@firstTime - is the first time that the function was caled in the turn
	return - house chosen by user
	*/
char nextMove(vector<BoardHouse>& board, int player, bool firstTime = true)
{
	char houseName, playerTurn, endGame;
	int index;
	bool endOfInput, keepGoing, validHouse = true;
	vector<char> possibelHouses;

	switch (player)	// player turn
	{
	case 1:
		playerTurn = '1';
		break;
	case 2:
		playerTurn = '2';
		break;
	}

	do
	{

		if (firstTime) // first time asking the house to move
		{
			firstTime = false;
			cout << "Moving house: ";
			cin >> houseName;
			do
			{
				endOfInput = false;
				if (cin.eof()) // if ctrl+z possibel to end the game
				{
					cin.clear();
					cout << "Are you sure you want to end the game? (y)" << endl; // make sure that it was not a mistake
					cin >> endGame;
					if ((endGame == 'y') || cin.eof()) // if sure to end
					{
						if (!cin.eof()) 
							cin.ignore(10000000, '\n'); 
						cin.clear();
						return 'q';
					}
					else
					{
						cout << "Let's continue then.\nPlayer " << player << " chose a house to move" << endl; // keep playing with anything else
						cin.clear();
						cin.ignore(1000000, '\n');
						endOfInput = true;
						cin >> houseName;
					}
				}
			} while (endOfInput);
			if (cin.peek() == '\n'); // if the input is more then a char
			else
			{
				do // remove all spaces and tabs from the input buffer
				{
					keepGoing = false;
					if (cin.peek() == ' ') // see if there is only spaces in the input (excpet the first char)
					{
						cin.ignore(1); // if peek == space, ignore it
						keepGoing = true;
					}
					else if (cin.peek() == '\t') // if peek == \t ignore it
					{
						cin.ignore(1);
						keepGoing = true;
					}
				} while (keepGoing);

				if (cin.peek() == '\n');
				// if the peek is \n it continues because it is valid
				else // if peek is something else then spaces and \n gives error
				{
					cin.clear();
					cin.ignore(100000000, '\n'); // ignore the trash input
					cout << "Give me something i can read!" << endl;
					houseName = nextMove(board, player, firstTime);
				}
			}
		}
		else // second time asking the house to move
		{
			cout << "Player " << playerTurn << " choose another house to move" << endl;
			cout << "Moving house: ";
			cin >> houseName;
			do
			{
				endOfInput = false;
				if (cin.eof())
				{
					cout << "I dont support that feature:( \nClose me and open again to return to menu!" << endl;
					cin.clear();
					endOfInput = true;
					cout << "But if you want to continue playing just type the house name" << endl;
					cin >> houseName;
				}
			} while (endOfInput);
			if (cin.peek() == '\n');	// if the input is more then a char
			else
			{
				do // remove all spaces and tabs from the input buffer
				{
					keepGoing = false;
					if (cin.peek() == ' ') // see if there is only spaces in the input (excpet the first char)
					{
						cin.ignore(1); // if peek == space, ignore it
						keepGoing = true;
					}
					else if (cin.peek() == '\t') // if peek == \t ignore it
					{
						cin.ignore(1);
						keepGoing = true;
					}
				} while (keepGoing);
				if (cin.peek() == '\n');
				// if the peek is \n it continues because it is valid
				else // if peek is something else then spaces and \n gives error
				{
					cin.clear();
					cin.ignore(100000000, '\n'); // ignore the trash input
					cout << "Give me something i can read!" << endl;
					houseName = nextMove(board, player, firstTime);
				}
			}
		}

		validHouse = true;
		if ((int)houseName - (int)'a' > 11 || (int)houseName - (int)'a' < 0) // if user inputs a letter that dont have house
		{
			validHouse = false;
			cout << "There is no house with this name!" << endl;
		}
		else
		{
			index = houseIndex(houseName);
			if (board.at(index).playerOwner != playerTurn) // player dont own the house chosen
			{
				validHouse = false;
				cout << "You dont control this house!" << endl;
			}
			else if (board.at(index).nSeeds == 0) // board house have 0 seeds
			{
				validHouse = false;
				cout << "This house don't have seeds to move!" << endl;
			}
			else if (mustGiveSeeds(board, player) == true) // have chance to give opponent seeds to continue the game
			{
				validHouse = false;
				possibelHouses = validHouses(board, player); // name of the houses that will continue the game
				for (int i = 0; i < possibelHouses.size(); i++) // check if the input is equal to some of the possibel houses
				{
					if (houseName == possibelHouses.at(i))
						validHouse = true;
				}
				if (validHouse == false)
					cout << "Be a nice player give some seeds to your enemy :)" << endl;
			}
		}

	} while (!validHouse);
	return houseName;
}

/* this function checks if the the actual player must make a move to give seeds to the opponent
@board - the vector that contains teh board houses
@player - the player that is playing
return - true if the enemy have seeds in ther houses else false
*/
bool mustGiveSeeds(vector<BoardHouse>& board, int player)
{
	char checkPlayer;
	int firstHouse, lastHouse;
	bool mustGive = true;


	if (player == 2) // houses to check (player 1 houses)
	{
		firstHouse = 1;
		lastHouse = 6;
	}
	else if (player == 1) // houses to check (player 2 houses)
	{
		firstHouse = 8;
		lastHouse = 13;
	}
	for (int i = firstHouse; i <= lastHouse; i++) // see if some of the houses have seeds
		if (board.at(i).nSeeds > 0)
			mustGive = false;

	if (mustGive)
	{
		if (validHouses(board, player).size() == 0) // if you dont have any house that give seeds to the opponent, every one is valid
			mustGive = false;
	}

	return mustGive;
}

/* this function give us a vector with the name of the houses that a player need to move to give the opponent seeds
@board - the vector that contains teh board houses
@player - the player that is playing
return - vector that contain the valid houses
*/
vector<char> validHouses(vector<BoardHouse>& board, int player)
{
	int firstHouse, lastHouse;
	vector<char> possibelHouses;

	if (player == 1) // player 1 houses
	{
		firstHouse = 1;
		lastHouse = 6;
	}
	else if (player == 2) // player 2 houses
	{
		firstHouse = 8;
		lastHouse = 13;
	}

	for (int i = firstHouse; i <= lastHouse; i++) // see the possibel houses that the player have to move in order to give seeds 
	{
		if (board.at(i).nSeeds >= 7 - (i % 7))
		{
			possibelHouses.push_back(board.at(i).name); //add there name to a vector
		}
	}

	return possibelHouses;
}

/** this function checks if the atual player have possibel moves
@board - vector that cointains the board houses
@player - player that will chose the house
return - true if player have moves, false if not
*/
bool haveMoves(vector <BoardHouse>& board, int player)
{
	int firstHouse, lastHouse, playerIndex;
	bool haveMove = false;
	char playerChar;
	if (player == 1) // player 1 houses
	{
		firstHouse = 1;
		lastHouse = 6;
	}
	else if (player == 2) // player 2 houses
	{
		firstHouse = 8;
		lastHouse = 13;
	}
	for (int i = firstHouse; i <= lastHouse; i++) // see if some of the houses have seeds
		if (board.at(i).nSeeds > 0)
			haveMove = true;

	if (!haveMove)	// if no moves other player collect all the seeds
	{
		cout << "You dont have any house to move :(" << endl << "The game it's over!!" << endl;

		if (player == 2) // player 2 have no moves player 1 collects
		{
			firstHouse = 1;
			lastHouse = 6;
			playerChar = '1';
		}
		else if (player == 1) // player 1 have no moves player 2 collects
		{
			firstHouse = 8;
			lastHouse = 13;
			playerChar = '2';
		}

		playerIndex = scoringHouseIndex(playerChar);
		for (int i = firstHouse; i <= lastHouse; i++)	//collect the seeds 
		{
			board.at(playerIndex).nSeeds += board.at(i).nSeeds;
			board.at(i).nSeeds = 0;
		}
	}
	return haveMove;
}

/* this function asks the user what house he want to move
and checks if the input is valid, if not asks again
@board - vector that contains the board houses
@player - player turn
@hardMode - True if hard mode have been chosen
return - house chosen by user
*/
char nextMoveBot(vector<BoardHouse>& board, int player, bool hardMode = false)
{
	char houseName, playerTurn = '2';
	int index, houseChoser = -1;
	bool validHouse = true;
	vector<char> possibelHouses;

	srand(time(NULL)); // 100% random choises

	if (hardMode == true)
	{
		if (bestMoveBot(board) > 0)
			houseChoser = bestMoveBot(board);
	}
	do
	{
		if (houseChoser < 0)
			houseChoser = rand() % 6 + 8; // chose one house random
		switch (houseChoser)
		{
		case 8:
			houseName = 'g';
			break;
		case 9:
			houseName = 'h';
			break;
		case 10:
			houseName = 'i';
			break;
		case 11:
			houseName = 'j';
			break;
		case 12:
			houseName = 'k';
			break;
		case 13:
			houseName = 'l';
			break;
		}
		validHouse = true;
		index = houseIndex(houseName);
		if (board.at(index).nSeeds == 0) // board house have 0 seeds
		{
			validHouse = false;
			houseChoser = -1;
		}
		else if (mustGiveSeeds(board, player) == true)
		{
			validHouse = false;
			houseChoser = -1;
			possibelHouses = validHouses(board, player);
			for (int i = 0; i < possibelHouses.size(); i++)
			{
				if (houseName == possibelHouses.at(i))
					validHouse = true;
			}
		}

	} while (!validHouse);
	cout << "Chosen house: " << houseName << endl;
	return houseName;
}

/** this is the main function to run the game 1vs1
@board - vector with the board houses
*/
void runGameMultiplayer(vector <BoardHouse>& board)
{
	char movingHouse;
	int playerTurn = 1;
	clrscr(); // clear screen
	do
	{
		printBoard(board);
		cout << "Player " << playerTurn << " turn" << endl; // output player turn

		if (haveMoves(board, playerTurn)) // checking if have possibel moves
		{
			movingHouse = nextMove(board, playerTurn);
			if (movingHouse == 'q') // receive comand o end the game
				return; // return to main function
			moveMaker(board, movingHouse);
		}

		playerTurn = (playerTurn % 2 + 1); // swaping players


	} while (!victoryChecker(board));
}

/** this is the main function to run the game agains a bot
@board - vector with the board houses
*/
void runGameBot(vector <BoardHouse>& board)
{
	char movingHouse, inputAnaliser;
	int playerTurn = 1;
	string choseDifficulty;
	bool validString, hardMode, validInput = false;
	clrscr(); // clear screen
	// little menu for the computer version
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	cout << setw(15)<< "DIFFICULTY" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "Easy mode, type 'easy'" << endl;
	cout << "Hard mode, type 'hard'" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	cout << "(ctrl+Z back to menu)" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	do
	{
		validString = false; // checking valid input
		do
		{
			cin >> choseDifficulty;
			inputAnaliser = cin.peek();
			if (cin.eof()) // if ctrl+Z press end fucntion
			{
				cin.clear();
				return; // end the funtion call
			}
			// testing chars that don't influence the input
			else if (inputAnaliser == '\n') 
				validString = true;
			else if (inputAnaliser == ' ') // check space
				cin.ignore(1);
			else if (inputAnaliser == '\t')// check tab
				cin.ignore(1);
			// invalid char input
			else
			{
				cout << "Write something i can read  :(" << endl;
				cin.ignore(1000000, '\n');
				break;
			}

		} while (!validString);

		if (validString) // take action
		{
			if (choseDifficulty == "easy") // run game multiplayer
			{
				hardMode = false;
				cout << "Man vs machine!\nMay the best win!" << endl;
				validInput = true;
			}
			else if (choseDifficulty == "hard") // run game agains computer
			{
				hardMode = true;
				cout << "Man vs machine!\nMay the best win!" << endl;
				validInput = true;
			}

			else // not valid string
			{
				cout << "I can't do nothing with this! \nPlease write something valid :)" << endl;
			}
		}
	} while (!validInput);
	clrscr(); // clear screen
	do
	{
		printBoard(board); // output the actual board

		if (haveMoves(board, playerTurn))
		{
			if (playerTurn == 1) // person playing
			{
				cout << "Player " << playerTurn << " turn" << endl;
				movingHouse = nextMove(board, playerTurn); // ask input
				if (movingHouse == 'q') // if receive order to stop the game
					return;	// return to main function
			}
			else // computer tunr
			{
				cout << "Computer turn" << endl;
				movingHouse = nextMoveBot(board, playerTurn, hardMode); // chose moves with the chosen diffuculty
			}


			moveMaker(board, movingHouse); // call funtion do make move 

		}
		playerTurn = (playerTurn % 2 + 1); // next turn


	} while (!victoryChecker(board,true)); // check program end
}

/*This function have the help guide for the user
@board - board that contais the board houses
*/
void helpCommand(vector<BoardHouse>& board)
{
	char userInput, inputAnaliser;
	bool validInput = false, validChar = false;
	clrscr(); // clear screen
	// help guide menu
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	cout <<setw(16) << "HELP\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "To see the game rules, type '1'" << endl << "To understand the board, type '2' \n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	cout << "(ctrl+Z to return to the menu)" << endl; 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	do
	{

		cin >> userInput; // if ctrl+z return to menu
		if (cin.eof())
		{
			cin.clear();
			break;
		}
		validChar = false; // input is valid
		do
		{
			inputAnaliser = cin.peek();

			if (inputAnaliser == '\n')
				validChar = true;
			else if (inputAnaliser == ' '); // skiping spaces 
			else if (inputAnaliser == '\t');// and tabs
			else
			{
				cout << "Write something i can read :(" << endl;
				cin.ignore(1000000, '\n');
				break;
			}

		} while (!validChar);

		if (validChar)
		{


			if (userInput == '1') // game rules
			{
				cin.clear();
				cin.ignore(10000000, '\n');
				cout << string(120, '=') << endl;
				cout << "The game starts with 4 seeds in each house, the goal is to capture the seeds. \n";
				cout << "Wins the one that colect more then 24 seeds first. If both player have 24 there is a draw" << endl;
				cout << "Players take turns to chose the house to move, first starts ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				cout << "Player 1 \n";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "After chosing a house the sowing is made, in counterclockwise" << endl;
				cout << "To capture the seeds you need to bring the opponent's house to exacly 2 or 3 seeds\nIf the houses immediately behind have 2 or 3 seeds they are also captured" << endl;
				cout << "SPECIFIC RULES:\nYou must give your opponent the chance to play. \nIf some player have 0 seeds in there houses the opponent must make a play that will give him some";
				cout << "\nIn the moves that would capture all the opponent seeds, there will be no scoring, the seeds will only be distributed" << endl;
				cout << "In your turn if you don't have seeds the opponent capture all of his seeds" << endl;
				cout << "It is possibel to end the game by pressing crtl+Z when it is asked and input" << endl;
				cout << string(120, '=') << endl;
			}
			else if (userInput == '2') // understand board
			{

				cin.clear();
				cin.ignore(10000000, '\n');
				cout << string(120, '=') << endl;
				cout << "The letters above the houses and below are the houses name. To choose a house to play just type ther name" << endl; // chose house
				cout << "The houses with the name "; // scoring houses
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				cout << "P1 ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "and ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "P2 ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "are the scoring houses of ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				cout << "player 1 ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "and ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "player 2";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << ", respectively\nDon't try move them, you can't ;)" << endl;
				printBoard(board);
				cout << "The upper houses, letters "; // gaming houses
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				cout << "g-l ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "are from ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "player 2" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "The lower houses, letters ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
				cout << "a-f ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "are from ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				cout << "player 1" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "The "; // number houses 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				cout << "numbers ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				cout << "in the houses represent the number of seeds that they have" << endl;
				cout << string(120, '=') << endl;
			}
			else // invalid char input
			{
				cout << "I can't do nothing with this! \nPlease write something valid :)" << endl;
				cin.ignore(1000000, '\n');
			}
		}

	} while (true);

}

/* this function returns the index of the house that will give more seeds
@board - vector that contains the gaming houses
*/
int bestMoveBot(vector<BoardHouse>& board)
{
	vector <BoardHouse> testBoard;
	int bestScore = 0;
	char bestMoveIdx = -1;

	// make a fake move to now the best 
	for (int i = 8; i <= 13; i++) // bot houses (player 2)    
	{
		testBoard = board;
		testBoard.at(7).nSeeds = 0; // place scoring house to 0, easier to analyze
		moveMaker(testBoard,(char) (i + 95));
		if (testBoard.at(7).nSeeds > bestScore)
		{
			bestMoveIdx = i;
			bestScore = testBoard.at(7).nSeeds;
		}
	}

	return bestMoveIdx;
}
