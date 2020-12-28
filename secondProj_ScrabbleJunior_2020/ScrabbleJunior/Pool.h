#pragma once
#include <vector>
#include <iostream>

class Pool
{
public:
	Pool();
	Pool(int nLetters, std::vector <char> pool);
	int getNumberLetters() const;
	std::vector <char> getPool() const;
	void addLetter(char letter);
	char removeRandomLetter();
	void show() const;
private:
	int nLetters;  //number of letters in pool (is always updating)
	std::vector <char> pool;  //has the letters
};
