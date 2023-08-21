#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "TextureManager.h"

class Cell {
	int xPos;
	int yPos;
	bool mine;
	bool revealed;
	int adjMines;
	bool flag;
	bool debug;
	sf::Sprite hiddenCell;
	sf::Sprite revealedCell;
	sf::Sprite flagT;
	sf::Sprite mineT;
	sf::Sprite visibleMines;
	vector<Cell*> adjCells;

public:
	Cell(int x, int y);

	void drawCell(sf::RenderWindow& window);

	void setRevealed();
	void setMine();
	void removeMine();
	void increaseAdj();
	void resetAdj();
	void setFlag();

	vector<Cell*>& getAdjCells();

	bool isMine();
	bool isFlag();
	bool isRevealed();

	void setDebug();
	void resetDebug();
	void debugOn();

	int getAdjMines();
};

