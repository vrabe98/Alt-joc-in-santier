#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

#define CHAR_PICDIM 100	//character picture is square

class PictureBox : public sf::Drawable {
	sf::VertexArray bounding_box;
	sf::Texture* texture;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const {
		state.texture = this->texture;
		target.draw(bounding_box, state);
	}
public:
	PictureBox() {}
	PictureBox(float radius, sf::Vector2f position, sf::Vector2f size);
	PictureBox(sf::Vector2f position, sf::Vector2f size);
	void Set_texture(sf::Texture* tex) {
		texture = tex;
	}
	sf::Vector2f Position() {
		sf::Vertex* quad = &bounding_box[0];
		return quad[0].position;
	}
	sf::Vector2f Size() {
		sf::Vertex* quad = &bounding_box[0];
		return sf::Vector2f(quad[1].position.x - quad[0].position.x, quad[2].position.y - quad[0].position.y);
	}
};