#include "Pool.h"

Pool::Pool()
{
	this->nLetters = 0;
	this->pool = {};
}

Pool::Pool(int nLetters, std::vector <char> pool)
{
	this->nLetters = nLetters;
	this->pool = pool;
}

int Pool::getNumberLetters() const
{
	return this->nLetters;
}

std::vector<char> Pool::getPool() const
{
	return this->pool;
}

void Pool::addLetter(char letter)
{
	if (letter != ' ')
	{
		this->pool.push_back(toupper(letter));
		this->nLetters++;
	}
}

char Pool::removeRandomLetter()
{
	if (nLetters == 0)
	{
		return ' ';
	}
	int randomNumber;
	randomNumber = rand() % (this->nLetters);
	char letter = this->pool.at(randomNumber);
	for (size_t i = randomNumber; i < pool.size() - 1; i++)
	{
		this->pool.at(i) = this->pool.at(i + 1);
	}
	this->nLetters--;
	this->pool.resize(this->nLetters);
	return letter;
}

void Pool::show() const
{
	std::cout << "[POOL]";
	for (int i = 0; i < this->nLetters; i++)
	{
		std::cout << this->pool.at(i) << " ";
	}
	std::cout << std::endl << std::endl;
}
