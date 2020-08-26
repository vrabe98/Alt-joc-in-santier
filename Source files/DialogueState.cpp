#include "DialogueState.h"
#include "Game.h"

extern Game* game;

void DialogueState::Load(std::ifstream& f,DialogueState** arr_addr){
	std::string aux;
	int curr_id;
	f >> curr_id >> num_options;
	f.ignore();
	std::getline(f, text, '\n');
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

void DialogueState::Enter_dialogue(int npc_id){

}
