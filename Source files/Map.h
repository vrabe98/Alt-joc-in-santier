#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "Object.h"

#define MAX_OBJ 100

class Connection:public sf::Drawable {
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	sf::VertexArray vertices;
	sf::Texture* texture;
	sf::Vector2i entry, exit;
	int exit_map_id;
	std::string Exit_map_name();
};

class Map:public sf::Drawable
{
	char* map_arr;
	sf::VertexArray vertices;
	std::list<Connection> conns;
	short int dim_x, dim_y;
	std::string name;
	std::string description;
	std::list<Object*> objects;
	std::list<int> characters;
public:
	Connection* Get_conn_data(sf::Vector2i pos);
	int Check_terrain(sf::Vector2i pos);
	std::string Name() { return name; }
	std::string Description() { return description; }
	void Add_character(int id) {
		characters.push_back(id);
	}
	void Load(std::ifstream&,sf::Texture*);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	void Add_object(Object* obj) { objects.push_back(obj); }
	int Impassable(sf::Vector2i pos);
	void Tile_action(sf::Vector2i pos);
};