#pragma once
#include <iostream>
#include <string>
#include <fstream>

class DialogueState;

struct DialogueOption
{
	std::string text;
	DialogueState* next;
	DialogueOption() {
		text = "";
		next = nullptr;
	}
};

class DialogueState{
	std::string text;
	int num_options;
	DialogueOption** options;	//will be dynamically allocated
public:
	DialogueState() {
		text = "";
		num_options = 0;
		options = nullptr;
	}
	int Num_opts() { return num_options; }
	std::string NPC_text() { return text; }
	DialogueOption** Dialogue_options() { return options; }
	void Load(std::ifstream&,DialogueState**);
	void Enter_dialogue(int npc_id);
};

