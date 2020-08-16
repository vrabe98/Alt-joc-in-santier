#include "Object.h"
#include "Game.h"
#include "Map.h"
#include <string>

#define TILEW 25
#define TILEH 25

#define upper_boundary 68

extern Game* game;

void Object::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	state.texture = &texture;
	target.draw(vertices,state);
}

void Generic_object::Load(std::ifstream& f){
	std::string aux;
	f >> coords.x >> coords.y;
	f >> aux;
	f >> map_id;
	f.ignore();
	std::getline(f, name, '\n');
	std::getline(f,description, '\t');
	texture.loadFromFile("Images//" + aux);
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(4);
	sf::Vertex* quad = &vertices[0];

	quad[0].position = sf::Vector2f(coords.x * TILEW, coords.y * TILEH + upper_boundary);
	quad[1].position = sf::Vector2f((coords.x + 1) * TILEW, coords.y* TILEH + upper_boundary);
	quad[2].position = sf::Vector2f((coords.x + 1) * TILEW, (coords.y + 1) * TILEH + upper_boundary);
	quad[3].position = sf::Vector2f(coords.x * TILEW, (coords.y + 1) * TILEH + upper_boundary);

	quad[0].texCoords = sf::Vector2f(0, 0);
	quad[1].texCoords = sf::Vector2f(TILEW, 0);
	quad[2].texCoords = sf::Vector2f(TILEW, TILEH);
	quad[3].texCoords = sf::Vector2f(0, TILEH);
	f >> aux;
	if (aux != ";;") {
		std::cout << aux;
		exit(-15);
	}
}

void Container_object::Load(std::ifstream& f){
	std::string aux;
	inventar = new Inventory(NONPLAYER_INV);
	f >> coords.x >> coords.y;
	while (true) {
		f >> aux;
		if (aux == ";") break;
		else {
			int item_nr = stoi(aux);
			inventar->Add_item(game->Get_item(item_nr));
		}
	}
	f >> aux;
	f >> map_id;
	f.ignore();
	std::getline(f, name, '\n');
	std::getline(f, description, '\t');
	texture.loadFromFile("Images//" + aux);
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(4);
	sf::Vertex* quad = &vertices[0];

	quad[0].position = sf::Vector2f(coords.x * TILEW, coords.y * TILEH + upper_boundary);
	quad[1].position = sf::Vector2f((coords.x + 1) * TILEW, coords.y * TILEH + upper_boundary);
	quad[2].position = sf::Vector2f((coords.x + 1) * TILEW, (coords.y + 1) * TILEH + upper_boundary);
	quad[3].position = sf::Vector2f(coords.x * TILEW, (coords.y + 1) * TILEH + upper_boundary);

	quad[0].texCoords = sf::Vector2f(0, 0);
	quad[1].texCoords = sf::Vector2f(TILEW, 0);
	quad[2].texCoords = sf::Vector2f(TILEW, TILEH);
	quad[3].texCoords = sf::Vector2f(0, TILEH);
	f >> aux;
	if (aux != ";;") {
		std::cout << aux;
		exit(-15);
	}
}
