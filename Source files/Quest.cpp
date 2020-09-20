#include "Quest.h"
#include "Game.h"

#define QUEST_INFO 13

extern Game* game;

void Quest_flag::Load(std::ifstream& f){
	f >> name >> counter;
}

void Quest_objective::Load(std::ifstream& f){
	std::string aux;
	short int num_flags;
	f.ignore();
	std::getline(f, description, '\n');
	f >> num_flags;
	for (int i = 0; i < num_flags; i++) {
		Quest_flag flag;
		flag.Load(f);
		needed_flags.push_back(flag);
	}
	f >> aux;
	if (aux != ";;") {
		std::cout << "Quest file missing a terminator string";
		exit(-32);
	}
}

void Quest_objective::Refresh(){
	status = OBJECTIVE_FINISHED;
	std::map<std::string, Quest_flag>* flags = game->Get_unclaimed_flags();
	for (std::list<Quest_flag>::iterator i = needed_flags.begin(); i != needed_flags.end();i++) {
		std::map<std::string, Quest_flag>::iterator flag = flags->find((*i).Name());
		if (flag != flags->end()) {
			int num=(*i).Change_counter(-1*(*flag).second.Counter());
			if(num==0)
				flags->erase(flag);
			else
				(*flag).second.Set_counter(num);
		}
		if (status == OBJECTIVE_FINISHED && (*i).Counter() != 0) status = OBJECTIVE_IN_PROGRESS;
	}
}

std::string Quest::Info()
{
	std::string info = "";
	int j = 1;
	info += name + "\n\n" + description + "\n\nObiective:\n";
	for (std::list<Quest_objective>::iterator i = objectives.begin(); i != objectives.end(); i++,j++) {
		info += std::to_string(j) + ".";
		(*i).Finished() ? info += " (F) ":info+="";	//if quest objective is finished append an (F)
		info += (*i).Description()+"\n";
	}
	info += "\n\nStatus: ";
	Finished() ? info += "FINISHED" : info += "IN PROGRESS";
	return info;
}

void Regular_quest::Load(std::ifstream& f){
	short int num_obj;
	status = NOT_TAKEN;
	std::string aux;
	f >> id;
	f.ignore();
	std::getline(f, name, '\n');
	std::getline(f, description, '\t');
	f >> num_obj;
	f >> aux;
	if (aux != "OBJECTIVES:") {
		std::cout << "Quest file missing an \"OBJECTIVES:\"";
		exit(-30);
	}
	for (int i = 0; i < num_obj; i++) {
		Quest_objective obj;
		obj.Load(f);
		objectives.push_back(obj);
	}
	f >> aux;
	if (aux != ";;") {
		std::cout << "Quest file missing a terminator string";
		exit(-31);
	}
}

void Regular_quest::Refresh(){
	status = FINISHED;
	for (std::list<Quest_objective>::iterator i = objectives.begin(); i != objectives.end(); i++) {
		(*i).Refresh();
		if (status == FINISHED && !(*i).Finished()) status = ACTIVE;
	}
}

void Chain_quest::Load(std::ifstream& f) {
	short int num_obj;
	status = NOT_TAKEN;
	std::string aux;
	f >> id;
	f >> next_quest;
	f.ignore();
	std::getline(f, name, '\n');
	std::getline(f, description, '\t');
	f >> num_obj;
	f >> aux;
	if (aux != "OBJECTIVES:") {
		std::cout << "Quest file missing an \"OBJECTIVES:\"";
		exit(-30);
	}
	for (int i = 0; i < num_obj; i++) {
		Quest_objective obj;
		obj.Load(f);
		objectives.push_back(obj);
	}
	f >> aux;
	if (aux != ";;") {
		std::cout << "Quest file missing a terminator string";
		exit(-31);
	}
}

void Chain_quest::Refresh() {
	int oldstatus = status;
	status = FINISHED;
	for (std::list<Quest_objective>::iterator i = objectives.begin(); i != objectives.end(); i++) {
		(*i).Refresh();
		if (status == FINISHED && !(*i).Finished()) status = ACTIVE;
	}
	if (status == FINISHED&&oldstatus==ACTIVE) {
		game->Get_mainchar()->Add_quest(next_quest);
	}
}
