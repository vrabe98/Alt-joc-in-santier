#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <thread>
#include "RoundedRectangle.h"
#include "Button.h"
#include "PictureBox.h"

#define MAX_ENTRIES_VIEW 4

#define NPC_PIC 0
#define MAINCHAR_PIC 1

#define ROUND 0
#define ATTACKER_NAME 1

#define NPC_ROUND 0		//main character can have 1 point of initiative that gets added to the round number when determining the current attacker
#define MAINCHAR_ROUND 1

#define NPC_ 0
#define CHAR_ 1

#define ATTACK 0
#define RUN 1

#define LOG_H 200

#define LOGENTRY_H 50 //4 log entries can be shown at a time

class Character;

class Combat_log_entry:public sf::Drawable {
	int order;
	sf::Text text;
	sf::VertexArray bounding_box;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	void Draw(sf::RenderTarget& target, int offset);
	bool MouseWithinBounds(sf::Vector2i pos);
	int Order() { return order; }
	void Update_order(int neword) {
		order = neword;
	}
	Combat_log_entry(std::string text, int order);
};

class Combat_log:public sf::Drawable {
	int offset;
	std::list<Combat_log_entry*> entries;
	sf::VertexArray bounding_box;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	void Add(std::string text);
	void Init();
	void Erase() {
		offset = 0;
		entries.clear();
	}
	int Size() { return entries.size(); }
	void Set_offset(int off) {
		offset = off;
	}
	void Wheel_scroll(int delta) {
		offset += delta;
		if (offset < 0) offset = 0;
		else if (offset >= Size()) offset = Size() - 1;
	}
	bool MouseWithinBounds(sf::Vector2i pos);
	Combat_log();
};

class HP_Bar{
	sf::VertexArray bounding_box, hp_box;
	sf::Text text;
public:
	HP_Bar() {};
	HP_Bar(int type);
	void Draw(sf::RenderTarget& target, int type);
	sf::Vector2f Position() {
		return bounding_box[0].position;
	};
	sf::Vector2f Size() {
		float w, h;
		w = bounding_box[1].position.x-bounding_box[0].position.x;
		h = bounding_box[2].position.y-bounding_box[1].position.y;
		return sf::Vector2f(w, h);
	};
	void Update_prog(float percent);
};

class CombatScreen
{
	int active, round, npc_id;
	Character* attacker, * defender;
	std::thread* hp_thread[2];
	RoundedRectangle box;
	PictureBox pics[2];
	sf::Text texts[2];
	sf::Text combat_log_title;
	Button buttons[2];
	HP_Bar hp_bars[2];
	sf::VertexArray separator;
	Combat_log log;
public:
	CombatScreen();
	Character* Attacker() { return attacker; }
	Character* Defender() { return defender; }
	void Change_round() {
		Character* aux = defender;
		defender = attacker;
		attacker = aux;
		round++;
	}
	int Round() { return round; }
	float Radius() { return box.Radius(); }
	HP_Bar* Bar(int id) { return &hp_bars[id]; }
	void Add_entry(std::string text) { 
		log.Add(text); 
		log.Set_offset(std::max(0, log.Size() - MAX_ENTRIES_VIEW));
	}
	int NPC_id() { return npc_id; }
	void Wheel_scroll(int delta) { log.Wheel_scroll(delta); }
	void LMB_Press(sf::Vector2i pos);
	void LMB_Released(sf::Vector2i pos);
	void Draw(sf::RenderTarget& target);
	void Create_screen();
	void Enter_combat(int npc_id);
	int Active() { return active + 1; }
	void Activate_deactivate() {
		active = -1 * active;
	}
};