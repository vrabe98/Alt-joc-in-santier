#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "Button.h"
#include "Encyclopedia.h"
#include "Inventory.h"
#include "RoundedRectangle.h"
#include "DialogueState.h"
#include "PictureBox.h"

#define PRELIMINARY -1
#define NPC_MSG 0
#define MAINCHAR_MSG 1

#define NPC_DIAG 0
#define VENDOR_DIAG 1

#define MAX_SHOWN_MESSAGES 3

#define LIGHTEN_FACTOR 40

#define MAINCHAROPTH 50
#define SPACING 10
#define YOFFSET 15

#define CHARNAMEW 100
#define CHARNAMEH 25

#define TRADE_SCREEN 10

class Character;

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
	bool MouseWithinBounds(sf::Vector2i pos);
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
		if(next!=nullptr)next->Set_flags();
		return next;
	}
};

class DialogueBox {
	bool active;
	short int state, offset;
	Button dialogue, combat, trade;
	PictureBox picbox;
	sf::Text char_name;
	NPCMessage npc_msg;
	std::list<MaincharMessage*> char_msgs;
	RoundedRectangle box;
public:
	DialogueBox() {};
	void Create();
	Character* Get_npc();
	int Size() { return char_msgs.size(); }
	void Draw(sf::RenderTarget& target,int code);
	void LMB_Press(sf::Vector2i pos);
	void LMB_Released(sf::Vector2i pos);
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
	void Enter_dialogue(DialogueState* entry,int npc_id,int state);
	bool MouseWithinBounds(sf::Vector2i pos) {
		return box.MouseWithinBounds(pos);
	}
	bool Active() { return active; }
	void Change_state() {
		offset = 0;
		state=(state+1)%2;
	}
	void Activate_deactivate() {
		active = !active;
		offset = 0;
	}
};