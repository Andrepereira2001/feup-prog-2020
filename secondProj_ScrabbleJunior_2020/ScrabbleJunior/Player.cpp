#include "Player.h"

Player::Player()
{
	this->name = "NO_NAME";
	this->hand = {};
	this->points = 0;
	this->playerColor = 0;
}

Player::Player(std::string name)
{
	this->name = name;
	this->hand = {};
	this->points = 0;
	this->playerColor = 0;
}

Player::Player(std::string name, std::vector<char> hand)
{
	this->name = name;
	this->hand = hand;
	this->points = 0;
	this->playerColor = 0;
}

std::string Player::getName() const
{
	return this->name;
}

std::vector<char> Player::getHand() const
{
	return this->hand;
}

int Player::getPoints() const
{
	return points;
}

int Player::getColor() const
{
	return playerColor;
}

void Player::setPlayerColor(int color)
{
	this->playerColor = color;
}

void Player::setName()
{
	std::string playerName;
	std::cout << "What's the Player Name? ";
	getline(std::cin, playerName);
	while (std::cin.fail())
	{
		std::cin.clear();
		std::cout << "What's the Player Name? ";
		getline(std::cin, playerName);
	}
	std::cout << std::endl;
	this->name = playerName;
}

void Player::showPoints() const
{
	std::cout << points;
	std::cout << std::endl;
}

void Player::showHand()const
{
	std::cout << "(hand) ";
	for (size_t i = 0; i < hand.size(); i++)
	{
		std::cout << hand[i] << " ";
	}
	std::cout << std::endl << std::endl;
}

void Player::oneMorePoint()
{
	points++;
}

void Player::addToHand(char letter)
{
	if (letter != ' ')
	{
		hand.push_back(letter);
	}
}

void Player::removeFromHand(char letter)
{
	int k = -1;
	for (size_t i = 0; i < hand.size(); i++)
	{
		if (hand.at(i) == toupper(letter))
		{
			k = (int)i;
			break;
		}
	}
	for (size_t j = k; j < hand.size() - 1; j++)
	{
		this->hand.at(j) = this->hand.at(j + 1);
	}
	this->hand.resize(hand.size() - 1);
}

/*PLAYERMETHOD evaluates if the letter is in the hand vector if it is not then false
	@letter
RETURNS true if it is from player's hand*/
bool Player::validLetterFromHand(char letter)
{
	for (int i = 0; i < hand.size(); i++)
	{
		if (hand.at(i) == letter)
			return true;
	}
	return false;
}
