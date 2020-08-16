#include "Button.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include "Character.h"
#include "Game.h"

extern Game* game;

#define LIGHTEN_FACTOR 40

#define FWD 1
#define BACK -1

#define EXIT 0
#define CHAR_SCR 4
#define INC_STR -2
#define INC_DEX -3
#define INC_CON -4
#define INC_CHA -5
#define	PLAYPAUSE -6
#define	PREV -7
#define	NEXT -8
#define LOOPSEQ -9
#define PLTONONPL -10
#define NONPLTOPL -11
#define PLUS10SEC -12
#define MINUS10SEC -13

int Next_state(Button* buton) {
	return buton->Nextstate();
}

int Plus10sec(Button* buton){
	if (*game->Get_string_from_db(CURRENT_SONG) != "") {
		sf::Music* music = game->Get_playlist()->Get_music();
		sf::Time current_time = music->getPlayingOffset(), duration = music->getDuration();
		if (current_time + sf::seconds(10) > duration) {
			game->Get_playlist()->Change_song(FWD);
		}
		else {
			current_time += sf::seconds(10);
			game->Get_playlist()->Get_music()->setPlayingOffset(current_time);
		}
	}
	return PLUS10SEC;
}

int Minus10sec(Button* buton) {
	if (*game->Get_string_from_db(CURRENT_SONG) != "") {
		sf::Music* music = game->Get_playlist()->Get_music();
		sf::Time current_time = music->getPlayingOffset(), duration = music->getDuration();
		if (current_time - sf::seconds(10) < sf::seconds(0)) {
			game->Get_playlist()->Change_song(BACK);
		}
		else {
			current_time -= sf::seconds(10);
			game->Get_playlist()->Get_music()->setPlayingOffset(current_time);
		}
	}
	return MINUS10SEC;
}

int Player_to_nonplayer(Button* buton) {
	InventorySlot* item_out = game->Get_mainchar()->Inventory()->Get_highlighted();
	if (item_out != nullptr) {
		game->Get_mainchar()->Get_inventory_interact()->Add_item(item_out->Get_item());
		game->Get_mainchar()->Inventory()->Remove_item(item_out->Order());
	}
	return PLTONONPL;
}

int Nonplayer_to_player(Button* buton) {
	InventorySlot* item_out = game->Get_mainchar()->Get_inventory_interact()->Get_highlighted();
	if (item_out != nullptr) {
		game->Get_mainchar()->Inventory()->Add_item(item_out->Get_item());
		game->Get_mainchar()->Get_inventory_interact()->Remove_item(item_out->Order());
	}
	return NONPLTOPL;
}

int Play_pause(Button* buton) {
	int nextstate;
	nextstate=game->Get_playlist()->Change_playing_state();
	if (nextstate) buton->UpdateText(">");
	else buton->UpdateText("||");
	return PLAYPAUSE;
}

int Loop_sequential(Button* buton) {
	int loop;
	loop = game->Get_playlist()->Loop_change();
	if (loop) buton->UpdateText("Sequential");
	else buton->UpdateText("Loop");
	return LOOPSEQ;
}

int Prev(Button* buton) {
	game->Get_playlist()->Change_song(BACK);
	return PREV;
}

int Next(Button* buton) {
	game->Get_playlist()->Change_song(FWD);
	return NEXT;
}

int Increment_strength(Button* buton) {
	std::string text;
	game->Get_mainchar()->Inc_STR();
	text = "STR: "+std::to_string(game->Get_mainchar()->STR());
	buton->UpdateText(text);
	return INC_STR;
}

int Increment_dexterity(Button* buton) {
	std::string text;
	game->Get_mainchar()->Inc_DEX();
	text = "DEX: "+std::to_string(game->Get_mainchar()->DEX());
	buton->UpdateText(text);
	return INC_DEX;
}

int Increment_constitution(Button* buton) {
	std::string text;
	game->Get_mainchar()->Inc_CON();
	text ="CON: "+ std::to_string(game->Get_mainchar()->CON());
	buton->UpdateText(text);
	return INC_CON;
}

int Increment_charisma(Button* buton) {
	std::string text;
	game->Get_mainchar()->Inc_CHA();
	text ="CHA: "+ std::to_string(game->Get_mainchar()->CHA());
	buton->UpdateText(text);
	return INC_CHA;
}

Button::Button(std::ifstream& file, sf::Font& font) {
	std::string text;
	sf::Color colr=sf::Color::Black;
	int r, g, b, action;
	float width, height, posx, posy, coeff = 141.0 / 72.0;
	float font_size;
	corners = sf::VertexArray(sf::Quads, 4);
	file >> width>> height;
	file >> posx >> posy;
	file >> r >> g >> b;
	file.ignore();
	std::getline(file, text, '\n');
	file >> action;
	colr.r = r;
	colr.g = g;
	colr.b = b;
	corners[0].position = sf::Vector2f(posx,posy);
	corners[1].position = sf::Vector2f(posx + width, posy);
	corners[2].position = sf::Vector2f(posx + width, posy + height);
	corners[3].position = sf::Vector2f(posx, posy + height);
	for (int i = 0; i < 4; i++) {
		corners[i].color = colr;
	}
	if (action >= 0) {	//action >= 0 corresponds to a state transition
		nextstate = action;
		function_pointer = Next_state;
	}
	else switch (action) {
	case INC_STR:
		text = "STR: ";
		text.append(std::to_string(game->Get_mainchar()->STR()));
		function_pointer = Increment_strength;
		break;
	case INC_DEX:
		text = "DEX: ";
		text.append(std::to_string(game->Get_mainchar()->DEX()));
		function_pointer = Increment_dexterity;
		break;
	case INC_CON:
		text = "CON: ";
		text.append(std::to_string(game->Get_mainchar()->CON()));
		function_pointer = Increment_constitution;
		break;
	case INC_CHA:
		text = "CHA: ";
		text.append(std::to_string(game->Get_mainchar()->CHA()));
		function_pointer = Increment_charisma;
		break;
	case PLAYPAUSE:
		text = "||";
		function_pointer = Play_pause;
		break;
	case PREV:
		text = "<<";
		function_pointer = Prev;
		break;
	case NEXT:
		text = ">>";
		function_pointer = Next;
		break;
	case LOOPSEQ:
		text = "Loop";
		function_pointer = Loop_sequential;
		break;
	case PLTONONPL:
		text = ">";
		function_pointer = Player_to_nonplayer;
		break;
	case NONPLTOPL:
		text = "<";
		function_pointer = Nonplayer_to_player;
		break;
	case PLUS10SEC:
		text = ">> 10s";
		function_pointer = Plus10sec;
		break;
	case MINUS10SEC:
		text = "<< 10s";
		function_pointer = Minus10sec;
		break;
	}
	font_size = (width * coeff) / (text.length() + 4.0f);
	button_text.setFont(font);
	button_text.setCharacterSize(font_size);
	button_text.setString(text);
	button_text.setStyle(sf::Text::Regular);
	button_text.setPosition(posx + (width - button_text.getLocalBounds().width) / 2, posy + (height - button_text.getLocalBounds().height) / 2-button_text.getLocalBounds().top);
	button_text.setFillColor(sf::Color::White);
	file >> text;	//terminator string, reusing a string variable
	if (text != ";;") {
		std::cout << text;
		exit(-6);
	}
	pressed = -1;
}

void Button::UpdateText(std::string text) {
	float coeff = 141.0 / 72.0;
	sf::Vector2f position = corners[0].position, size = corners[2].position - corners[0].position;
	float font_size = (size.x * coeff) / (text.length() + 4.0f);
	button_text.setCharacterSize(font_size);
	button_text.setString(text);
	button_text.setPosition(position.x+ (size.x - button_text.getLocalBounds().width) / 2, position.y + (size.y - button_text.getLocalBounds().height) / 2-button_text.getLocalBounds().top);
}

int Button::MouseWithinBounds(sf::Vector2i pos) {
	return (corners[0].position.x <= (float)pos.x) && ((float)pos.x <= corners[1].position.x) && (corners[0].position.y  <= (float)pos.y) && ((float)pos.y <= corners[3].position.y);
}

void Button::Change_state() {
	sf::Color color = corners[0].color;
	pressed = -1 * pressed;
	color.r += pressed*LIGHTEN_FACTOR;
	color.g += pressed*LIGHTEN_FACTOR;
	color.b += pressed*LIGHTEN_FACTOR;
	for (int i = 0; i < 4; i++) {
		corners[i].color = color;
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(corners);
	target.draw(button_text);
}