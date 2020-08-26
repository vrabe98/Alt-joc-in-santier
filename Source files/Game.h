#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include "GameWindow.h"
#include "Character.h"
#include "Map.h"
#include "Item.h"
#include "MusicContainer.h"
#include "DialogueBox.h"

class Main_character;

#define MAX_STRINGS 100
#define NUM_COLORS 8
#define MAX_MAPS 100
#define MAX_ITEMS 100
#define MAX_NPCS 100

#define black 0
#define white 1
#define red 2
#define green 3
#define blue 4
#define yellow 5
#define magenta 6
#define cyan 7

#define EXIT 0 
#define MAIN_MENU 1
#define CHARACTER_CREATION 2
#define MAP_VIEW 3
#define CHARACTER_VIEW 4
#define ENCYCLOPEDIA 5
#define MUSIC 6
#define INV_TRANSFER 7

class Game
{
	int state;						//current state to be rendered;
	std::string* string_db[MAX_STRINGS];
	sf::Color color_db[NUM_COLORS];
	sf::Texture connection_texture;
	sf::Font font;
	Map* maps[MAX_MAPS];
	Item* items[MAX_ITEMS];
	Character* npcs[MAX_NPCS];
	Main_character* main_char;
	GameWindow* main_window;
	DialogueBox diag;
	MusicContainer playlist;
public:
	Game() {
		state = MAIN_MENU;
		color_db[black] = sf::Color::Black;
		color_db[white] = sf::Color::White;
		color_db[red] = sf::Color::Red;
		color_db[green] = sf::Color::Green;
		color_db[blue] = sf::Color::Blue;
		color_db[yellow] = sf::Color::Yellow;
		color_db[magenta] = sf::Color::Magenta;
		color_db[cyan] = sf::Color::Cyan;
		main_window = new GameWindow();
	}
	MusicContainer* Get_playlist() {
		return &playlist;
	}
	void Start_dialogue(int npc_id);
	void LMB_Pressed(sf::Vector2i pos){
		diag.LMB_Press(pos);
	}
	std::string* Alloc_string(int, std::string="");
	DialogueBox* Get_dialoguebox() { return &diag; }
	Main_character* Get_mainchar() { return main_char; }
	Item* Get_item(int i) { return items[i]; }
	Character* Get_char(int i) { return npcs[i]; }
	void MouseMoved(sf::Vector2i pos);
	void Change_state(int newstate) {
		state = newstate;
	}
	const sf::Color Get_color(char code) {
		return color_db[code-'0'];
	}
	inline std::string* Get_string_from_db(int code) {
		return string_db[code];
	}
	inline void Update_string(int code, std::string* newstr) {
		*string_db[code] = *newstr;
	}
	sf::Font* Get_font() { return &font; }
	Map* Get_map(int map_id) {
		return maps[map_id];
	}
	Connection* Get_conn(sf::Vector2i pos, int map) { return maps[map]->Get_conn_data(pos); }
	void Tile_action(sf::Vector2i pos, int map) { maps[map]->Tile_action(pos); }
	int Check_terrain(int,sf::Vector2i);
	void Alloc_strings();
	void Load_dialogues(std::ifstream&);
	void Load_npcs(std::ifstream&);
	void Load_music(std::ifstream&);
	void Load_objects(std::ifstream&);
	void Load_screens(std::ifstream&);
	void Load_mainchar(std::ifstream&);
	void Load_maps(std::ifstream&);
	void Load_item_db(std::ifstream&);
	void Load();
	void Play();
};

