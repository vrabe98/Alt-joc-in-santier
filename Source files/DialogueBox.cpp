#include "DialogueBox.h"
#include "Game.h"

#define MAINCHAROPTH 50
#define SPACING 10
#define YOFFSET 15

extern Game* game;

void DialogueBox::Enter_dialogue(DialogueState* entry, int npc_id){
	DialogueOption** opts = entry->Dialogue_options();
	std::string npc_message = entry->NPC_text();
	int num_opts = entry->Num_opts();
	char_msgs.clear();
	npc_msg = NPCMessage(npc_message, npc_id);
	for (int i = 0; i < num_opts; i++) {
		MaincharMessage* mainchar_msg = new MaincharMessage(opts[i]->text,i,opts[i]->next);
		char_msgs.push_back(mainchar_msg);
	}
	state = NPC_MSG;
}

MaincharMessage::MaincharMessage(std::string message,int order,DialogueState* next){
	this->order = order;
	this->next = next;
	text.setFont(*game->Get_font());
	text.setString(game->Get_mainchar()->Name()+":\n"+message);
	text.setStyle(sf::Text::Regular);
	text.setFillColor(sf::Color::White);
}

void MaincharMessage::Draw(sf::RenderTarget& target,int radius, sf::Vector2f position, sf::Vector2f size_,int offset){
	double coeff = 141.0 / 72.0, font_size, max_word_length = 0, num_words = 0;
	std::string str = text.getString();
	sf::Vector2f size = sf::Vector2f(size_.x - 2 * radius, size_.y);
	sf::Vector2f pos = sf::Vector2f(position.x + radius, position.y);

	sf::Vertex* quad = &bounding_box[0];
	quad[0].position = sf::Vector2f(pos.x, pos.y + YOFFSET + (order - offset) * (MAINCHAROPTH+SPACING));
	quad[1].position = sf::Vector2f(pos.x+size.x, pos.y + YOFFSET+ (order - offset) * (MAINCHAROPTH+SPACING));
	quad[2].position = sf::Vector2f(pos.x+size.x, pos.y + YOFFSET+(order - offset + 1) * (MAINCHAROPTH+SPACING));
	quad[3].position = sf::Vector2f(pos.x, pos.y + YOFFSET+(order - offset + 1) * (MAINCHAROPTH+SPACING));

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
	text.setCharacterSize(font_size);
	text.setPosition(pos.x, pos.y+ YOFFSET + (order - offset) * (MAINCHAROPTH+SPACING) + (MAINCHAROPTH-text.getLocalBounds().height) / 2 - text.getLocalBounds().top);

	target.draw(*this);
}

NPCMessage::NPCMessage(std::string message,int npc_id){
	this->npc_id = npc_id;
	text.setFont(*game->Get_font());
	text.setString(game->Get_char(npc_id)->Name()+":\n"+message);
	text.setStyle(sf::Text::Regular);
	text.setFillColor(sf::Color::White);
}

void NPCMessage::Draw(sf::RenderTarget& target,int radius, sf::Vector2f position, sf::Vector2f size_,int offset){
	double coeff = 141.0 / 72.0, font_size, max_word_length = 0, num_words = 0;
	std::string str = text.getString();
	sf::Vector2f size = sf::Vector2f(size_.x - 2 * radius, size_.y);
	sf::Vector2f pos = sf::Vector2f(position.x + radius, position.y);

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
	text.setCharacterSize(font_size);
	text.setPosition(quad[0].position.x, quad[0].position.y + (size.y - text.getLocalBounds().height) / 2 - text.getLocalBounds().top);

	target.draw(*this);
}

int DialogueMessage::MouseWithinBounds(sf::Vector2i pos){
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}
