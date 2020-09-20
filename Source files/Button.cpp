#include "Button.h"
#include "Game.h"

extern Game* game;

#define LIGHTEN_FACTOR 40

#define FWD 1
#define BACK -1

int Next_state(Button* buton) {
	return buton->Nextstate();
}

int Buy(Button* buton) {
	InventorySlot* item_out = game->Get_mainchar()->Get_inventory_interact()->Get_highlighted();
	if (item_out != nullptr) {
		if (game->Get_mainchar()->Try_update_currency(-1 * item_out->Get_item()->Price())) {
			game->Get_mainchar()->Inventar()->Add_item(item_out->Get_item());
			game->Get_mainchar()->Get_inventory_interact()->Remove_item(item_out->Order());
		}
	}
	return Button::Action::buy;
}

int Sell(Button* buton) {
	InventorySlot* item_out = game->Get_mainchar()->Inventar()->Get_highlighted();
	if (item_out != nullptr) {
		if (game->Get_mainchar()->Try_update_currency(item_out->Get_item()->Price())) {
			game->Get_mainchar()->Get_inventory_interact()->Add_item(item_out->Get_item());
			game->Get_mainchar()->Inventar()->Remove_item(item_out->Order());
		}
	}
	return Button::Action::sell;
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
	return Button::Action::plus10sec;
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
	return Button::Action::minus10sec;
}

int Player_to_nonplayer(Button* buton) {
	InventorySlot* item_out = game->Get_mainchar()->Inventar()->Get_highlighted();
	if (item_out != nullptr) {
		game->Get_mainchar()->Get_inventory_interact()->Add_item(item_out->Get_item());
		game->Get_mainchar()->Inventar()->Remove_item(item_out->Order());
	}
	return Button::Action::pltononpl;
}

int Nonplayer_to_player(Button* buton) {
	InventorySlot* item_out = game->Get_mainchar()->Get_inventory_interact()->Get_highlighted();
	if (item_out != nullptr) {
		game->Get_mainchar()->Inventar()->Add_item(item_out->Get_item());
		game->Get_mainchar()->Get_inventory_interact()->Remove_item(item_out->Order());
	}
	return Button::Action::nonpltopl;
}

int Play_pause(Button* buton) {
	int nextstate;
	nextstate=game->Get_playlist()->Change_playing_state();
	if (nextstate) buton->UpdateText(">");
	else buton->UpdateText("||");
	return Button::Action::playpause;
}

int Loop_sequential(Button* buton) {
	int loop;
	loop = game->Get_playlist()->Loop_change();
	if (loop) buton->UpdateText("Sequential");
	else buton->UpdateText("Loop");
	return Button::Action::loopseq;
}

int Prev(Button* buton) {
	game->Get_playlist()->Change_song(BACK);
	return Button::Action::prev;
}

int Next(Button* buton) {
	game->Get_playlist()->Change_song(FWD);
	return Button::Action::next;
}

int Increment_strength(Button* buton) {
	std::string text;
	game->Get_mainchar()->Inc_STR();
	text = "STR: "+std::to_string(game->Get_mainchar()->STR());
	buton->UpdateText(text);
	return Button::Action::inc_str;
}

int Increment_dexterity(Button* buton) {
	std::string text;
	game->Get_mainchar()->Inc_DEX();
	text = "DEX: "+std::to_string(game->Get_mainchar()->DEX());
	buton->UpdateText(text);
	return Button::Action::inc_dex;
}

int Increment_constitution(Button* buton) {
	std::string text;
	game->Get_mainchar()->Inc_CON();
	text ="CON: "+ std::to_string(game->Get_mainchar()->CON());
	buton->UpdateText(text);
	return Button::Action::inc_con;
}

int Increment_charisma(Button* buton) {
	std::string text;
	game->Get_mainchar()->Inc_CHA();
	text ="CHA: "+ std::to_string(game->Get_mainchar()->CHA());
	buton->UpdateText(text);
	return Button::Action::inc_cha;
}

Button::Button(std::ifstream& file) {
	std::string text;
	sf::Color colr=sf::Color::Black;
	int r, g, b, act;
	float width, height, posx, posy, coeff = 141.0 / 72.0;
	float font_size;
	corners = sf::VertexArray(sf::Quads, 4);
	file >> width>> height;
	file >> posx >> posy;
	file >> r >> g >> b;
	file.ignore();
	std::getline(file, text, '\n');
	file >> act;
	colr.r = r;
	colr.g = g;
	colr.b = b;
	colr.a = 200;
	corners[0].position = sf::Vector2f(posx,posy);
	corners[1].position = sf::Vector2f(posx + width, posy);
	corners[2].position = sf::Vector2f(posx + width, posy + height);
	corners[3].position = sf::Vector2f(posx, posy + height);

	Create_colors(colr);

	if (act >= 0) {	//action >= 0 corresponds to a state transition
		nextstate = act;
		function_pointer = Next_state;
	}
	else switch (act) {
	case Button::Action::inc_str:
		text = "STR: ";
		text.append(std::to_string(game->Get_mainchar()->STR()));
		function_pointer = Increment_strength;
		break;
	case Button::Action::inc_dex:
		text = "DEX: ";
		text.append(std::to_string(game->Get_mainchar()->DEX()));
		function_pointer = Increment_dexterity;
		break;
	case Button::Action::inc_con:
		text = "CON: ";
		text.append(std::to_string(game->Get_mainchar()->CON()));
		function_pointer = Increment_constitution;
		break;
	case Button::Action::inc_cha:
		text = "CHA: ";
		text.append(std::to_string(game->Get_mainchar()->CHA()));
		function_pointer = Increment_charisma;
		break;
	case Button::Action::playpause:
		text = "||";
		function_pointer = Play_pause;
		break;
	case Button::Action::prev:
		text = "<<";
		function_pointer = Prev;
		break;
	case Button::Action::next:
		text = ">>";
		function_pointer = Next;
		break;
	case Button::Action::loopseq:
		text = "Loop";
		function_pointer = Loop_sequential;
		break;
	case Button::Action::pltononpl:
		text = ">";
		function_pointer = Player_to_nonplayer;
		break;
	case Button::Action::nonpltopl:
		text = "<";
		function_pointer = Nonplayer_to_player;
		break;
	case Button::Action::plus10sec:
		text = ">> 10s";
		function_pointer = Plus10sec;
		break;
	case Button::Action::minus10sec:
		text = "<< 10s";
		function_pointer = Minus10sec;
		break;
	case Button::Action::buy:
		text = "Buy";
		function_pointer = Buy;
		break;
	case Button::Action::sell:
		text = "Sell";
		function_pointer = Sell;
		break;
	default:
		std::cout << "INVALID BUTTON CODE!";
		exit(-34);
	}
	font_size = (width * coeff) / (text.length() + 4.0f);
	button_text.setFont(*game->Get_font());
	button_text.setCharacterSize(font_size);
	button_text.setString(text);

	if (button_text.getLocalBounds().height >= height) {
		font_size = height;
		button_text.setCharacterSize(font_size);
	}
	if (button_text.getCharacterSize() >= 30.0) button_text.setCharacterSize(30.0);

	button_text.setStyle(sf::Text::Regular);
	button_text.setPosition(posx + (width - button_text.getLocalBounds().width) / 2, posy + (height - button_text.getLocalBounds().height) / 2-button_text.getLocalBounds().top);
	button_text.setFillColor(sf::Color::White);
	file >> text;	//terminator string, reusing a string variable
	if (text != ";;") {
		std::cout << text;
		exit(-6);
	}
	pressed = false;
}

Button::Button(sf::Vector2f pos, sf::Vector2f size, std::string text, sf::Color color){
	float coeff = 141.0 / 72.0, font_size = 0;
	nextstate = 0;
	corners = sf::VertexArray(sf::Quads, 4);

	corners[0].position = sf::Vector2f(pos.x, pos.y);
	corners[1].position = sf::Vector2f(pos.x + size.x, pos.y);
	corners[2].position = sf::Vector2f(pos.x + size.x, pos.y + size.y);
	corners[3].position = sf::Vector2f(pos.x, pos.y + size.y);

	Create_colors(color);

	font_size = (size.x * coeff) / (text.length() + 4.0f);
	button_text.setFont(*game->Get_font());
	button_text.setCharacterSize(font_size);
	button_text.setString(text);

	if (button_text.getLocalBounds().height >= size.y) {
		font_size = size.y;
		button_text.setCharacterSize(font_size);
	}
	if (button_text.getCharacterSize() >= 30.0) button_text.setCharacterSize(30.0);

	button_text.setStyle(sf::Text::Regular);
	button_text.setPosition(pos.x + (size.x - button_text.getLocalBounds().width) / 2, pos.y + (size.y - button_text.getLocalBounds().height) / 2 - button_text.getLocalBounds().top);
	button_text.setFillColor(sf::Color::White);
	pressed = false;
}

void Button::Create_colors(sf::Color color){
	unsigned int r = (unsigned int)color.r, g = (unsigned int)color.g, b = (unsigned int)color.b;
	button_colors[0] = color;

	r = std::min(255u, r + LIGHTEN_FACTOR);

	g = std::min(255u, g + LIGHTEN_FACTOR);

	b = std::min(255u, b + LIGHTEN_FACTOR);

	button_colors[1] = sf::Color(r, g, b);

	for (int i = 0; i < 4; i++) {
		corners[i].color = button_colors[0];
	}
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
	short int index;
	pressed = !pressed;
	pressed == false ? index = 0 : index = 1;
	for (int i = 0; i < 4; i++) {
		corners[i].color = button_colors[pressed];
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(corners);
	target.draw(button_text);
}