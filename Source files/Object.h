#pragma once
#include <SFML/Graphics.hpp>
#include "Inventory.h"

class Inventory;

#define GENERIC_OBJ 0
#define CONTAINER 1

class Object:public sf::Drawable{
protected:
	int map_id;
	sf::Vector2i coords;
	std::string name, description;
	sf::Texture texture;
	sf::VertexArray vertices;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	virtual int Type() = 0;
	int Map() { return map_id; }
	sf::Vector2i Coords() { return coords; }
	std::string Name() { return name; }
	std::string Description() { return description; }
	virtual Inventory* Get_inventory() = 0;
	virtual void Action() = 0;
	virtual void Load(std::ifstream&) = 0;
};

class Generic_object :public Object {
public:
	int Type() { return GENERIC_OBJ; }
	Inventory* Get_inventory() { return nullptr; }
	void Action() {}
	virtual void Load(std::ifstream& f);
};

class Container_object :public Object {
	Inventory* inventar;
public:
	int Type() { return CONTAINER; }
	void Action(){}
	Inventory* Get_inventory() {
		return inventar;
	}
	virtual void Load(std::ifstream& f);
};