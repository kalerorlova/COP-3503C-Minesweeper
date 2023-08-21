#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "Board.h"

void Board::setMines() {
	vector<int> idxs;
	int minesSet = 0;
	isTest = false;
	testMines = 0;
	for (int i = 0; i < width * height; i++) {
		cells[i].removeMine();
		cells[i].resetAdj();
	}
	while (minesSet != numMines) {
		int idx = Random::Int(0, width * height - 1);
		if (find(idxs.begin(), idxs.end(), idx) == idxs.end()) {
			idxs.push_back(idx);
			cells[idx].setMine();
			minesSet++;
		}
	}
	for (int i = 0; i < width * height; i++) {
		for (int j = 0; j < 8; j++) {
			if (cells[i].getAdjCells()[j] != nullptr && (cells[i].getAdjCells()[j])->isMine()) {
				cells[i].increaseAdj();
			}
		}
	}
}

Board::Board() {
	ifstream file("./boards/config.cfg");
	if (!file.is_open()) {
		cout << "Failed to load the config.cfg!\nFailed to initialize the game window!" << endl;
		width = 0;
		height = 0;
		numMines = 0;
		numFlags = 0;
	}
	else {
		string line = "";
		vector<int> cfgValues;
		while (getline(file, line)) {
			istringstream myStr(line);
			string token;
			getline(myStr, token);
			cfgValues.push_back(stoi(token));
		}
		width = cfgValues[0];
		height = cfgValues[1];
		numMines = cfgValues[2];
	}
	file.close();
	isOver = false;
	isWon = false;
	isTest = false;
	numFlags = 0;
	testMines = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Cell cell(j, i);
			cells.push_back(cell);
		}
	}
	for (int i = 0; i < width * height; i++) {
		//top left
		if (i == 0) {
			(cells[i].getAdjCells())[3] = &cells[i + 1];
			(cells[i].getAdjCells())[4] = &cells[i + width + 1];
			(cells[i].getAdjCells())[5] = &cells[i + width];
		}
		//top right
		else if (i == width - 1) {
			(cells[i].getAdjCells())[5] = &cells[i + width];
			(cells[i].getAdjCells())[6] = &cells[i + width - 1];
			(cells[i].getAdjCells())[7] = &cells[i - 1];
		}
		//bottom left
		else if (i == (height - 1) * width) {
			(cells[i].getAdjCells())[1] = &cells[i - width];
			(cells[i].getAdjCells())[2] = &cells[i - width + 1];
			(cells[i].getAdjCells())[3] = &cells[i + 1];
		}
		//bottom right
		else if (i == height * width - 1) {
			(cells[i].getAdjCells())[0] = &cells[i - width - 1];
			(cells[i].getAdjCells())[1] = &cells[i - width];
			(cells[i].getAdjCells())[7] = &cells[i - 1];
		}
		//top row
		else if (i > 0 && i < width - 1) {
			(cells[i].getAdjCells())[7] = &cells[i - 1];
			(cells[i].getAdjCells())[3] = &cells[i + 1];
			(cells[i].getAdjCells())[4] = &cells[i + width + 1];
			(cells[i].getAdjCells())[5] = &cells[i + width];
			(cells[i].getAdjCells())[6] = &cells[i + width - 1];
		}
		//bottom row
		else if (i > (height - 1) * width && i < height * width - 1) {
			(cells[i].getAdjCells())[7] = &cells[i - 1];
			(cells[i].getAdjCells())[0] = &cells[i - width - 1];
			(cells[i].getAdjCells())[1] = &cells[i - width];
			(cells[i].getAdjCells())[2] = &cells[i - width + 1];
			(cells[i].getAdjCells())[3] = &cells[i + 1];
		}
		//left column
		else if (i != 0 && i != (height - 1) * width && i % width == 0) {
			(cells[i].getAdjCells())[1] = &cells[i - width];
			(cells[i].getAdjCells())[2] = &cells[i - width + 1];
			(cells[i].getAdjCells())[3] = &cells[i + 1];
			(cells[i].getAdjCells())[4] = &cells[i + width + 1];
			(cells[i].getAdjCells())[5] = &cells[i + width];
		}
		//right column
		else if (i != width - 1 && i != height * width - 1 && (i - 24) % width == 0 && i - 24 > 0) {
			(cells[i].getAdjCells())[0] = &cells[i - width - 1];
			(cells[i].getAdjCells())[1] = &cells[i - width];
			(cells[i].getAdjCells())[5] = &cells[i + width];
			(cells[i].getAdjCells())[6] = &cells[i + width - 1];
			(cells[i].getAdjCells())[7] = &cells[i - 1];
		}
		//middle
		else {
			(cells[i].getAdjCells())[0] = &cells[i - width - 1];
			(cells[i].getAdjCells())[1] = &cells[i - width]; 
			(cells[i].getAdjCells())[2] = &cells[i - width + 1];
			(cells[i].getAdjCells())[3] = &cells[i + 1];
			(cells[i].getAdjCells())[4] = &cells[i + width + 1];
			(cells[i].getAdjCells())[5] = &cells[i + width];
			(cells[i].getAdjCells())[6] = &cells[i + width - 1];
			(cells[i].getAdjCells())[7] = &cells[i - 1];
		}
	}
	setMines();
}

Board::~Board() {
	TextureManager::Clear();
}

int Board::getWidth() {
	return width;
}

int Board::getHeight() {
	return height;
}

void Board::makeCustomBoard(string filename) {
	ifstream file(filename);
	vector<string> setup;
	if (!file.is_open()) {
		cout << "Failed to load the test board!" << endl;
	}
	else {
		string line = "";
		while (getline(file, line)) {
			istringstream myStr(line);
			string token;
			getline(myStr, token);
			setup.push_back(token);
		}
	}
	file.close();
	isOver = false;
	isWon = false;
	isTest = true;
	numFlags = 0;
	testMines = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			cells[j * width + i].removeMine();
			cells[j * width + i].resetAdj();
			cells[j * width + i].resetDebug();
			if (setup[j][i] == '1') {
				cells[j * width + i].setMine();
				testMines++;
			}
		}
	}
	for (int i = 0; i < width * height; i++) {
		for (int j = 0; j < 8; j++) {
			if (cells[i].getAdjCells()[j] != nullptr && (cells[i].getAdjCells()[j])->isMine()) {
				cells[i].increaseAdj();
			}
		}
	}
}

void Board::drawTestButtons(sf::RenderWindow& window) {
	test1.setTexture(TextureManager::GetTexture("test_1"));
	test2.setTexture(TextureManager::GetTexture("test_2"));
	test3.setTexture(TextureManager::GetTexture("test_3"));
	test1.setPosition(float(5 * 32 + width * 32 / 2), float(height * 32));
	test2.setPosition(float(7 * 32 + width * 32 / 2), float(height * 32));
	test3.setPosition(float(9 * 32 + width * 32 / 2), float(height * 32));
	window.draw(test1);
	window.draw(test2);
	window.draw(test3);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);
		if (localPosition.x > 5 * 32 + width * 32 / 2 && localPosition.x < 7 * 32 + width * 32 / 2
			&& localPosition.y > height * 32 && localPosition.y < height * 32 + 64) {
			makeCustomBoard("./boards/testboard1.brd");
			drawCells(window);
		}
		if (localPosition.x > 7 * 32 + width * 32 / 2 && localPosition.x < 9 * 32 + width * 32 / 2
			&& localPosition.y > height * 32 && localPosition.y < height * 32 + 64) {
			makeCustomBoard("./boards/testboard2.brd");
			drawCells(window);
		}
		if (localPosition.x > 9 * 32 + width * 32 / 2 && localPosition.x < 11 * 32 + width * 32 / 2
			&& localPosition.y > height * 32 && localPosition.y < height * 32 + 64) {
			makeCustomBoard("./boards/testboard3.brd");
			drawCells(window);
		}
	}
}

void Board::drawCells(sf::RenderWindow& window) {
	if (!isOver && !isWon) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i localPosition = sf::Mouse::getPosition(window);
			if (localPosition.x > 0 && localPosition.x < width * 32 && localPosition.y > 0 && localPosition.y < height * 32) {
				cells[(localPosition.y / 32) * width + localPosition.x / 32].setRevealed();
				if (cells[(localPosition.y / 32) * width + localPosition.x / 32].isMine() &&
					!cells[(localPosition.y / 32) * width + localPosition.x / 32].isFlag()) {
					isOver = true;
					for (int i = 0; i < cells.size(); i++) {
						cells[i].debugOn();
					}
				}
				for (int i = 0; i < width * height; i++) {
					if (!cells[i].isMine() && !cells[i].isRevealed()) {
						break;
					}
					if (i == width * height - 1) {
						isWon = true;
						for (int j = 0; j < width * height; j++) {
							cells[j].resetDebug();
							if (cells[j].isMine() && !cells[j].isFlag()) {
								cells[j].setFlag();
								numFlags++;
							}
						}
					}
				}
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::Vector2i localPosition = sf::Mouse::getPosition(window);
			if (localPosition.x > 0 && localPosition.x < width * 32 && localPosition.y > 0 && localPosition.y < height * 32 &&
				!cells[(localPosition.y / 32) * width + localPosition.x / 32].isRevealed()) {
				if (cells[(localPosition.y / 32) * width + localPosition.x / 32].isFlag()) {
					numFlags--;
				}
				else {
					numFlags++;
				}
				cells[(localPosition.y / 32) * width + localPosition.x / 32].setFlag();
			}
		}
	}
	for (int i = 0; i < cells.size(); i++) {
		cells[i].drawCell(window);
	}
}

void Board::drawFace(sf::RenderWindow& window) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);
		if (localPosition.x > width * 32 / 2 - 32 && localPosition.x < width * 32 / 2 + 32 && localPosition.y > height * 32 && localPosition.y < height * 32 + 64) {
			isOver = false;
			isWon = false;
			numFlags = 0;
			for (int i = 0; i < cells.size(); i++) {
				cells[i].resetDebug();
			}
			setMines();
		}
	}
	if (!isOver) {
		face.setTexture(TextureManager::GetTexture("face_happy"));
	}
	else if (isOver) {
		face.setTexture(TextureManager::GetTexture("face_lose"));
	}
	if (isWon) {
		face.setTexture(TextureManager::GetTexture("face_win"));
	}
	face.setPosition(float(width * 32 / 2 - 32), float(height * 32));
	window.draw(face);
}

void Board::drawDebug(sf::RenderWindow& window) {
	debug.setTexture(TextureManager::GetTexture("debug"));
	debug.setPosition(float(3 * 32 + width * 32 / 2), float(height * 32));
	window.draw(debug);

	if (!isOver && !isWon) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			//sf::Vector2i localPosition = sf::Mouse::getPosition(window);
			sf::Vector2f localPosition(sf::Mouse::getPosition(window));
			//if (localPosition.x > 3 * 32 + width * 32 / 2 && localPosition.x < 5 * 32 + width * 32 / 2 && localPosition.y > height * 32 && localPosition.y < height * 32 + 64) {
			if (debug.getGlobalBounds().contains(localPosition)) {
				for (int i = 0; i < cells.size(); i++) {
					cells[i].setDebug();
				}
			}
		}
	}
}

void Board::drawCounter(sf::RenderWindow& window) {

	int count = abs(numMines - numFlags);
	if (isTest) {
		count = abs(testMines - numFlags);
	}
	int ones = count % 10;
	count = count / 10;
	int tens = count % 10;
	count = count / 10;
	int hund = count % 10;

	digit1.setTexture(TextureManager::GetTexture("digits"));
	digit1.setTextureRect(sf::Rect<int>(21 * hund, 0, 21, 32));
	digit2.setTexture(TextureManager::GetTexture("digits"));
	digit2.setTextureRect(sf::Rect<int>(21 * tens, 0, 21, 32));
	digit3.setTexture(TextureManager::GetTexture("digits"));
	digit3.setTextureRect(sf::Rect<int>(21 * ones, 0, 21, 32));

	digit1.setPosition(float(max(21, width * 32 / 16)), float(height * 32));
	window.draw(digit1);
	digit2.setPosition(float(max(21, width * 32 / 16) + 21), float(height * 32));
	window.draw(digit2);
	digit3.setPosition(float(max(21, width * 32 / 16) + 21 * 2), float(height * 32));
	window.draw(digit3);

	count = numMines - numFlags;
	if (isTest) {
		count = testMines - numFlags;
	}

	if (count < 0) {
		minus.setTexture(TextureManager::GetTexture("digits"));
		minus.setTextureRect(sf::Rect<int>(21 * 10, 0, 21, 32));
		minus.setPosition(float(max(0, width * 32 / 16 - 21)), float(height * 32));
		window.draw(minus);
	}
}

void Board::drawBoard(sf::RenderWindow& window) {
	drawTestButtons(window);
	drawCells(window);
	drawFace(window);
	drawDebug(window);
	drawCounter(window);
}