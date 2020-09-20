#include "PictureBox.h"

PictureBox::PictureBox(float radius, sf::Vector2f position, sf::Vector2f size) {
	texture = nullptr;
	bounding_box.setPrimitiveType(sf::Quads);
	bounding_box.resize(4);

	sf::Vertex* quad = &bounding_box[0];

	quad[0].position = sf::Vector2f(position.x + radius, position.y + size.y / 2 - CHAR_PICDIM / 2);
	quad[1].position = sf::Vector2f(position.x + radius + CHAR_PICDIM, position.y + size.y / 2 - CHAR_PICDIM / 2);
	quad[2].position = sf::Vector2f(position.x + radius + CHAR_PICDIM, position.y + size.y / 2 - CHAR_PICDIM / 2 + CHAR_PICDIM);
	quad[3].position = sf::Vector2f(position.x + radius, position.y + size.y / 2 - CHAR_PICDIM / 2 + CHAR_PICDIM);

	quad[0].texCoords = sf::Vector2f(0, 0);
	quad[1].texCoords = sf::Vector2f(CHAR_PICDIM, 0);
	quad[2].texCoords = sf::Vector2f(CHAR_PICDIM, CHAR_PICDIM);
	quad[3].texCoords = sf::Vector2f(0, CHAR_PICDIM);

	for (int i = 0; i < 4; i++) {
		quad[i].color = sf::Color::White;
	}
}

PictureBox::PictureBox(sf::Vector2f position, sf::Vector2f size){
	texture = nullptr;
	bounding_box.setPrimitiveType(sf::Quads);
	bounding_box.resize(4);

	sf::Vertex* quad = &bounding_box[0];

	quad[0].position = sf::Vector2f(position.x,position.y);
	quad[1].position = sf::Vector2f(position.x+size.x, position.y);
	quad[2].position = sf::Vector2f(position.x+size.x, position.y+size.y);
	quad[3].position = sf::Vector2f(position.x, position.y+size.y);

	quad[0].texCoords = sf::Vector2f(0, 0);
	quad[1].texCoords = sf::Vector2f(CHAR_PICDIM, 0);
	quad[2].texCoords = sf::Vector2f(CHAR_PICDIM, CHAR_PICDIM);
	quad[3].texCoords = sf::Vector2f(0, CHAR_PICDIM);
}

