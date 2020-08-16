#include "Encyclopedia.h"
#include "Game.h"

extern Game* game;

void Entry::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(bounding_box);
	target.draw(name_text);
	target.draw(order_text);
}

int Entry::MouseWithinBounds(sf::Vector2i pos)
{
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

void Entry::Change_state(int action){
	sf::Color color = bounding_box[0].color;
	highlighted = -1 * highlighted;
	color.r += highlighted * LIGHTEN_FACTOR;
	color.g += highlighted * LIGHTEN_FACTOR;
	color.b += highlighted * LIGHTEN_FACTOR;
	for (int i = 0; i < 4; i++) {
		bounding_box[i].color = color;
	}
	if (highlighted == 1) game->Update_string(ENCYCLO_DESCR, &description);
	else if (action == OUTOFBOX) game->Update_string(ENCYCLO_DESCR, &std::string(""));
}

void Entry::Draw(sf::RenderTarget& target, int offset){
	double coeff = 141.0 / 72.0, font_size;
	sf::Vertex* quad = &bounding_box[0];

	name_text.setString(name);
	font_size = ENTRYW * coeff / (((std::string)name_text.getString()).length() + 3.0f);
	if (font_size >= ENTRYH) font_size = ENTRYH;
	name_text.setCharacterSize(font_size);
	name_text.setPosition(XPAD+ NUMW+(ENTRYW - name_text.getLocalBounds().width) / 2, YPAD + (order - offset) * ENTRYH + (ENTRYH - name_text.getLocalBounds().height) / 2 - name_text.getLocalBounds().top);

	order_text.setString(std::to_string(order)+". ");
	font_size = NUMW * coeff / (((std::string)order_text.getString()).length() + 3.0f);
	if (font_size >= NUMH) font_size = NUMH;
	order_text.setCharacterSize(font_size);
	order_text.setPosition(XPAD + (NUMW - order_text.getLocalBounds().width) / 2, YPAD + (order - offset) * NUMH + (NUMH - order_text.getLocalBounds().height) / 2 - order_text.getLocalBounds().top);

	quad[0].position = sf::Vector2f(XPAD, YPAD + (order - offset) * ENTRYH);
	quad[1].position = sf::Vector2f(XPAD +NUMW+ ENTRYW, YPAD + (order - offset) * ENTRYH);
	quad[2].position = sf::Vector2f(XPAD+ NUMW+ENTRYW, YPAD + (order - offset+1) * ENTRYH);
	quad[3].position = sf::Vector2f(XPAD, YPAD + (order - offset+1) * ENTRYH);

	target.draw(*this);
}

void Entry::Update(std::string name, std::string description){
	this->name = name;
	this->description = description;
}

void Entry::Init(){
	order_text.setFont(*game->Get_font());
	name_text.setFont(*game->Get_font());
	order_text.setString(std::to_string(order)+". ");
	name_text.setString(name);
	name_text.setStyle(sf::Text::Regular);
	name_text.setFillColor(sf::Color::White);
	order_text.setStyle(sf::Text::Regular);
	order_text.setFillColor(sf::Color::White);
}

void Encyclopedia::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	for (std::list<Entry*>::const_iterator i = entries.begin(); i != entries.end() && ((*i)->Order() - offset) < MAX_ENTRY_VIEW; i++) {
		if ((*i)->Order() >= offset) {
			(*i)->Draw(target, offset);
		}
	}
}

int Encyclopedia::MouseWithinBounds(sf::Vector2i pos)
{
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

void Encyclopedia::LMB_Pressed(sf::Vector2i pos){
	for (std::list<Entry*>::iterator i = entries.begin(); i != entries.end(); i++) {
		if (!(*i)->MouseWithinBounds(pos) && (*i)->Is_highlighted()) {
			(*i)->Change_state(SELECTDESELECT);
		}
	}
	for (std::list<Entry*>::iterator i = entries.begin(); i != entries.end(); i++) {
		if ((*i)->MouseWithinBounds(pos)) {
			(*i)->Change_state(SELECTDESELECT);
		}
	}
}

void Encyclopedia::Try_add(std::string name, std::string description, int code){
	Entry* new_entry = nullptr;
	for (std::list<Entry*>::iterator i = entries.begin(); i != entries.end(); i++) {
		if ((*i)->Id() == code) return;
	}
	new_entry = new Entry(name, description, entries.size(), code);
	new_entry->Init();
	entries.push_back(new_entry);
}

void Encyclopedia::Update(std::string name, std::string description, int code){
	Entry* new_entry = nullptr;
	for (std::list<Entry*>::iterator i = entries.begin(); i != entries.end(); i++) {
		if ((*i)->Id() == code) {
			(*i)->Update(name, description);
			return;
		}
	}
	new_entry = new Entry(name, description, entries.size(), code);
	new_entry->Init();
	entries.push_back(new_entry);
}
