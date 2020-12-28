#pragma once
#include <iostream>
#include <string>
#include <vector>

class Player
{
public:
	Player();
	Player(std::string name);
	Player(std::string name, std::vector<char> hand);
	std::string getName() const;
	std::vector <char> getHand() const;
	int getPoints() const;
	int getColor() const;
	void setPlayerColor(int color);
	void setName();
	void showPoints() const;
	void showHand() const;
	void oneMorePoint();
	void addToHand(char letter);
	void removeFromHand(char letter);
	bool validLetterFromHand(char letter);
private:
	std::string name;  //name of the player is asked in first place 
	int playerColor;  //each player has a color so they can differ between each other
	std::vector<char> hand;  //has the letters in the hand of player
	int points;   //classification the one/ones with the most points (finished words) win
};
