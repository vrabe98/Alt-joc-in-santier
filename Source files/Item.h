#pragma once
#include <string>
#include <SFML/Graphics.hpp>

#define GENERIC 0
#define WEAPON 1
#define ARMOR 2
#define SHIELD 3

#define CHEST 0
#define HEAD 1
#define LEGS 2
#define ARMS 3
#define FEET 4
#define RHAND 5
#define LHAND 6
#define NONEQUIPABLE - 1

#define NUM_SLOTS 7

#define IMGW 50
#define IMGH 50

class Item{
protected:
	std::string name, description;
	sf::Texture texture;
	int weight, equip_slot,id;
	float price;
	std::string Slot_name() {
		switch (equip_slot) {
		case CHEST: return "Chest";
			break;
		case HEAD: return "Head";
			break;
		case LEGS: return "Legs";
			break;
		case ARMS: return "Arms";
			break;
		case FEET: return "Feet";
			break;
		case RHAND: return "Right hand";
			break;
		case LHAND: return "Left hand";
			break;
		}
	}
public:
	virtual ~Item(){}
	int Id() { return id; }
	float Price() { return price; }
	int Equip_slot() { return equip_slot; }
	std::string Name() { return name; }
	std::string Description() { return description; }
	sf::Texture* Get_texture() { return &texture; }
	virtual std::string Typestr() = 0;
	virtual std::string Info()=0;
	virtual float _Damage() = 0;
	virtual int _Armor()=0;
	virtual int Type() = 0;
	virtual void Load(std::ifstream&)=0;
};

class Generic :public Item {
public:
	int _Armor() override { return 0; }
	float _Damage() override { return 0; }
	int Type() override { return GENERIC; }
	std::string Typestr() override { return "\n\nGeneric item"; }
	std::string Info() override;
	void Load(std::ifstream& f) override;
};

class Weapon :public Item {
	float damage;
public:
	int _Armor() override { return 0; }
	float _Damage() override { return damage; }
	int Type() override { return WEAPON; }
	std::string Typestr() override { return "\n\nWeapon"; }
	std::string Info() override;
	void Load(std::ifstream& f) override;
};

class Armor :public Item {
	int armor;
public:
	int _Armor() override { return armor; }
	float _Damage() override { return 0; }
	int Type() override { return ARMOR; }
	std::string Typestr() override { return "\n\nArmor"; }
	std::string Info() override ;
	void Load(std::ifstream& f) override;
};

class Shield :public Item {
public:
	int _Armor() override { return 0; }
	float _Damage() override { return 0; }
	int Type() override { return SHIELD; }
	std::string Typestr() override { return "\n\nShield"; }
	std::string Info() override;
	void Load(std::ifstream& f) override;
};