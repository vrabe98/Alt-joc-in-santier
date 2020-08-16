#include "Item.h"
#include "Game.h"

extern Game* game;

std::string Generic::Info()
{
	return name +Typestr()+ "\nWeight: " + std::to_string(weight) + "\nPrice: " + std::to_string(price);
}

std::string Weapon::Info()
{
	return name +Typestr()+"\nSlot: "+Slot_name()+"\nDamage: " + std::to_string(damage)+ "\nWeight: " + std::to_string(weight) + "\nPrice: " + std::to_string(price);
}

std::string Armor::Info()
{
	return name + Typestr()+"\nSlot: "+Slot_name()+"\nArmor: "+std::to_string(armor)+"\nWeight: " + std::to_string(weight) + "\nPrice: " + std::to_string(price);
}

std::string Shield::Info()
{
	return name + Typestr()+"\nWeight: " + std::to_string(weight) + "\nPrice: " + std::to_string(price);
}

void Generic::Load(std::ifstream& f){
	std::string aux;
	f >> id;
	f >> price >> weight;
	equip_slot = NONEQUIPABLE;
	f.ignore();
	std::getline(f, name, '\n');
	std::getline(f, description, '\t');
	f >> aux;
	texture.loadFromFile("Images//" + aux);
	f >> aux;
	if (aux != ";;") {
		std::cout << aux;
		exit(-18);
	}
}

void Weapon::Load(std::ifstream& f){
	std::string aux;
	f >> id;
	f >> price >> equip_slot>>weight>>damage;
	f.ignore();
	std::getline(f, name, '\n');
	std::getline(f, description, '\t');
	f >> aux;
	texture.loadFromFile("Images//" + aux);
	f >> aux;
	if (aux != ";;") {
		std::cout << aux;
		exit(-18);
	}
}

void Armor::Load(std::ifstream& f){
	std::string aux;
	f >> id;
	f >> price >> equip_slot >> weight>>armor;
	f.ignore();
	std::getline(f, name, '\n');
	std::getline(f, description, '\t');
	f >> aux;
	texture.loadFromFile("Images//" + aux);
	f >> aux;
	if (aux != ";;") {
		std::cout << aux;
		exit(-18);
	}
}

void Shield::Load(std::ifstream& f){
	std::string aux;
	f >> id;
	f >> price >> equip_slot >> weight;
	f.ignore();
	std::getline(f, name, '\n');
	std::getline(f, description, '\t');
	f >> aux;
	texture.loadFromFile("Images//" + aux);
	f >> aux;
	if (aux != ";;") {
		std::cout << aux;
		exit(-18);
	}
}
