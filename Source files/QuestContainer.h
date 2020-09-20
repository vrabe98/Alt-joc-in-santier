#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <thread>
#include "Quest.h"

class Quest_entry: public sf::Drawable {
	bool highlighted;
	int order;
	Quest* quest;
	sf::Text name_text;
	sf::VertexArray status_img, bounding_box;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	bool MouseWithinBounds(sf::Vector2i pos);
	void Draw(sf::RenderTarget& target, int offset);
	void UpdateOrder(int neword) { order = neword; }
	int Order() { return order; }
	bool Is_highlighted() { return highlighted; }
	void Change_state(short int action);
	Quest_entry(Quest* quest, int order);
};

class QuestContainer:public sf::Drawable{
	short int offset;
	sf::Text text;
	std::list<Quest_entry*> entries;
	sf::VertexArray bounding_box;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	void Wheel_scroll(short int delta);
	int Size() { return entries.size(); }
	void Add(Quest* quest) {
		Quest_entry* entry = new Quest_entry(quest,Size());
		entries.push_back(entry);
	}
	void Init_text();
	bool MouseWithinBounds(sf::Vector2i pos);
	void LMB_Pressed(sf::Vector2i pos);
	QuestContainer();
};