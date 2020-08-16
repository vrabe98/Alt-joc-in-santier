#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

#define ENCYCLO_DESCR 8

#define XPAD 31
#define YPAD 50

#define ENCYW 450
#define ENCYH 300

#define NUMW 50
#define NUMH 50

#define ENTRYW 400
#define ENTRYH 50

#define MAX_ENTRY_VIEW 6
#define LIGHTEN_FACTOR 40

#define OUTOFBOX 0
#define SELECTDESELECT 1

class Entry:public sf::Drawable {
	int order, highlighted, code;
	sf::Text name_text;
	sf::Text order_text;
	sf::VertexArray bounding_box;
	std::string name, description;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	int MouseWithinBounds(sf::Vector2i pos);
	int Order() { return order; }
	int Is_highlighted() { return highlighted + 1; }
	void Change_state(int action);
	void Draw(sf::RenderTarget& target, int offset);
	void Update(std::string name, std::string description);
	void Update_order(int neword) {
		order = neword;
	}
	int Id() { return code; }
	void Init();
	Entry(std::string name, std::string description, int num,int code) {
		this->name = name;
		this->description = name+"\n\n"+description;
		this->code = code;
		this->order = num;
		bounding_box.setPrimitiveType(sf::Quads);
		bounding_box.resize(4);
		sf::Vertex* quad = &bounding_box[0];
		for (int i = 0; i < 4; i++) {
			quad[i].color = sf::Color(50, 50, 50);
		}
		highlighted = -1;
	}
};

class Encyclopedia:public sf::Drawable{
	int offset;
	std::list<Entry*> entries;
	sf::VertexArray bounding_box;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	int MouseWithinBounds(sf::Vector2i pos);
	int Size() { return entries.size(); }
	void LMB_Pressed(sf::Vector2i pos);
	void Try_add(std::string name, std::string description, int code);
	void Update(std::string name, std::string description, int code);
	void Wheel_scroll(int delta) {
		offset += delta;
		if (offset < 0) offset = 0;
		else if (offset >= Size()) offset = Size() - 1;
		for (std::list<Entry*>::iterator i = entries.begin(); i != entries.end(); i++) {
			if ((*i)->Is_highlighted() && (((*i)->Order() < offset) || ((*i)->Order() >= (offset + MAX_ENTRY_VIEW)))) {
				(*i)->Change_state(OUTOFBOX);
			}
		}
	}
	Encyclopedia() {
		bounding_box.setPrimitiveType(sf::Quads);
		bounding_box.resize(4);
		sf::Vertex* quad = &bounding_box[0];
		quad[0].position = sf::Vector2f(XPAD, YPAD);
		quad[1].position = sf::Vector2f(XPAD + ENCYW, XPAD);
		quad[2].position = sf::Vector2f(XPAD + ENCYW, YPAD + ENCYH);
		quad[3].position = sf::Vector2f(XPAD, YPAD + ENCYH);
		offset = 0;
	}
};

