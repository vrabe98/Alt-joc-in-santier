#include "Panel.h"
#include <fstream>

void Panel::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	target.draw(background);
}

Panel::Panel(std::ifstream& file) {
	std::string color;
	sf::Color colr=sf::Color::Transparent;
	int width, height, posx, posy;
	file >> width >> height;
	file >> posx >> posy;
	file >> color;
	if (color.rfind("tx-") == 0) {
		color = color.substr(3,color.length());
		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile("Images//"+color);
		background.setTexture(texture);
	}
	else if (color == "red") {
		colr = sf::Color::Red;
	}
	else if (color == "blue") {
		colr = sf::Color::Blue;
	}
	else if (color == "green") {
		colr = sf::Color::Green;
	}
	else if (color == "white") {
		colr = sf::Color::White;
	}
	if (colr != sf::Color::Transparent) {
		background.setFillColor(colr);
	}
	background.setPosition(sf::Vector2f(posx, posy));
	background.setSize(sf::Vector2f(width, height));
	file >> color;		//reused string for terminator string reading
	if (color != ";;") {
		exit(-5);
	}
}