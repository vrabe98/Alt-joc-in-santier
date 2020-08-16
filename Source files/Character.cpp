#include "Character.h"
#include <string>
#include "Map.h"
#include "Game.h"

#define HP_BASE 100.0

#define CHARACTER_ENCYCLOPEDIA_OFFSET 200

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define PLAYER_INV 0
#define NONPLAYER_INV 1

#define WIDTH 25
#define HEIGHT 25

#define upper_boundary 68

#define CHAR_NAME 0
#define CHAR_INFO 1
#define CHAR_HP 2
#define CHAR_MONEY 3
#define MAP_NAME 5
#define CHAR_INFO_FULL 7

#define NORMAL 0
#define CONN 1
#define IMPASSABLE_TERR 2
#define GENERIC_OBJ_BLOCK 3
#define NPC_ 4
#define CNT_ 5

extern Game* game;

void Character::RefreshHP() {			//function replenishes HP, by recalculating its value
	float base = 100.0;
	hp = HP_BASE * (1 + 0.05 * (strength - 10.0)) + constitution * 10.0;
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	state.texture = &texture;
	target.draw(sprite_vertices, state);
}

NPC::NPC(std::ifstream& f) {
	std::string aux;
	name = new std::string();
	f >> id;
	f.ignore();
	std::getline(f, *name, '\n');
	std::getline(f, description, '\t');
	f >> strength >> dexterity >> constitution >> charisma >> currency >> current_map >> posx >> posy;
	f >> aux;
	texture.loadFromFile("Images//" + aux);
	sprite_vertices.setPrimitiveType(sf::Quads);
	sprite_vertices.resize(4);
	sf::Vertex* quad = &sprite_vertices[0];

	quad[0].position = sf::Vector2f(posx * WIDTH, posy * HEIGHT + upper_boundary);
	quad[1].position = sf::Vector2f((posx + 1) * WIDTH, posy * HEIGHT + upper_boundary);
	quad[2].position = sf::Vector2f((posx + 1) * WIDTH, (posy + 1) * HEIGHT + upper_boundary);
	quad[3].position = sf::Vector2f(posx * WIDTH, (posy + 1) * HEIGHT + upper_boundary);

	quad[0].texCoords = sf::Vector2f(0, 0);
	quad[1].texCoords = sf::Vector2f(WIDTH, 0);
	quad[2].texCoords = sf::Vector2f(WIDTH, HEIGHT);
	quad[3].texCoords = sf::Vector2f(0, HEIGHT);

	f >> aux;
	if (aux != ";;") {
		exit(-10);
	}
	RefreshHP();
}

void NPC::Update_info(){

}

Main_character::Main_character(std::ifstream& f)
{
	std::string aux;
	inventar = new ::Inventory(PLAYER_INV);
	id = 0;
	std::getline(f, description, '\t');
	f >> strength >> dexterity >> constitution >> charisma >> currency >> current_map>>posx>>posy;
	for (int i = 0; i < NUM_SLOTS; i++) {
		int x=0, y=0, xdup=0, ydup=0;
		f >> x >> y;
		if (i == ARMS) {
			f >> xdup>>ydup;
		}
		slots[i] = new ItemSlot(sf::Vector2f(x, y), i,sf::Vector2f(xdup, ydup));
	}
	while (true) {
		f >> aux;
		if (aux == ";") break;
		else {
			int item_nr = stoi(aux);
			inventar->Add_item(game->Get_item(item_nr));
		}
	}
	f >> aux;
	texture.loadFromFile("Images//" + aux);
	sprite_vertices.setPrimitiveType(sf::Quads);
	sprite_vertices.resize(4);
	sf::Vertex* quad = &sprite_vertices[0];
	
	quad[0].position = sf::Vector2f(posx*WIDTH,posy*HEIGHT+upper_boundary);
	quad[1].position = sf::Vector2f((posx+1)*WIDTH,posy*HEIGHT+upper_boundary);
	quad[2].position = sf::Vector2f((posx+1)*WIDTH,(posy+1)*HEIGHT+upper_boundary);
	quad[3].position = sf::Vector2f(posx*WIDTH,(posy+1)*HEIGHT+upper_boundary);

	quad[0].texCoords = sf::Vector2f(0, 0);
	quad[1].texCoords = sf::Vector2f(WIDTH, 0);
	quad[2].texCoords = sf::Vector2f(WIDTH, HEIGHT);
	quad[3].texCoords = sf::Vector2f(0, HEIGHT);

	f >> aux;
	if (aux != ";;") {
		exit(-10);
	}
	RefreshHP();
}

void Main_character::LMB_Pressed(sf::Vector2i pos, int state) {
	if (state == CHARACTER_VIEW) {
		if (inventar->MouseWithinBounds(pos)) {
			inventar->LMB_Pressed(pos);
			InventorySlot* item = inventar->Get_highlighted();
			if (item != nullptr)	encyclopedia.Try_add(item->Get_item()->Name(), item->Get_item()->Description(), item->Get_item()->Id());
		}
		else {
			InventorySlot* item = inventar->Get_highlighted();
			RetItem* out = nullptr;
			if (item != nullptr) {
				for (int i = 0; i < NUM_SLOTS; i++) {
					if (slots[i]->MouseWithinBounds(pos)) {
						out = &slots[i]->Add_item(item->Get_item(), item->Get_item()->Equip_slot());
						if (out != nullptr && out->code) {
							inventar->Remove_item(item->Order());
							inventar->Add_item(out->item);
						}
					}
				}
			}
		}
	}
	else if (state == ENCYCLOPEDIA) {
		if (encyclopedia.MouseWithinBounds(pos)) encyclopedia.LMB_Pressed(pos);
	}
	else if (state == INV_TRANSFER) {
		if (inventar->MouseWithinBounds(pos)) inventar->LMB_Pressed(pos);
		else if (interact_inventory->MouseWithinBounds(pos)) interact_inventory->LMB_Pressed(pos);
	}
}

void Main_character::Getname(){
	name = game->Get_string_from_db(CHAR_NAME);
}

void Main_character::Update_info(){
	std::string info = "";
	RefreshHP();
	info ="INFO:\n\nName: "+*name + "\n\n'"+description+"'\n\nSTR: " + std::to_string(strength) + "\nDEX: " + std::to_string(dexterity) + "\nCON: " + std::to_string(constitution) + "\nCHA: " + std::to_string(charisma) + "\n\nHP: "+std::to_string((int)hp)+"\nValuta: "+std::to_string((int)currency)+" $";
	game->Update_string(CHAR_INFO, &info);
	info ="HP: "+std::to_string((int)hp);
	game->Update_string(CHAR_HP, &info);
	info = std::to_string((int)currency)+" $";
	game->Update_string(CHAR_MONEY,&info);
	info = game->Get_map(current_map)->Name();
	game->Update_string(MAP_NAME, &info);
	info = "INFO:\n\nName: " + *name+"\n\n'" + description + "'\n\nFortza: " + std::to_string(strength) + "\nSmecherie: " + std::to_string(dexterity) + "\nBarosaneala: " + std::to_string(constitution) + "\nScoala vieti: " + std::to_string(charisma) + "\n\nHP: " + std::to_string((int)hp) + "\nArmura: "+std::to_string(Armor())+"\nDaune cu stanga: "+std::to_string(Offhand_Damage())+"\nDaune cu dreapta: "+std::to_string(Mainhand_Damage())+"\nValuta: " + std::to_string((int)currency) + " $";
	game->Update_string(CHAR_INFO_FULL, &info);
	encyclopedia.Update(*name, description, 0 + CHARACTER_ENCYCLOPEDIA_OFFSET);
}

void Character::Move(int keycode) {
	int posx_b = posx, posy_b = posy, terr;
	switch (keycode) {
	case UP: posy--;
		break;
	case DOWN:posy++;
		break;
	case LEFT:posx--;
		break;
	case RIGHT:posx++;
		break;
	}
	game->Tile_action(sf::Vector2i(posx, posy), current_map);
	terr = game->Check_terrain(current_map, sf::Vector2i(posx, posy));
	if (terr==IMPASSABLE_TERR|| terr == GENERIC_OBJ_BLOCK|| terr == NPC_||terr==CNT_) {
		posx=posx_b;
		posy=posy_b;
	}
	else {
		sf::Vertex* quad = &sprite_vertices[0];
		quad[0].position = sf::Vector2f(posx * WIDTH, posy * HEIGHT + upper_boundary);
		quad[1].position = sf::Vector2f((posx + 1) * WIDTH, posy * HEIGHT + upper_boundary);
		quad[2].position = sf::Vector2f((posx + 1) * WIDTH, (posy + 1) * HEIGHT + upper_boundary);
		quad[3].position = sf::Vector2f(posx * WIDTH, (posy + 1) * HEIGHT + upper_boundary);
	}
}

void Main_character::Action(){
	if (game->Check_terrain(current_map, sf::Vector2i(posx, posy)) == 1) {
		Connection* conn = game->Get_conn(sf::Vector2i(posx, posy), current_map);
		posx = conn->exit.x;
		posy = conn->exit.y;
		current_map = conn->exit_map_id;
		sf::Vertex* quad = &sprite_vertices[0];
		quad[0].position = sf::Vector2f(posx * WIDTH, posy * HEIGHT + upper_boundary);
		quad[1].position = sf::Vector2f((posx + 1) * WIDTH, posy * HEIGHT + upper_boundary);
		quad[2].position = sf::Vector2f((posx + 1) * WIDTH, (posy + 1) * HEIGHT + upper_boundary);
		quad[3].position = sf::Vector2f(posx * WIDTH, (posy + 1) * HEIGHT + upper_boundary);
	}
	else if (game->Check_terrain(current_map, sf::Vector2i(posx, posy)) == 5) {
		
	}
}

void ItemSlot::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	if(item!=nullptr){
		state.texture = item->Get_texture();
		target.draw(img_vertices, state);
		if (this->slot == ARMS) target.draw(duplicate, state);
	}
	else {
		target.draw(img_vertices);
		if (this->slot == ARMS) target.draw(duplicate);
	}
}

int ItemSlot::MouseWithinBounds(sf::Vector2i pos){
	return (img_vertices[0].position.x <= (float)pos.x) && ((float)pos.x <= img_vertices[1].position.x) && (img_vertices[0].position.y <= (float)pos.y) && ((float)pos.y <= img_vertices[3].position.y);
}

RetItem ItemSlot::Add_item(Item* item,int slot){	//will return nullptr when the operation fails,twohanders are not yet implemented
	RetItem result;
	result.item = this->item;
	if (this->slot != item->Equip_slot()) result.code = 0;
	else {
		this->item = item;
		result.code = 1;
	}
	return result;
}

ItemSlot::ItemSlot(sf::Vector2f pos,int slot,sf::Vector2f duplicatepos){
	this->item = nullptr;
	this->slot = slot;
	img_vertices.setPrimitiveType(sf::Quads);
	img_vertices.resize(4);
	sf::Vertex* quad = &img_vertices[0];
	
	quad[0].position = pos;
	quad[1].position = sf::Vector2f(pos.x + SLOTCW, pos.y);
	quad[2].position = sf::Vector2f(pos.x + SLOTCW, pos.y + SLOTCH);
	quad[3].position = sf::Vector2f(pos.x, pos.y + SLOTCH);

	quad[0].texCoords = sf::Vector2f(0, 0);
	quad[1].texCoords = sf::Vector2f(SLOTCW, 0);
	quad[2].texCoords = sf::Vector2f(SLOTCW, SLOTCH);
	quad[3].texCoords = sf::Vector2f(0, SLOTCH);

	for (int i = 0; i < 4; i++) {
		quad[i].color = sf::Color::White;
	}

	duplicate.setPrimitiveType(sf::Quads);
	duplicate.resize(4);

	if (duplicatepos != sf::Vector2f(0, 0)) {
		sf::Vertex* quad = &duplicate [0] ;

		quad[0].position = duplicatepos;
		quad[1].position = sf::Vector2f(duplicatepos.x + SLOTCW, duplicatepos.y);
		quad[2].position = sf::Vector2f(duplicatepos.x + SLOTCW, duplicatepos.y + SLOTCH);
		quad[3].position = sf::Vector2f(duplicatepos.x, duplicatepos.y + SLOTCH);

		quad[0].texCoords = sf::Vector2f(0, 0);
		quad[1].texCoords = sf::Vector2f(SLOTCW, 0);
		quad[2].texCoords = sf::Vector2f(SLOTCW, SLOTCH);
		quad[3].texCoords = sf::Vector2f(0, SLOTCH);

		for (int i = 0; i < 4; i++) {
			quad[i].color = sf::Color::White;
		}
	}
}
