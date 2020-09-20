#include "QuestContainer.h"
#include "Game.h"

extern Game* game;

#define QUESTC_W 450
#define QUESTC_H 650

#define QUESTC_X 31
#define QUESTC_Y 93

#define QUESTENTRY_W 450
#define QUESTENTRY_H 50

#define TEXT_Y 20

#define XPAD 10

#define STATUSIMG_DIM 50

#define OUTOFBOX 0
#define SELECTDESELECT 1

#define MAX_QUESTENTRY_VIEW 13

#define QUEST_INFO 13

void Quest_entry::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(bounding_box);
	target.draw(name_text);
	target.draw(status_img,state);
}

bool Quest_entry::MouseWithinBounds(sf::Vector2i pos){
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

void Quest_entry::Draw(sf::RenderTarget& target, int offset){
	sf::RenderStates state;
	sf::Vector2f pos;
	sf::Vertex* quad = &bounding_box[0];

	quad[0].position = sf::Vector2f(QUESTC_X,QUESTC_Y+(order-offset)*QUESTENTRY_H);
	quad[1].position = sf::Vector2f(QUESTC_X+QUESTENTRY_W,QUESTC_Y+(order-offset)*QUESTENTRY_H);
	quad[2].position = sf::Vector2f(QUESTC_X + QUESTENTRY_W, QUESTC_Y + (order - offset + 1) * QUESTENTRY_H);
	quad[3].position = sf::Vector2f(QUESTC_X, QUESTC_Y + (order - offset + 1) * QUESTENTRY_H);
	
	quad = &status_img[0];

	quad[0].position = sf::Vector2f(QUESTC_X, QUESTC_Y + (order - offset) * STATUSIMG_DIM);
	quad[1].position = sf::Vector2f(QUESTC_X + STATUSIMG_DIM, QUESTC_Y + (order - offset) * STATUSIMG_DIM);
	quad[2].position = sf::Vector2f(QUESTC_X + STATUSIMG_DIM, QUESTC_Y + (order - offset + 1) * STATUSIMG_DIM);
	quad[3].position = sf::Vector2f(QUESTC_X, QUESTC_Y + (order - offset + 1) * STATUSIMG_DIM);

	name_text.setString(std::to_string(order)+". "+quest->Name());

	pos.x = QUESTC_X+STATUSIMG_DIM+XPAD;
	pos.y = QUESTC_Y + (order - offset + 0.5) * QUESTENTRY_H - name_text.getLocalBounds().height / 2 - name_text.getLocalBounds().top;

	name_text.setPosition(pos);

	if (quest->Finished()) state.texture = &game->Get_questcomplete_tex();
	else state.texture = &game->Get_questinprogress_tex();

	target.draw(*this, state);
}

void Quest_entry::Change_state(short int action){
	short int modif;
	sf::Color color = bounding_box[0].color;
	highlighted = !highlighted;
	highlighted == false ? modif = -1 : modif = 1;
	color.r += modif * LIGHTEN_FACTOR;
	color.g += modif * LIGHTEN_FACTOR;
	color.b += modif * LIGHTEN_FACTOR;
	for (int i = 0; i < 4; i++) {
		bounding_box[i].color = color;
	}
	if (highlighted) game->Update_string(QUEST_INFO,&quest->Info());
}

Quest_entry::Quest_entry(Quest* quest, int order){
	this->order = order;
	this->quest = quest;

	name_text.setStyle(sf::Text::Regular);
	name_text.setFillColor(sf::Color::White);
	name_text.setFont(*game->Get_font());
	name_text.setCharacterSize(20);

	bounding_box.setPrimitiveType(sf::Quads);
	bounding_box.resize(4);

	sf::Vertex* quad = &bounding_box[0];

	quad[0].color = sf::Color(50, 50, 50, 200);
	quad[1].color = sf::Color(50, 50, 50, 200);
	quad[2].color = sf::Color(50, 50, 50, 200);
	quad[3].color = sf::Color(50, 50, 50, 200);

	status_img.setPrimitiveType(sf::Quads);
	status_img.resize(4);

	quad = &status_img[0];

	quad[0].texCoords = sf::Vector2f(0,0);
	quad[1].texCoords = sf::Vector2f(STATUSIMG_DIM,0);
	quad[2].texCoords = sf::Vector2f(STATUSIMG_DIM,STATUSIMG_DIM);
	quad[3].texCoords = sf::Vector2f(0,STATUSIMG_DIM);

	highlighted = false;
}

void QuestContainer::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(text);
	for (std::list<Quest_entry*>::const_iterator i = entries.begin(); i != entries.end() && ((*i)->Order() - offset) < MAX_ENTRY_VIEW; i++) {
		if ((*i)->Order() >= offset) {
			(*i)->Draw(target, offset);
		}
	}
}

void QuestContainer::Wheel_scroll(short int delta){
	offset += delta;
	if (offset < 0) offset = 0;
	else if (offset >= Size()) offset = Size() - 1;
	for (std::list<Quest_entry*>::iterator i = entries.begin(); i != entries.end(); i++) {
		if ((*i)->Is_highlighted() && (((*i)->Order() < offset) || ((*i)->Order() >= (offset + MAX_ENTRY_VIEW)))) {
			(*i)->Change_state(OUTOFBOX);
		}
	}
}

void QuestContainer::Init_text(){
	sf::Vector2f pos;
	text.setFont(*game->Get_font());

	pos.y = TEXT_Y;
	pos.x = SCREENW / 2 - text.getLocalBounds().width / 2 - text.getLocalBounds().left;

	text.setPosition(pos);
}

bool QuestContainer::MouseWithinBounds(sf::Vector2i pos){
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

void QuestContainer::LMB_Pressed(sf::Vector2i pos){
	for (std::list<Quest_entry*>::iterator i = entries.begin(); i != entries.end(); i++) {
		if (!(*i)->MouseWithinBounds(pos) && (*i)->Is_highlighted()) {
			(*i)->Change_state(SELECTDESELECT);
		}
	}
	for (std::list<Quest_entry*>::iterator i = entries.begin(); i != entries.end(); i++) {
		if ((*i)->MouseWithinBounds(pos)) {
			(*i)->Change_state(SELECTDESELECT);
		}
	}
}

QuestContainer::QuestContainer() {
	text.setStyle(sf::Text::Underlined);
	text.setCharacterSize(35);
	text.setFillColor(sf::Color::White);
	text.setString("Questii");

	bounding_box.setPrimitiveType(sf::Quads);
	bounding_box.resize(4);

	sf::Vertex* quad = &bounding_box[0];

	quad[0].position = sf::Vector2f(QUESTC_X, QUESTC_Y);
	quad[1].position = sf::Vector2f(QUESTC_X + QUESTC_W, QUESTC_Y);
	quad[2].position = sf::Vector2f(QUESTC_X + QUESTC_W, QUESTC_Y + QUESTC_H);
	quad[3].position = sf::Vector2f(QUESTC_X, QUESTC_Y + QUESTC_H);

	offset = 0;
}
