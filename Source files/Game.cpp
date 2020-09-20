#include "Game.h"
#include <Windows.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define CHAR_NAME 0
#define CHAR_INFO 1
#define CHAR_HP 2
#define CHAR_MONEY 3
#define TILE_STRING 4
#define MAP_NAME 5
#define ITEM_STRING 6
#define CHAR_INFO_FULL 7
#define ENCYCLO_DESCR 8
#define CURRENT_SONG 9
#define PLAY_TIME 10
#define NPC_HP 11
#define MAINCHAR_HP 12
#define QUEST_INFO 13
#define TRADE_MESSAGE 14

#define NUM_STRINGS 15

void Game::Play() {
	playlist.Begin_timecalc();
	while (main_window->IsOpen()&&state!=EXIT) {
		main_char->Update_info();
		main_char->Refresh_quests();
		if (!diag.Active()) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
				main_char->Move(UP);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
				main_char->Move(DOWN);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
				main_char->Move(LEFT);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
				main_char->Move(RIGHT);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
				main_char->Action();
				Sleep(100);
			}
		}
		main_window->Render(state);
		switch (state) {
		case MAP_VIEW:
			main_window->Render(maps[main_char->Get_map()]);
			main_window->Render(main_char);
			if (diag.Active()) {
				if (diag.Get_npc()->Is_Vendor())
					main_window->Render(&diag, VENDOR_DIAG);
				else main_window->Render(&diag, NPC_DIAG);
			}
			if (combat.Active()) main_window->Render(&combat);
			break;
		case CHARACTER_VIEW:
			main_window->Render(main_char->Inventar());
			main_window->Render(main_char->Get_slots());
			break;
		case ENCYCLOPEDIA:
			main_window->Render(main_char->Encyclopedia());
			break;
		case MUSIC:
			main_window->Render(&playlist);
			break;
		case INV_TRANSFER:
			main_window->Render(main_char->Inventar());
			main_window->Render(main_char->Get_inventory_interact());
			break;
		case QUEST_SCREEN:
			main_window->Render(&quest_container);
			break;
		case TRADE_SCREEN:
			main_window->Render(main_char->Inventar());
			main_window->Render(main_char->Get_inventory_interact());
			break;
		}
		main_window->Disp();
		if (state != DEATH && state != TRADE_SCREEN) Change_state(main_window->Ret(state));
		else if (main_window->Ret(state) == EXIT) Change_state(EXIT);
		else if (main_window->Ret(state) == MAP_VIEW) Change_state(MAP_VIEW);
		Sleep(25);
	}
}

void Game::Load_screens(std::ifstream& screen_file) {
	int id, num_panels, num_buttons, num_textboxes;
	std::string aux;
	for (int i = 0; i < 36; i++) {	//skip header
		std::getline(screen_file, aux, '\n');
	}
	while (!screen_file.eof()) {
		screen_file >> aux;
		if (aux != "SCREEN:") {
			exit(-1);
		}
		else {
			screen_file >> id >> num_panels >> num_buttons >> num_textboxes;
			main_window->Load_screen(screen_file, id, num_panels, num_buttons, num_textboxes);
		}
	}
}


void Game::Load_mainchar(std::ifstream& char_file) {
	std::string aux;
	for (int i = 0; i < 13; i++) {	//skip header
		std::getline(char_file, aux, '\n');
	}
	main_char = new Main_character(char_file);
}

void Game::Load_maps(std::ifstream& map_file){
	std::string aux;
	int num_maps = 0;
	connection_texture.loadFromFile("Images//arrow.png");
	for (int i = 0; i < 11; i++) {	//skip header
		std::getline(map_file , aux, '\n');
	}
	map_file >> num_maps;
	map_file >> aux;
	if (aux != ";;") {
		exit(-11);
	}
	for (int i = 0; i < num_maps; i++) {
		maps[i] = new Map();
		maps[i]->Load(map_file,&connection_texture);
	}
	Alloc_string(TILE_STRING);
}

void Game::Load_item_db(std::ifstream& item_file){
	int num_items;
	std::string aux;
	for (int i = 0; i < 22; i++) {	//skip header
		std::getline(item_file, aux, '\n');
	}
	item_file >> num_items >> aux;
	if (aux != ";;") {
		exit(-17);
	}
	for (int i = 0; i < num_items; i++) {
		item_file >> aux;
		if (aux == "Generic") {
			Generic* item = new Generic();
			item->Load(item_file);
			items[i] = item;
		}
		else if (aux == "Weapon") {
			Weapon* item = new Weapon();
			item->Load(item_file);
			items[i] = item;
		}
		else if (aux == "Armor") {
			Armor* item = new Armor();
			item->Load(item_file);
			items[i] = item;
		}
		else if (aux == "Shield") {
			Shield* item = new Shield();
			item->Load(item_file);
			items[i] = item;
		}
	}
}

void Game::Load_objects(std::ifstream& obj_file) {
	int num_objects;
	std::string aux;
	for (int i = 0; i < 9; i++) {	//skip header
		std::getline(obj_file, aux, '\n');
	}
	obj_file >> num_objects;
	obj_file >> aux;
	if (aux != ";;") {
		exit(-16);
	}
	for (int i = 0; i < num_objects; i++) {
		Object* obj=nullptr;
		obj_file >> aux;
		if (aux == "Generic") {
			Generic_object* object=new Generic_object();
			object->Load(obj_file);
			obj = object;
		}
		else if (aux == "Container") {
			Container_object* object = new Container_object();
			object->Load(obj_file);
			obj = object;
		}
		maps[obj->Map()]->Add_object(obj);
	}
}

void Game::Load_music(std::ifstream& music_file) {
	std::string name;
	while (!music_file.eof()) {
		std::getline(music_file,name, '\n');
		playlist.Add_song(name);
	}
}

void Game::Load_npcs(std::ifstream& npc_file) {
	std::string aux;
	int num_npcs;
	for (int i = 0; i < 16; i++) {	//skip header
		std::getline(npc_file, aux, '\n');
	}
	npc_file >> num_npcs;
	npc_file >> aux;
	if (aux != ";;") {
		exit(-16);
	}
	for (int i = 0; i < num_npcs; i++) {
		int id;
		Character* npc;
		npc_file >> id >> aux;
		if (aux == "NPC") {
			npc = new NPC(npc_file,id);
			npcs[i] = npc;
			maps[npc->Get_map()]->Add_character(i);
		}
		else if (aux == "Vendor") {
			npc = new Vendor(npc_file, id);
			npcs[i] = npc;
			maps[npc->Get_map()]->Add_character(i);
		}
	}
}

void Game::Load_dialogues(std::ifstream& dialogue_file) {
	std::string aux;
	int i = 0;
	for (int i = 0; i < 18; i++) {	//skip header
		std::getline(dialogue_file, aux, '\n');
	}
	while (!dialogue_file.eof()) {
		int id;
		dialogue_file >> id;
		dialogue_file.ignore();
		std::getline(dialogue_file, aux, '\n');
		if (aux != npcs[id-1]->Name()) {
			std::cout << "\nNPC dialogue file error! NPC name and ID mismatch!";
			exit(-20);
		}
		npcs[i]->Load_dialogue(dialogue_file);
		i++;
	}
}

void Game::Load_quest_db(std::ifstream& quest_file) {
	std::string aux;
	int i = 0;
	for (int i = 0; i < 14; i++) {	//skip header
		std::getline(quest_file, aux, '\n');
	}
	i = 0;
	while (!quest_file.eof()) {
		quest_file >> aux;
		if (aux == "Regular") {
			Quest* quest = new Regular_quest();
			quest->Load(quest_file);
			quests[i] = quest;
		}
		else if (aux == "Chain") {
			Quest* quest = new Chain_quest();
			quest->Load(quest_file);
			quests[i] = quest;
		}
		i++;
	}
}

void Game::Load() {
	std::ifstream screen_file("Data/Screens.txt", std::ifstream::in);
	std::ifstream char_file("Data/Character.txt", std::ifstream::in);
	std::ifstream map_file("Data/Maps.txt", std::ifstream::in);
	std::ifstream obj_file("Data/Objects.txt", std::ifstream::in);
	std::ifstream item_file("Data/ItemDB.txt", std::ifstream::in);
	std::ifstream music_file("Data/Music.txt", std::ifstream::in);
	std::ifstream npc_file("Data/NPCs.txt", std::ifstream::in);
	std::ifstream dialogue_file("Data/Dialogues.txt", std::ifstream::in);
	std::ifstream quest_file("Data/Quests.txt", std::ifstream::in);
	Alloc_strings();
	quest_complete.loadFromFile("Images//quest_completed.png");
	quest_in_progress.loadFromFile("Images//quest_in_progress.png");
	while (!font.loadFromFile("Fonts//TERMINUS.TTF")) {}
	main_window->Set_font(&font);
	Load_item_db(item_file);
	Load_quest_db(quest_file);
	Load_mainchar(char_file);
	Load_screens(screen_file);
	Load_maps(map_file);
	Load_objects(obj_file);
	Load_music(music_file);
	Load_npcs(npc_file);
	Load_dialogues(dialogue_file);
	diag.Create();
	combat.Create_screen();
	main_char->Getname();
	quest_container.Init_text();
}

void Game::Set_quest_flag(Quest_flag flag){
	std::map<std::string,Quest_flag>::iterator i = unclaimed_flags.find(flag.Name());
	if (i != unclaimed_flags.end()) {
		(*i).second.Change_counter(flag.Counter());
	}
	else {
		unclaimed_flags[flag.Name()] = flag;
	}
}

void Game::Start_dialogue(int npc_id){
	if (!diag.Active()&&!npcs[npc_id]->Died()) {
		diag.Activate_deactivate();
		diag.Enter_dialogue(npcs[npc_id]->Dialogue(),npc_id,PRELIMINARY);
	}
}

void Game::Start_combat(int npc_id){
	if (!combat.Active()) {
		diag.Activate_deactivate();
		combat.Activate_deactivate();
		combat.Enter_combat(npc_id);
	}
}

std::string* Game::Alloc_string(int code, std::string string) {
	string_db[code] = new std::string(string);
	return string_db[code];
}

void Game::MouseMoved(sf::Vector2i pos){
	diag.MouseMoved(pos);
}

int Game::Check_terrain(int map, sf::Vector2i pos) {
	return maps[map]->Check_terrain(pos);
}

void Game::Alloc_strings(){
	for (int i = 0; i < NUM_STRINGS; i++) Alloc_string(i);
}