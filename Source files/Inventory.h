#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "Item.h"

class Item;

#define ITEM_INFO 6

#define HALFW 512

#define PLAYER_INV 0
#define NONPLAYER_INV 1

#define XPADDING 31
#define YPADDING 34

#define INVW 450
#define INVH 400

#define SLOTW 450
#define SLOTH 50

#define IMGW 50
#define IMGH 50

#define NUMW 50
#define NUMH 50

#define NAMEW 350
#define NAMEH 50

#define MAX_ITEMS_VIEW 8

#define OUTOFBOX 0
#define SELECTDESELECT 1

class InventorySlot:public sf::Drawable {
	int order, highlighted, type;
	Item* item;
	sf::Text order_text;
	sf::Text name_text;
	sf::VertexArray img_vertices, bounding_box;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	Item* Get_item() {
		return item;
	}
	bool MouseWithinBounds(sf::Vector2i pos);
	int Is_highlighted() { return highlighted + 1; }
	void Change_state(int action);
	void Draw(sf::RenderTarget& target, int offset);
	void Init_texts();
	void UpdateOrder(int neword);
	int Order() { return order; }
	InventorySlot(Item* item, int num, int type) {
		this->item = item;
		this->type = type;
		bounding_box.setPrimitiveType(sf::Quads);
		bounding_box.resize(4);
		sf::Vertex* quad = &bounding_box[0];
		for (int i = 0; i < 4; i++) {
			quad[i].color = sf::Color(50, 50, 50,200);
		}
		img_vertices.setPrimitiveType(sf::Quads);
		img_vertices.resize(4);
		quad = &img_vertices[0];
		quad[0].texCoords = sf::Vector2f(0, 0);
		quad[1].texCoords = sf::Vector2f(IMGW, 0);
		quad[2].texCoords = sf::Vector2f(IMGW, IMGH);
		quad[3].texCoords = sf::Vector2f(0, IMGH);
		order = num;
		highlighted = -1;
	}
};

class Inventory:public sf::Drawable{
	int offset, type;
	std::list<InventorySlot*> slots;
	sf::VertexArray bounding_box;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	bool MouseWithinBounds(sf::Vector2i pos);
	void Remove_item(int index) {
		int ind = 0;
		std::list<InventorySlot*>::iterator i = slots.begin();
		advance(i, index);
		slots.erase(i);
		for (std::list<InventorySlot*>::iterator i = slots.begin(); i != slots.end(); i++) {
			(*i)->UpdateOrder(ind++);
		}
	}
	void Add_item(Item* item) {
		if (item != nullptr) {
			InventorySlot* itemslot = new InventorySlot(item, slots.size(),type);
			itemslot->Init_texts();
			slots.push_back(itemslot);
		}
	}
	void Wheel_scroll(int delta) {
		offset += delta;
		if (offset < 0) offset = 0;
		else if (offset >= Size()) offset = Size() - 1;
		for (std::list<InventorySlot*>::iterator i = slots.begin(); i != slots.end(); i++) {
			if ((*i)->Is_highlighted()&&(((*i)->Order()<offset)||((*i)->Order()>=(offset+MAX_ITEMS_VIEW)))) {
				(*i)->Change_state(OUTOFBOX);
			}
		}
	}
	int Size() { return slots.size(); }
	void LMB_Pressed(sf::Vector2i pos);
	InventorySlot* Get_highlighted() {
		for (std::list<InventorySlot*>::iterator i = slots.begin(); i != slots.end(); i++) {
			if ((*i)->Is_highlighted()) {
				return *i;
			}
		}
		return nullptr;
	}
	Inventory(int type) {
		this->type = type;
		bounding_box.setPrimitiveType(sf::Quads);
		bounding_box.resize(4);
		sf::Vertex* quad = &bounding_box[0];
		if (type == PLAYER_INV) {
			quad[0].position = sf::Vector2f(XPADDING, YPADDING);
			quad[1].position = sf::Vector2f(XPADDING + INVW, YPADDING);
			quad[2].position = sf::Vector2f(XPADDING + INVW, YPADDING + INVH);
			quad[3].position = sf::Vector2f(XPADDING, YPADDING + INVH);
		}
		else if (type == NONPLAYER_INV) {
			quad[0].position = sf::Vector2f(XPADDING+HALFW, YPADDING);
			quad[1].position = sf::Vector2f(XPADDING+HALFW + INVW, YPADDING);
			quad[2].position = sf::Vector2f(XPADDING+HALFW + INVW, YPADDING + INVH);
			quad[3].position = sf::Vector2f(XPADDING+HALFW, YPADDING + INVH);
		}
		for (int i = 0; i < 4; i++) {
			quad[i].color = sf::Color(50, 50, 50, 200);
		}
		offset = 0;
	}
};