#include "RoundedRectangle.h"
#include <math.h>

int RoundedRectangle::MouseWithinBounds(sf::Vector2i pos){
	sf::Vector2f position(back.getPosition());
	sf::Vector2f size(back.getLocalBounds().width,back.getLocalBounds().height);
	sf::Vector2f centerpos(position.x + size.x / 2, position.y + size.y / 2);
	float r = corners[0].getRadius(), dist=-1;
	if ((pos.x > position.x) && (pos.x<(position.x + size.x)) && (pos.y>position.y) && (pos.y < (position.y + size.y))) {
		if (pos.x < (centerpos.x - size.x / 2 + r) && pos.y < (centerpos.y - size.y / 2 + r)) {	//circle 0
			sf::Vector2f distvect(pos.x - (corners[0].getPosition().x+r), pos.y - (corners[0].getPosition().y+r));
			dist = sqrt(distvect.x * distvect.x + distvect.y * distvect.y);
		}
		else if (pos.x > (centerpos.x + size.x / 2 - r) && pos.y > (centerpos.y + size.y / 2 - r)) {	//circle 2
			sf::Vector2f distvect(pos.x - (corners[2].getPosition().x+r), pos.y - (corners[2].getPosition().y+r));
			dist = sqrt(distvect.x * distvect.x + distvect.y * distvect.y);
		}
		else if (pos.x > (centerpos.x + size.x / 2 - r) && pos.y < (centerpos.y - size.y / 2 + r)) {	//circle 1
			sf::Vector2f distvect(pos.x - (corners[1].getPosition().x+r), pos.y - (corners[1].getPosition().y+r));
			dist = sqrt(distvect.x * distvect.x + distvect.y * distvect.y);
		}
		else if (pos.x<(centerpos.x - size.x / 2 + r) && pos.y>(centerpos.y + size.y / 2 - r)) {	//circle 3
			sf::Vector2f distvect(pos.x - (corners[3].getPosition().x+r), pos.y - (corners[3].getPosition().y+r));
			dist = sqrt(distvect.x * distvect.x + distvect.y * distvect.y);
		}
		if (dist > r) return 0;
		else return 1;
	}
	else return 0;
}
