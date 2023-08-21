#include "Cell.h"

Cell::Cell(int x, int y) {
	xPos = x;
	yPos = y;
	mine = false;
	revealed = false;
	adjMines = 0;
	flag = false;
	debug = false;
	hiddenCell.setTexture(TextureManager::GetTexture("tile_hidden"));
	revealedCell.setTexture(TextureManager::GetTexture("tile_revealed"));
	flagT.setTexture(TextureManager::GetTexture("flag"));
	mineT.setTexture(TextureManager::GetTexture("mine"));
	for (int i = 0; i < 8; i++) {
		adjCells.push_back(nullptr);
	}
}

void Cell::drawCell(sf::RenderWindow& window) {
	if (!revealed) {
		hiddenCell.setPosition(float(xPos * 32), float(yPos * 32));
		window.draw(hiddenCell);
		if (flag) {
			flagT.setPosition(float(xPos * 32), float(yPos * 32));
			window.draw(flagT);
		}
		if (debug) {
			if (mine) {
				mineT.setPosition(float(xPos * 32), float(yPos * 32));
				window.draw(mineT);
			}
		}
	}
	else {
		revealedCell.setPosition(float(xPos * 32), float(yPos * 32));
		window.draw(revealedCell);
		if (mine) {
			mineT.setPosition(float(xPos * 32), float(yPos * 32));
			window.draw(mineT);
		}
		else {
			if (adjMines > 0) {
				visibleMines.setTexture(TextureManager::GetTexture("number_" + to_string(adjMines)));
				visibleMines.setPosition(float(xPos * 32), float(yPos * 32));
				window.draw(visibleMines);
			}
		}
	}
}

void Cell::setRevealed() {
	if (!flag) {
		revealed = true;
		if (adjMines == 0) {
			for (int i = 0; i < 8; i++) {
				if (!mine && adjCells[i] != nullptr && !(adjCells[i]->revealed) && !(adjCells[i]->mine) && !(adjCells[i]->flag)) {
					adjCells[i]->setRevealed();
				}
			}
		}
	}
}


void Cell::setMine() {
	mine = true;
}

void Cell::removeMine() {
	mine = false;
}

void Cell::increaseAdj() {
	adjMines++;
}

void Cell::resetAdj() {
	adjMines = 0;
	flag = false;
	revealed = false;
}

void Cell::setFlag() {
	flag = !flag;
}

vector<Cell*>& Cell::getAdjCells() {
	return adjCells;
}

bool Cell::isMine() {
	return mine;
}

bool Cell::isFlag() {
	return flag;
}

bool Cell::isRevealed() {
	return revealed;
}

void Cell::setDebug() {
	debug = !debug;
}

void Cell::resetDebug() {
	debug = false;
}

void Cell::debugOn() {
	debug = true;
}

int Cell::getAdjMines() {
	return adjMines;
}