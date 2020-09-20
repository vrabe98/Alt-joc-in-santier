#include "DialogueBox.h"
#include "Game.h"

extern Game* game;

int npc=-1;

int Dialogue(Button* buton) {
	game->Get_dialoguebox()->Change_state();
	return 0;
}

int Combat(Button* buton) {
	game->Start_combat(npc);
	return 0;
}

int Trade(Button* buton) {
	game->Get_mainchar()->Set_interact_inventory(game->Get_dialoguebox()->Get_npc()->Inventar());
	game->Change_state(TRADE_SCREEN);
	return TRADE_SCREEN;
}

void DialogueBox::Create(){
	sf::Vector2f pos, size;
	state = PRELIMINARY;
	offset = 0;
	active = false;
	box = RoundedRectangle(25, sf::Vector2f(75, 493), sf::Vector2f(700, 200));
	picbox = PictureBox(box.Radius(), box.Position(), box.Size());
	char_name.setStyle(sf::Text::Regular);
	char_name.setFillColor(sf::Color::White);
	char_name.setPosition(box.Position().x + box.Radius(), picbox.Position().y - SPACING - CHARNAMEH);

	pos = sf::Vector2f(box.Position().x + box.Size().x / 2 - 100, box.Position().y + box.Size().y / 2 - 25);
	size = sf::Vector2f(100, 50);

	dialogue = Button(pos, size, "Talk", sf::Color(100, 100, 100,200));

	pos = sf::Vector2f(box.Position().x+box.Size().x/2+50, box.Position().y + box.Size().y / 2 - 25);

	combat = Button(pos, size, "Fight", sf::Color(255,0,0,200));

	pos = sf::Vector2f(box.Position().x + box.Size().x / 2 + 200, box.Position().y + box.Size().y / 2 - 25);

	trade = Button(pos, size, "Trade", sf::Color(255, 215, 0, 200));

	dialogue.function_pointer = Dialogue;
	combat.function_pointer = Combat;
	trade.function_pointer = Trade;
}

Character* DialogueBox::Get_npc(){
	return game->Get_char(npc_msg.Npc_id());
}

void DialogueBox::Enter_dialogue(DialogueState* entry, int npc_id,int state){
	DialogueOption** opts = entry->Dialogue_options();
	std::string npc_message = entry->NPC_text();
	entry->Set_flags();
	npc = npc_id;
	int num_opts = entry->Num_opts();
	char_msgs.clear();
	npc_msg = NPCMessage(npc_message, npc_id);
	for (int i = 0; i < num_opts; i++) {
		MaincharMessage* mainchar_msg = new MaincharMessage(opts[i]->text, i, opts[i]->next);
		char_msgs.push_back(mainchar_msg);
	}
	this->state = state;
}

MaincharMessage::MaincharMessage(std::string message,int order,DialogueState* next){
	this->order = order;
	this->next = next;
	text.setFont(*game->Get_font());
	text.setString(message);
	text.setStyle(sf::Text::Regular);
	text.setFillColor(sf::Color::White);
}

void MaincharMessage::Draw(sf::RenderTarget& target,int radius, sf::Vector2f position, sf::Vector2f size_,int offset){
	double coeff = 141.0 / 72.0, font_size, max_word_length = 0, num_words = 0;
	std::string str = text.getString();
	sf::Vector2f size = sf::Vector2f(size_.x - 2 * radius-CHAR_PICDIM-SPACING, size_.y);
	sf::Vector2f pos = sf::Vector2f(position.x + radius+CHAR_PICDIM+SPACING, position.y);

	sf::Vertex* quad = &bounding_box[0];
	quad[0].position = sf::Vector2f(pos.x, pos.y + YOFFSET + (order - offset) * (MAINCHAROPTH+SPACING));
	quad[1].position = sf::Vector2f(pos.x+size.x, pos.y + YOFFSET+ (order - offset) * (MAINCHAROPTH+SPACING));
	quad[2].position = sf::Vector2f(pos.x+size.x, pos.y + YOFFSET+(order - offset) * (MAINCHAROPTH+SPACING)+MAINCHAROPTH);
	quad[3].position = sf::Vector2f(pos.x, pos.y + YOFFSET+(order - offset) * (MAINCHAROPTH+SPACING)+MAINCHAROPTH);

	char* cstring = new char[str.length() + 1];
	std::strcpy(cstring, str.c_str());
	char* p = std::strtok(cstring, "\n");
	while (p != 0)
	{
		if (strlen(p) > max_word_length) max_word_length = strlen(p);
		num_words++;
		p = std::strtok(NULL, "\n");
	}
	delete[] cstring;
	font_size = size.x * coeff / (max_word_length + 4.0f);
	text.setCharacterSize(font_size);
	if (text.getLocalBounds().height >= MAINCHAROPTH) {
		font_size = (MAINCHAROPTH) / num_words - text.getLineSpacing() * (num_words - 4);
		text.setCharacterSize(font_size);
	}
	if (text.getCharacterSize() >= 25.0) text.setCharacterSize(25.0);
	text.setPosition(pos.x, pos.y+ YOFFSET + (order - offset) * (MAINCHAROPTH+SPACING) + (MAINCHAROPTH-text.getLocalBounds().height) / 2 - text.getLocalBounds().top);

	target.draw(*this);
}

NPCMessage::NPCMessage(std::string message,int npc_id){
	this->npc_id = npc_id;
	text.setFont(*game->Get_font());
	text.setString(message);
	text.setStyle(sf::Text::Regular);
	text.setFillColor(sf::Color::White);
}

void NPCMessage::Draw(sf::RenderTarget& target,int radius, sf::Vector2f position, sf::Vector2f size_,int offset){
	double coeff = 141.0 / 72.0, font_size, max_word_length = 0, num_words = 0;
	std::string str = text.getString();
	sf::Vector2f size = sf::Vector2f(size_.x - 2 * radius-CHAR_PICDIM-SPACING, size_.y);
	sf::Vector2f pos = sf::Vector2f(position.x + radius+CHAR_PICDIM+SPACING, position.y);

	sf::Vertex* quad = &bounding_box[0];
	quad[0].position = sf::Vector2f(pos.x, pos.y);
	quad[1].position = sf::Vector2f(pos.x+ size.x, pos.y);
	quad[2].position = sf::Vector2f(pos.x +size.x, pos.y+size.y);
	quad[3].position = sf::Vector2f(pos.x, pos.y+size.y);

	char* cstring = new char[str.length() + 1];
	std::strcpy(cstring, str.c_str());
	char* p = std::strtok(cstring, "\n");
	while (p != 0)
	{
		if (strlen(p) > max_word_length) max_word_length = strlen(p);
		num_words++;
		p = std::strtok(NULL, "\n");
	}
	delete[] cstring;
	font_size = size.x * coeff / (max_word_length + 4.0f);
	text.setCharacterSize(font_size);
	if (text.getLocalBounds().height >= size.y) {
		font_size = (size.y) / num_words - text.getLineSpacing() * (num_words - 6);
		text.setCharacterSize(font_size);
	}
	if (text.getCharacterSize() >= 25.0) text.setCharacterSize(25.0);
	text.setPosition(quad[0].position.x, quad[0].position.y+radius*1.5+SPACING);

	target.draw(*this);
}

bool DialogueMessage::MouseWithinBounds(sf::Vector2i pos){
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

void DialogueBox::Draw(sf::RenderTarget& target,int code) {
	float coeff = 141.0 / 72.0;
	float font_size;
	target.draw(box);
	char_name.setFont(*game->Get_font());
	switch (this->state) {
	case PRELIMINARY:
		char_name.setString(game->Get_char(npc_msg.Npc_id())->Name() + ":");
		font_size = (CHARNAMEW * coeff) / (std::string(char_name.getString()).length() + 4.0f);
		char_name.setCharacterSize(font_size);
		picbox.Set_texture(game->Get_char(npc_msg.Npc_id())->Diag_texture());
		target.draw(dialogue);
		target.draw(combat);
		if(code==VENDOR_DIAG)	target.draw(trade);
		break;
	case NPC_MSG:
		char_name.setString(game->Get_char(npc_msg.Npc_id())->Name() + ":");
		font_size = (CHARNAMEW * coeff) / (std::string(char_name.getString()).length() + 4.0f);
		char_name.setCharacterSize(font_size);
		picbox.Set_texture(game->Get_char(npc_msg.Npc_id())->Diag_texture());
		npc_msg.Draw(target, box.Radius(), box.Position(), box.Size(), offset);
		break;
	case MAINCHAR_MSG:
		int msgs = 0;
		std::list<MaincharMessage*>::iterator i = char_msgs.begin();
		std::advance(i, offset);
		while (i != char_msgs.end() && msgs != MAX_SHOWN_MESSAGES) {
			(*i)->Draw(target, box.Radius(), box.Position(), box.Size(), offset);
			msgs++;
			i++;
		}
		char_name.setString(game->Get_mainchar()->Name() + ":");
		font_size = (CHARNAMEW * coeff) / (std::string(char_name.getString()).length() + 4.0f);
		char_name.setCharacterSize(font_size);
		picbox.Set_texture(game->Get_mainchar()->Diag_texture());
		break;
	}
	target.draw(picbox);
	target.draw(char_name);
}

void DialogueBox::LMB_Press(sf::Vector2i pos){
	if (MouseWithinBounds(pos)&&Active()) {
		switch (this->state) {
		case PRELIMINARY:
			if (dialogue.MouseWithinBounds(pos)) {
				dialogue.Change_state();
			}
			else if (combat.MouseWithinBounds(pos)) {
				combat.Change_state();
			}
			else if (trade.MouseWithinBounds(pos)) {
				trade.Change_state();
			}
			break;
		case NPC_MSG:
			Change_state();
			break;
		case MAINCHAR_MSG:
			DialogueState* next = nullptr;
			for (std::list<MaincharMessage*>::iterator i = char_msgs.begin(); i != char_msgs.end(); i++) {
				if ((*i)->MouseWithinBounds(pos)) {
					next = (*i)->Action();
					if (next == nullptr) Activate_deactivate();
					else Enter_dialogue(next, npc_msg.Npc_id(),NPC_MSG);
					return;
				}
			}
			break;
		}
	}
}

void DialogueBox::LMB_Released(sf::Vector2i pos){
	if (this->state == PRELIMINARY) {
		if (dialogue.MouseWithinBounds(pos)) {
			dialogue.Change_state();
			dialogue.function_pointer(&dialogue);
		}
		else if (combat.MouseWithinBounds(pos)) {
			combat.Change_state();
			combat.function_pointer(&combat);
		}
		else if (trade.MouseWithinBounds(pos)) {
			trade.Change_state();
			trade.function_pointer(&trade);
		}
	}
}