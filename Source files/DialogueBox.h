#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "Encyclopedia.h"
#include "Inventory.h"
#include "RoundedRectangle.h"
#include "DialogueState.h"

#define NPC_MSG 0
#define MAINCHAR_MSG 1

#define MAX_SHOWN_MESSAGES 3

#define LIGHTEN_FACTOR 40

class DialogueMessage:public sf::Drawable {
protected:
	sf::VertexArray bounding_box;
	sf::Text text;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const {
		target.draw(bounding_box);
		target.draw(text);
	}
public:
	DialogueMessage() {
		bounding_box.setPrimitiveType(sf::Quads);
		bounding_box.resize(4);
		sf::Vertex* quad = &bounding_box[0];
		for (int i = 0; i < 4; i++) {
			quad[i].color = sf::Color(50, 50, 50);
		}
	}
	virtual ~DialogueMessage(){}
	int MouseWithinBounds(sf::Vector2i pos);
	virtual void Draw(sf::RenderTarget& target,int radius,sf::Vector2f pos,sf::Vector2f size,int offset)=0;
	virtual DialogueState* Action() = 0;
};

class NPCMessage:public DialogueMessage {
	int npc_id;
public:
	NPCMessage(){}
	NPCMessage(std::string message,int npc_id);
	int Npc_id() { return npc_id; }
	void Draw(sf::RenderTarget& target,int radius, sf::Vector2f pos, sf::Vector2f size,int offset) override;
	DialogueState* Action() override { return nullptr; }
};

class MaincharMessage :public DialogueMessage {
	int order;
	DialogueState* next;
public:
	MaincharMessage() {}
	MaincharMessage(std::string message,int order,DialogueState* next);
	void MouseOver(sf::Vector2i pos) {
		if (MouseWithinBounds(pos)) {
			sf::Vertex* quad = &bounding_box[0];
			if (next != nullptr) {
				for (int i = 0; i < 4; i++) {
					quad[i].color = sf::Color(50 + LIGHTEN_FACTOR, 50 + LIGHTEN_FACTOR, 50 + LIGHTEN_FACTOR);
				}
			}
			else {
				for (int i = 0; i < 4; i++) {
					quad[i].color = sf::Color::Red;
				}
			}
		}
		else {
			sf::Vertex* quad = &bounding_box[0];
			for (int i = 0; i < 4; i++) {
				quad[i].color = sf::Color(50, 50, 50);
			}
		}
	}
	void Update_order(int neword) {
		order = neword;
	}
	void Draw(sf::RenderTarget& target,int radius, sf::Vector2f pos, sf::Vector2f size,int offset) override;
	DialogueState* Action() override {
		return next;
	}
};

class DialogueBox {
	int active, state, offset;
	NPCMessage npc_msg;
	std::list<MaincharMessage*> char_msgs;
	RoundedRectangle box;
public:
	DialogueBox() {
		state = NPC_MSG;
		offset = 0;
		active = -1;
		box = RoundedRectangle(25,sf::Vector2f(75,493),sf::Vector2f(700,200));
	}
	int Size() { return char_msgs.size(); }
	void Draw(sf::RenderTarget& target) {
		target.draw(box);
		if (this->state == NPC_MSG) {
			npc_msg.Draw(target, box.Radius(), box.Position(), box.Size(),offset);
		}
		else if (this->state == MAINCHAR_MSG) {
			int msgs = 0;
			std::list<MaincharMessage*>::iterator i = char_msgs.begin();
			std::advance(i, offset);
			while (i != char_msgs.end()&&msgs!=MAX_SHOWN_MESSAGES) {
				(*i)->Draw(target, box.Radius(), box.Position(), box.Size(), offset);
				msgs++;
				i++;
			}
		}

	}
	void LMB_Press(sf::Vector2i pos) {
		if (MouseWithinBounds(pos)) {
			if (state == MAINCHAR_MSG) {
				DialogueState* next = nullptr;
				for (std::list<MaincharMessage*>::iterator i = char_msgs.begin(); i != char_msgs.end(); i++) {
					if ((*i)->MouseWithinBounds(pos)) {
						next = (*i)->Action();
						if (next == nullptr) Activate_deactivate();
						else Enter_dialogue(next, npc_msg.Npc_id());
						return;
					}
				}
			}
			else if (state == NPC_MSG) Change_state();
		}
	}
	void Wheel_scroll(int delta) {
		offset += delta;
		if (offset < 0) offset = 0;
		else if (offset >= Size()) offset = Size() - 1;
	}
	void MouseMoved(sf::Vector2i pos) {
		if (Active()) {
			if(state==MAINCHAR_MSG){
				int message_count = 0;
				std::list<MaincharMessage*>::iterator i = char_msgs.begin();
				std::advance(i, offset);
				while (i != char_msgs.end() && message_count <= MAX_SHOWN_MESSAGES) {
					message_count++;
					(*i)->MouseOver(pos);
					i++;
				}
			}
		}
	}
	void Enter_dialogue(DialogueState* entry,int npc_id);
	int MouseWithinBounds(sf::Vector2i pos) {
		return box.MouseWithinBounds(pos);
	}
	int Active() { return active+1; }
	void Change_state() {
		offset = 0;
		state=(state+1)%2;
	}
	void Activate_deactivate() {
		active = -1 * active;
		offset = 0;
	}
};

