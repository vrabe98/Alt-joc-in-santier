#include "DialogueState.h"
#include "Game.h"

extern Game* game;

void DialogueState::Set_flags(){
	for (std::list<Quest_flag>::iterator i = set_flags.begin(); i != set_flags.end();) {
		game->Set_quest_flag(*i);
		i = set_flags.erase(i);
	}
}

void DialogueState::Load(std::ifstream& f,DialogueState** arr_addr){
	std::string aux;
	int curr_id, num_flags;
	f >> curr_id >> num_options;
	f.ignore();
	std::getline(f, text, '\n');
	f >> num_flags;
	for (int i = 0; i < num_flags; i++) {
		Quest_flag flag;
		flag.Load(f);
		set_flags.push_back(flag);
	}
	options = new DialogueOption*[num_options];
	for(int i=0;i<num_options;i++){
		int nextstate_id;
		options[i] = new DialogueOption;
		f.ignore();
		std::getline(f, options[i]->text, '\t');
		f >> nextstate_id;
		if (nextstate_id != -1) {
			options[i]->next = arr_addr[nextstate_id];
		}
	}
	f >> aux;
	if (aux != ";;") {
		std::cout << "\nNPC dialogue file error! Terminator string missing or wrong!";
		exit(-22);
	}
}