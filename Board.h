#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"
#include "Random.h"
#include "TextureManager.h"

class Board {
	int width;
	int height;
	int numMines;
	int testMines;
	bool isOver;
	bool isWon;
	bool isTest;
	int numFlags;
	sf::Sprite test1;
	sf::Sprite test2;
	sf::Sprite test3;
	sf::Sprite face;
	sf::Sprite debug;
	sf::Sprite digits;
	sf::Sprite digit1;
	sf::Sprite digit2;
	sf::Sprite digit3;
	sf::Sprite minus;
	vector<Cell> cells;
public:
	void setMines();

	Board();
	~Board();
	int getWidth();
	int getHeight();

	void makeCustomBoard(string filename);
	
	void drawTestButtons(sf::RenderWindow& window);
	void drawCells(sf::RenderWindow& window);
	void drawFace(sf::RenderWindow& window);
	void drawDebug(sf::RenderWindow& window);
	void drawCounter(sf::RenderWindow& window);
	void drawBoard(sf::RenderWindow& window);

};

