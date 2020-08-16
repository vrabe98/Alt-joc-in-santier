#include "Inventory.h"
#include "Game.h"

#define LIGHTEN_FACTOR 40
#define ITEM_STRING 6

extern Game* game;

void InventorySlot::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	state.texture = item->Get_texture();
	target.draw(bounding_box);
	target.draw(img_vertices, state);
	target.draw(order_text);
	target.draw(name_text);
}

void InventorySlot::Init_texts() {
	order_text.setFont(*game->Get_font());
	name_text.setFont(*game->Get_font());
	order_text.setString(std::to_string(order));
	name_text.setString(item->Name());
	name_text.setStyle(sf::Text::Regular);
	name_text.setFillColor(sf::Color::White);
	order_text.setStyle(sf::Text::Regular);
	order_text.setFillColor(sf::Color::White);
}

void InventorySlot::UpdateOrder(int neword){
	order = neword;
}

int InventorySlot::MouseWithinBounds(sf::Vector2i pos){
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

void InventorySlot::Change_state(int action){
	sf::Color color = bounding_box[0].color;
	highlighted = -1 * highlighted;
	color.r += highlighted * LIGHTEN_FACTOR;
	color.g += highlighted * LIGHTEN_FACTOR;
	color.b += highlighted * LIGHTEN_FACTOR;
	for (int i = 0; i < 4; i++) {
		bounding_box[i].color = color;
	}
	if (highlighted == 1) game->Update_string(ITEM_STRING, &item->Info());
	else if (action==OUTOFBOX) game->Update_string(ITEM_STRING, &std::string(""));
}

void InventorySlot::Draw(sf::RenderTarget& target, int offset){
	double coeff = 141.0 / 72.0, font_size;

	img_vertices.setPrimitiveType(sf::Quads);
	img_vertices.resize(4);

	order_text.setString(std::to_string(order));
	font_size = NUMW * coeff / (((std::string)order_text.getString()).length() + 3.0f);
	if (font_size >= NUMH) font_size = NUMH;
	order_text.setCharacterSize(font_size);

	name_text.setString(item->Name());
	font_size = NAMEW * coeff / (((std::string)name_text.getString()).length() + 3.0f);
	if (font_size >= NAMEH) font_size = NAMEH;
	name_text.setCharacterSize(font_size);

	if (type == PLAYER_INV) {
		sf::Vertex* quad = &bounding_box[0];
		order_text.setPosition(XPADDING + (NUMW - order_text.getLocalBounds().width) / 2, YPADDING + (order - offset) * NUMH + (NUMH - order_text.getLocalBounds().height) / 2 - order_text.getLocalBounds().top);
		name_text.setPosition(XPADDING + NUMW + IMGW + (NAMEW - name_text.getLocalBounds().width) / 2, YPADDING + (order - offset) * NAMEH + (NAMEH - name_text.getLocalBounds().height) / 2 - name_text.getLocalBounds().top);
		quad[0].position = sf::Vector2f(XPADDING, YPADDING + (order - offset) * NUMH);
		quad[1].position = sf::Vector2f(XPADDING + NUMW + IMGW + NAMEW, YPADDING + (order - offset) * NUMH);
		quad[2].position = sf::Vector2f(XPADDING + NUMW + IMGW + NAMEW, YPADDING + (order - offset) * NUMH + IMGH);
		quad[3].position = sf::Vector2f(XPADDING, YPADDING + (order - offset) * NUMH + IMGH);
		quad = &img_vertices[0];

		quad[0].position = sf::Vector2f(XPADDING + NUMW, YPADDING + (order - offset) * NUMH);
		quad[1].position = sf::Vector2f(XPADDING + NUMW + IMGW, YPADDING + (order - offset) * NUMH);
		quad[2].position = sf::Vector2f(XPADDING + NUMW + IMGW, YPADDING + (order - offset) * NUMH + IMGH);
		quad[3].position = sf::Vector2f(XPADDING + NUMW, YPADDING + (order - offset) * NUMH + IMGH);
	}
	else if(type==NONPLAYER_INV) {
		sf::Vertex* quad = &bounding_box[0];
		order_text.setPosition(XPADDING+HALFW + (NUMW - order_text.getLocalBounds().width) / 2, YPADDING + (order - offset) * NUMH + (NUMH - order_text.getLocalBounds().height) / 2 - order_text.getLocalBounds().top);
		name_text.setPosition(XPADDING+HALFW + NUMW + IMGW + (NAMEW - name_text.getLocalBounds().width) / 2, YPADDING + (order - offset) * NAMEH + (NAMEH - name_text.getLocalBounds().height) / 2 - name_text.getLocalBounds().top);
		quad[0].position = sf::Vector2f(XPADDING+HALFW, YPADDING + (order - offset) * NUMH);
		quad[1].position = sf::Vector2f(XPADDING +HALFW+ NUMW + IMGW + NAMEW, YPADDING + (order - offset) * NUMH);
		quad[2].position = sf::Vector2f(XPADDING+HALFW + NUMW + IMGW + NAMEW, YPADDING + (order - offset) * NUMH + IMGH);
		quad[3].position = sf::Vector2f(XPADDING+HALFW, YPADDING + (order - offset) * NUMH + IMGH);

		quad = &img_vertices[0];

		quad[0].position = sf::Vector2f(XPADDING+HALFW + NUMW, YPADDING + (order - offset) * NUMH);
		quad[1].position = sf::Vector2f(XPADDING+HALFW + NUMW + IMGW, YPADDING + (order - offset) * NUMH);
		quad[2].position = sf::Vector2f(XPADDING+HALFW + NUMW + IMGW, YPADDING + (order - offset) * NUMH + IMGH);
		quad[3].position = sf::Vector2f(XPADDING+HALFW + NUMW, YPADDING + (order - offset) * NUMH + IMGH);
	}
	target.draw(*this);
}

void Inventory::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	for (std::list<InventorySlot*>::const_iterator i = slots.begin(); i != slots.end() && ((*i)->Order() - offset) < MAX_ITEMS_VIEW; i++) {
		if ((*i)->Order() >= offset) {
			(*i)->Draw(target, offset);
		}
	}
}

int Inventory::MouseWithinBounds(sf::Vector2i pos)
{
    return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

void Inventory::LMB_Pressed(sf::Vector2i pos){
	for (std::list<InventorySlot*>::iterator i = slots.begin(); i != slots.end(); i++) {
		if (!(*i)->MouseWithinBounds(pos) && (*i)->Is_highlighted()) {
			(*i)->Change_state(SELECTDESELECT);
		}
	}
	for (std::list<InventorySlot*>::iterator i = slots.begin(); i != slots.end(); i++) {
		if ((*i)->MouseWithinBounds(pos)) {
			(*i)->Change_state(SELECTDESELECT);
		}
	}
}
