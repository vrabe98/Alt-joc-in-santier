#pragma once

#include <SFML/Graphics.hpp>

class Panel : public sf::Drawable
{
	sf::RectangleShape background;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	Panel(std::ifstream&);
	Panel(){}
};