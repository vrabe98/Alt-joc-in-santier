#include "GameState.h"
#include <fstream>
#include <iostream>

void GameState::Load(std::ifstream& f, int num_panels, int num_buttons,int num_textboxes,sf::Font& font) {
	std::string aux;
	f >> aux;
	if (aux != "PANELS:") {
		exit(-2);
	}
	for (int i = 0; i < num_panels; i++) {
		Panel panel(f);
		panels.push_back(panel);
	}
	f >> aux;
	if (aux != "BUTTONS:") {
		std::cout << aux << std::endl;
		exit(-3);
	}
	for (int i = 0; i < num_buttons; i++) {
		Button* buton=new Button(f);
		butoane.push_back(buton);
	}
	f >> aux;
	if (aux != "TEXTBOXES:") {
		std::cout << aux << std::endl;
		exit(-9);
	}
	for (int i = 0; i < num_textboxes; i++) {
		TextBox* textbox;
		std::string type;
		f >> type;
		if (type == "input") {
			InputBox* box = new InputBox(f, font);
			textbox = box;
		}
		else if (type == "output") {
			OutputBox* box = new OutputBox(f, font);
			textbox = box;
		}
		else {
			exit(-8);
		}
		textboxes.push_back(textbox);
	}
	f >> aux;
	if (aux != ";;") {
		exit(-4);
	}
}

void GameState::LMB_Pressed(sf::Vector2i pos) {
	int found = 0;
	for (std::list<Button*>::iterator i = butoane.begin(); i != butoane.end(); i++) {
		if ((*i)->MouseWithinBounds(pos)) {
			(*i)->Change_state();
		}
	}
	for (std::list<TextBox*>::iterator i = textboxes.begin(); i != textboxes.end()&&(!found); i++) {
		if ((*i)->MouseWithinBounds(pos)) {
			(*i)->Change_state();
			found = 1;
		}
	}
	for (std::list<TextBox*>::iterator i = textboxes.begin(); i != textboxes.end(); i++) {
		if (!(*i)->MouseWithinBounds(pos)&&(*i)->Is_highlighted()) {
			(*i)->Change_state();
		}
	}
}

int GameState::LMB_Released(sf::Vector2i pos) {
	for (std::list<Button*>::iterator i = butoane.begin(); i != butoane.end(); i++) {
		if ((*i)->MouseWithinBounds(pos)) {
			(*i)->Change_state();
			return (*i)->function_pointer(*i);
		}
	}
	return -1;
}

void GameState::Draw(sf::RenderTarget& target) {
	for (std::list<Panel>::iterator i = panels.begin(); i != panels.end(); i++) {
		target.draw(*i);
	}
	for (std::list<Button*>::iterator i = butoane.begin(); i != butoane.end(); i++) {
		target.draw(**i);
	}
	for (std::list<TextBox*>::iterator i = textboxes.begin(); i != textboxes.end(); i++) {
		(*i)->Update();
		target.draw(**i);
	}
}

void GameState::KeyPress(char c) {
	for (std::list<TextBox*>::iterator i = textboxes.begin(); i != textboxes.end(); i++) {
		if ((*i)->Is_highlighted()) {
			if (c == 13) (*i)->Action();	//ENTER
			else (*i)->Append_char(c);
		}
		return;
	}
}