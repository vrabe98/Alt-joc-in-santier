#include "Map.h"
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>
#include "Game.h"

#define TILEW 25
#define TILEH 25

#define INV_TRANSFER 7

#define CHAR_OFFSET 200

#define upper_boundary 68

#define TILE_STRING 4

#define NORMAL 0
#define CONN 1
#define IMPASSABLE_TERR 2
#define GENERIC_OBJ_BLOCK 3
#define NPC_ 4
#define CNT_ 5

#define GENERIC_OBJ 0
#define CONTAINER 1

extern Game* game;

std::string Connection::Exit_map_name()
{
	return game->Get_map(exit_map_id)->Name();
}

void Map::Tile_action(sf::Vector2i pos){
	int code = Check_terrain(pos);
	std::string message, aux;
	switch (code) {
	case NORMAL:
		message = "Te scufunzi\nparca in pamantul\nmoale, ca intr-o\ndepresie fara leac.\nDiverse ganduri\nte napadesc,\ndintre care unul\niti acapareaza\ntot spatiul dintre\nochi: gandul ca\nte vei intoarce,\nla un moment\ndat tot in acest\npamant moale\nsi familiar.";
		break;
	case CONN:	
		for (std::list<Connection>::iterator i = conns.begin(); i != conns.end(); i++) {
			if (i->entry == pos) aux = i->Exit_map_name();
		 }
		message = "Te afli intre\ndoua lumi:\npe de o parte\nse afla\n" + Name()+",\niar pe partea\ncealalta se afla\n" + aux+ ".\nCulmea, tu nu\nai vrea sa\nte afli in\nniciuna, ci doar\nsa iesi definitiv\ndin acest Univers\ndeprimant.";
		break;
	case IMPASSABLE_TERR:message="Oricat de mult\nai vrea, stii\nsi tu ca nu\nvei putea\ntrece peste\nacest obstacol\nce se ridica\namenintator\nin fata ta.";
		break;
	case GENERIC_OBJ_BLOCK:
		for (std::list<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			if ((*i)->Coords() == pos) message = (*i)->Description();
		}
		break;
	case NPC_:
		for (std::list<int>::iterator i = characters.begin(); i != characters.end(); i++) {
			Character* npc = game->Get_char(*i);
			if (npc->Coords() == pos) message = npc->Name();
			game->Get_mainchar()->Encyclopedia()->Try_add(npc->Name(), npc->Description(), npc->Id() +CHAR_OFFSET);
		}
		break;
	case CNT_:
		for (std::list<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			if ((*i)->Coords() == pos) {
				game->Get_mainchar()->Set_interact_inventory((*i)->Get_inventory());
				game->Change_state(INV_TRANSFER);
			}
		}
		break;
	}
	game->Update_string(TILE_STRING, &message);
}

Connection* Map::Get_conn_data(sf::Vector2i pos){
	for (std::list<Connection>::iterator i = conns.begin(); i != conns.end(); i++) {
		if (i->entry == pos) return &(*i);
	}
}

int Map::Impassable(sf::Vector2i pos) {
	char c = map_arr[pos.y * dim_x + pos.x];
	if (c == '0' || c == '1') return 1;
	else {
		for (std::list<Object*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
			if ((*i)->Coords() == pos) return 1;
		}
		for (std::list<int>::const_iterator i = characters.begin(); i != characters.end(); i++) {
			if (game->Get_char(*i)->Coords() == pos) return 1;
		}
		return 0;
	}
}

int Map::Check_terrain(sf::Vector2i pos)	//returns 1 if there is a connection, 2 if the terrain is impassable and not an object, 3 if there's an object, 4 if there's an NPC and 0 if it is a regular tile
{
	if (Impassable(pos)) {
		for (std::list<Object*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
			if ((*i)->Coords() == pos&&(*i)->Type()==GENERIC) return GENERIC_OBJ_BLOCK;
		}
		for (std::list<Object*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
			if ((*i)->Coords() == pos && (*i)->Type() == CONTAINER) return CNT_;
		}
		for (std::list<int>::const_iterator i = characters.begin(); i != characters.end(); i++) {
			if (game->Get_char(*i)->Coords()==pos) return NPC_;
		}
		return IMPASSABLE_TERR;
	}
	else {
		for (std::list<Connection>::const_iterator i = conns.begin(); i != conns.end(); i++) {
			if (i->entry == pos) return CONN;
		}
	}
	return NORMAL;
}

void Map::Load(std::ifstream& f,sf::Texture* conn_texture) {
	std::string aux;
	int num_conns = 0;
	f >> aux;	//skip map ID
	f.ignore();
	getline(f, name, '\n');
	f >> dim_y >> dim_x;
	map_arr = (char*)malloc(sizeof(char) * dim_x * dim_y);
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(dim_x * dim_y * 4);
	for (int i = 0; i < dim_y; i++) {
		for (int j = 0; j < dim_x; j++) {
			char c;
			f >> map_arr[i * dim_x + j];
			c = map_arr[i * dim_x + j];
			sf::Vertex* quad_curr = &vertices[(i * dim_x + j) * 4];

			quad_curr[0].color = game->Get_color(c);
			quad_curr[1].color = game->Get_color(c);
			quad_curr[2].color = game->Get_color(c);
			quad_curr[3].color = game->Get_color(c);

			quad_curr[0].position = sf::Vector2f(TILEW * j, TILEH * i + upper_boundary);
			quad_curr[1].position = sf::Vector2f(TILEW * (j + 1), TILEH * i + upper_boundary);
			quad_curr[2].position = sf::Vector2f(TILEW * (j + 1), TILEH * (i + 1) + upper_boundary);
			quad_curr[3].position = sf::Vector2f(TILEW * j, TILEH * (i + 1) + upper_boundary);
		}
	}
	f >> aux;
	if (aux != "CONNECTIONS:") {
		std::cout << aux;
		exit(-13);
	}
	f >> num_conns;
	for (int i = 0; i < num_conns; i++) {
		Connection conn;
		f >> conn.entry.x >> conn.entry.y >> conn.exit.x >> conn.exit.y >> conn.exit_map_id;
		conn.texture = conn_texture;
		
		conn.vertices.setPrimitiveType(sf::Quads);
		conn.vertices.resize(4);
		sf::Vertex* quad = &conn.vertices[0];

		quad[0].position = sf::Vector2f(conn.entry.x * TILEW, conn.entry.y * TILEH + upper_boundary);
		quad[1].position = sf::Vector2f((conn.entry.x + 1) * TILEW, conn.entry.y * TILEH + upper_boundary);
		quad[2].position = sf::Vector2f((conn.entry.x + 1) * TILEW, (conn.entry.y+ 1) * TILEH + upper_boundary);
		quad[3].position = sf::Vector2f(conn.entry.x * TILEW, (conn.entry.y + 1) * TILEH + upper_boundary);

		quad[0].texCoords = sf::Vector2f(0, 0);
		quad[1].texCoords = sf::Vector2f(TILEW, 0);
		quad[2].texCoords = sf::Vector2f(TILEW, TILEH);
		quad[3].texCoords = sf::Vector2f(0, TILEH);

		conns.push_back(conn);
	}
	f >> aux;
	if (aux != ";;") {
		std::cout << aux;
		exit(-12);
	}
}

void Connection::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	state.texture = texture;
	target.draw(vertices,state);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	target.draw(vertices, state);
	for (std::list<Connection>::const_iterator i = conns.begin(); i != conns.end(); i++) {
		target.draw(*i);
	}
	for (std::list<Object*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
		target.draw(**i);
	}
	for (std::list<int>::const_iterator i = characters.begin(); i != characters.end(); i++) {
		target.draw(*game->Get_char(*i));
	}
}