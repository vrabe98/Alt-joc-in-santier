#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include "GameState.h"

#define MAX_STATES 20

class Map;
class Character;
class Inventory;
class Item;
class ItemSlot;
class Encyclopedia;
class MusicContainer;
class DialogueBox;
class CombatScreen;
class QuestContainer;

class GameWindow
{
	sf::RenderWindow render_wnd;
	GameState states[20];
	sf::Font* font;
	int retval;
public:
	GameWindow() {
		this->font = font;
		render_wnd.create(sf::VideoMode(1024,768), "ceva");
	}
	void Set_font(sf::Font* font) { this->font = font; }
	void Load_screen(std::ifstream&, int, int, int,int);
	void Render(Inventory*);
	void Render(Map*);
	void Render(Character*);
	void Render(ItemSlot** slots);
	void Render(Encyclopedia* encyclopedia);
	void Render(MusicContainer* playlist);
	void Render(DialogueBox* diag,int code);
	void Render(CombatScreen* combat);
	void Render(QuestContainer* quest_container);
	void Render(short int);
	void Override_state(int newstate) {
		retval = newstate;
	}
	int Ret(int current_state) {
		if (retval > -1) return retval;
		else return current_state;
	}
	void Disp() { render_wnd.display(); }
	bool IsOpen() {
		return render_wnd.isOpen();
	}
};