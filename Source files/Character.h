#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "Encyclopedia.h"
#include "Inventory.h"

#define SLOTCW 50
#define SLOTCH 50

#define CHARACTER_VIEW 4
#define ENCYCLOPEDIA 5
#define INV_TRANSFER 7

struct RetItem {
	Item* item;
	int code;
};

class ItemSlot:public sf::Drawable {
	int slot;
	Item* item;
	sf::VertexArray img_vertices;
	sf::VertexArray duplicate;	//only used for ARMS
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	Item* Get_item() {
		return item;
	}
	int MouseWithinBounds(sf::Vector2i pos);
	RetItem Add_item(Item* item,int slot);
	ItemSlot(sf::Vector2f pos, int slot,sf::Vector2f duplicatepos=sf::Vector2f(0,0));
	ItemSlot(){}
};

class Character:public sf::Drawable
{
protected:
	int id;
	sf::VertexArray sprite_vertices;
	std::string* name;
	std::string description;
	sf::Texture texture;
	ItemSlot* slots[NUM_SLOTS];
	int strength, dexterity, constitution, charisma, current_map, posx, posy;
	float hp, currency;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	Character(){}
	Character(std::ifstream&);
	ItemSlot** Get_slots() { return slots; }
	int Armor() {
		int armor = 0;
		for (int i = 0; i < NUM_SLOTS; i++) {
			if (slots[i]->Get_item() != nullptr) armor += slots[i]->Get_item()->_Armor();
		}
		return armor;
	}
	float Offhand_Damage() {
		if (slots[LHAND]->Get_item() != nullptr) return slots[LHAND]->Get_item()->_Damage();
		else return 0;
	}
	float Mainhand_Damage() {
		if (slots[RHAND]->Get_item() != nullptr) return slots[RHAND]->Get_item()->_Damage();
		else return 0;
	}
	int Id() { return id; }
	virtual void Update_info()=0;
	void RefreshHP();
	void Move(int);
	sf::Vector2i Coords() { return sf::Vector2i(posx, posy); }
	std::string Name() { return *name; }
	std::string Description() { return description; }
	ItemSlot** Slots() { return slots; }
	int Get_map() {
		return current_map;
	}
	int STR() {
		return strength;
	}
	int DEX() {
		return dexterity;
	}
	int CON() {
		return constitution;
	}
	int CHA() {
		return charisma;
	}
	void Inc_STR() {
		strength++;
	}
	void Inc_DEX() {
		dexterity++;
	}
	void Inc_CON() {
		constitution++;
	}
	void Inc_CHA() {
		charisma++;
	}
};

class Main_character : public Character {
	Encyclopedia encyclopedia;
	Inventory* inventar;
	Inventory* interact_inventory;
public:
	Main_character() {}
	Main_character(std::ifstream&);
	Inventory* Get_inventory_interact() {
		return interact_inventory;
	}
	void Action();
	void Getname();
	void Update_info();
	void Set_interact_inventory(Inventory* inv) {
		interact_inventory = inv;
	}
	void LMB_Pressed(sf::Vector2i pos, int state);
	void Wheel_Scroll(sf::Vector2i pos, int delta, int state) {
		if (state == CHARACTER_VIEW && inventar->MouseWithinBounds(pos)) inventar->Wheel_scroll(delta);
		else if (state == ENCYCLOPEDIA && encyclopedia.MouseWithinBounds(pos)) encyclopedia.Wheel_scroll(delta);
		else if (state == INV_TRANSFER) {
			if (inventar->MouseWithinBounds(pos)) inventar->Wheel_scroll(delta);
			else if (interact_inventory->MouseWithinBounds(pos)) interact_inventory->Wheel_scroll(delta);
		}
	}
	Inventory* Inventory() { return inventar; }
	Encyclopedia* Encyclopedia() { return &encyclopedia; }
};

class NPC :public Character {
public:
	NPC(){}
	NPC(std::ifstream&);
	void Update_info();
};