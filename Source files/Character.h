#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "Encyclopedia.h"
#include "Inventory.h"
#include "DialogueState.h"
#include "Quest.h"

#define HP_BASE 200.0

#define FIST_DMG 50.0

#define SLOTCW 50
#define SLOTCH 50

#define CHARACTER_VIEW 4
#define ENCYCLOPEDIA 5
#define INV_TRANSFER 7
#define TRADE_SCREEN 10

struct RetItem {
	Item* item;
	Item* extra;
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
	bool MouseWithinBounds(sf::Vector2i pos);
	RetItem Add_item(Item* item,int slot);
	ItemSlot(sf::Vector2f pos, int slot,sf::Vector2f duplicatepos=sf::Vector2f(0,0));
	ItemSlot(){}
};

class Character:public sf::Drawable
{
protected:
	bool damage_bonus;
	int id;
	Inventory* inventar;
	sf::VertexArray sprite_vertices;
	std::string* name;
	std::string description;
	sf::Texture texture, dialogue_texture;
	ItemSlot* slots[NUM_SLOTS];
	std::list<Quest_flag> set_flags;	//flags set on death but will be expanded later i think
	int strength, dexterity, constitution, charisma, current_map, posx, posy;
	float hp, currency;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	virtual ~Character(){}
	Character(){}
	virtual DialogueState* Dialogue()=0;
	virtual void Load_dialogue(std::ifstream& f)=0;
	virtual bool Is_Vendor() = 0;
	bool Try_update_currency(float diff);
	ItemSlot** Get_slots() { return slots; }
	bool HasDmgBonus() {
		return damage_bonus;
	}
	void SetDmgBonus() {
		damage_bonus = true;
	}
	void ResetDmgBonus() {
		damage_bonus = false;
	}
	void HP_clamp() {
		if (hp <= 0) {
			texture.loadFromFile("Images/deadchar.png");
			hp = 0;
		}
	}
	int Died();
	int DualWield() {
		if (slots[LHAND]->Get_item() == nullptr) return 0;
		else if (slots[LHAND]->Get_item()->Type() == SHIELD) return 0;
		else if (Using_fists()) return 0;
		else if (slots[RHAND]->Get_item()->Type() == WEAPON && (!slots[LHAND]->Get_item()->TwoHanded())) return 1;
	}
	int Has_1h() {
		return !(slots[RHAND]->Get_item() != nullptr && slots[RHAND]->Get_item()->TwoHanded());
	}
	int Has_shield() {
		return slots[LHAND]->Get_item() != nullptr && slots[LHAND]->Get_item()->Type() == SHIELD;
	}
	float Evasion() {
		return 0.5f * (DEX() - 10) + 0.5f * (!Has_shield());
	}
	float Hit_bonus() {
		return 0.5f * (DEX() - 10) + 0.5f * Has_1h();
	}
	int Armor() {
		int armor = 0;
		for (int i = 0; i < NUM_SLOTS-2; i++) {	//skips hands
			if (slots[i]->Get_item() != nullptr) armor += slots[i]->Get_item()->_Armor();
		}
		return armor;
	}
	float Offhand_Damage() {
		if (slots[LHAND]->Get_item() != nullptr) return slots[LHAND]->Get_item()->_Damage();
		else return FIST_DMG;
	}
	float Mainhand_Damage() {
		if (slots[RHAND]->Get_item() != nullptr) return slots[RHAND]->Get_item()->_Damage();
		else return FIST_DMG;
	}
	bool Using_fists() {
		return slots[RHAND]->Get_item() == nullptr;
	}
	void Defend(float dmg,Character* enemy,int riposte, int critted);
	float HP() { return hp; }
	float Max_HP(){	return HP_BASE * (1 + 0.05 * (strength - 10.0)) + constitution * 10.0;}
	int Id() { return id; }
	virtual void Update_info()=0;
	void RefreshHP();
	void Move(int);
	Inventory* Inventar() { return inventar; }
	sf::Texture* Diag_texture() { return &dialogue_texture; }
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
	Inventory* interact_inventory;
	std::list<Quest*> quests;
public:
	Main_character() {}
	Main_character(std::ifstream&);
	Inventory* Get_inventory_interact() {
		return interact_inventory;
	}
	DialogueState* Dialogue() { return nullptr; }
	bool Is_Vendor() { return false; }
	void Load_dialogue(std::ifstream& f) {}
	void Action();
	void Getname();
	void Update_info();
	void Set_interact_inventory(Inventory* inv) {
		interact_inventory = inv;
	}
	void Add_quest(int id);
	void Refresh_quests();
	void LMB_Pressed(sf::Vector2i pos, int state);
	void Wheel_Scroll(sf::Vector2i pos, int delta, int state) {
		if (state == CHARACTER_VIEW && inventar->MouseWithinBounds(pos)) inventar->Wheel_scroll(delta);
		else if (state == ENCYCLOPEDIA && encyclopedia.MouseWithinBounds(pos)) encyclopedia.Wheel_scroll(delta);
		else if (state == INV_TRANSFER||state==TRADE_SCREEN) {
			if (inventar->MouseWithinBounds(pos)) inventar->Wheel_scroll(delta);
			else if (interact_inventory->MouseWithinBounds(pos)) interact_inventory->Wheel_scroll(delta);
		}
	}
	Encyclopedia* Encyclopedia() { return &encyclopedia; }
};

class NPC :public Character {
protected:
	DialogueState** dialogue_entry;
public:
	NPC(){}
	NPC(std::ifstream&,int);
	DialogueState* Dialogue() { return dialogue_entry[0]; }
	DialogueState* Get_state(int id) {
		return dialogue_entry[id];
	}
	bool Is_Vendor() { return false; }
	void Load_dialogue(std::ifstream& f);
	void Update_info();
};

class Vendor :public NPC {
public:
	Vendor() {}
	Vendor(std::ifstream&,int);
	bool Is_Vendor() { return true; }
};