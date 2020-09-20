#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>

class RoundedRectangle:public sf::Drawable{
	sf::ConvexShape back;
	sf::CircleShape corners[4];
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const {
		target.draw(back);
		for (int i = 0; i < 4; i++) {
			target.draw(corners[i]);
		}
	}
public:
	sf::Vector2f Position(){
		return back.getPosition();
	}
	sf::Vector2f Size() {
		return sf::Vector2f(back.getPoint(2).x-back.getPoint(7).x, back.getPoint(4).y-back.getPoint(0).y);
	}
	float Radius() { return corners[0].getRadius(); }
	bool MouseWithinBounds(sf::Vector2i pos);
	RoundedRectangle(){}
	RoundedRectangle(float radius,sf::Vector2f position,sf::Vector2f size) {
		back.setPointCount(8);

		for (int i = 0; i < 4; i++) {
			corners[i].setRadius(radius);
			corners[i].setFillColor(sf::Color(50, 50, 50));
		}
		corners[0].setPosition(position);
		corners[1].setPosition(sf::Vector2f(position.x + size.x - 2*radius, position.y));
		corners[2].setPosition(sf::Vector2f(position.x + size.x - 2*radius, position.y + size.y - 2*radius));
		corners[3].setPosition(sf::Vector2f(position.x, position.y + size.y - 2*radius));

		back.setPosition(position);

		back.setPoint(0, sf::Vector2f(radius, 0));
		back.setPoint(1, sf::Vector2f(size.x - radius, 0));
		back.setPoint(2, sf::Vector2f(size.x, radius));
		back.setPoint(3, sf::Vector2f(size.x, size.y - radius));
		back.setPoint(4, sf::Vector2f(size.x - radius, size.y));
		back.setPoint(5, sf::Vector2f(radius, size.y));
		back.setPoint(6, sf::Vector2f(0, size.y - radius));
		back.setPoint(7, sf::Vector2f(0,radius));
		back.setFillColor(sf::Color(50, 50, 50));
	}
};