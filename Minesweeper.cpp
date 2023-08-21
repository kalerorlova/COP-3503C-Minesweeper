#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.h"
#include "TextureManager.h"
using namespace std;

int main() {
    Board game;
	sf::RenderWindow window(sf::VideoMode(game.getWidth() * 32, game.getHeight() * 32 + 100), "Minesweeper");
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			window.clear(sf::Color(255, 255, 255, 255));
			game.drawBoard(window);
			window.display();
		}
	}
	return 0;
}

