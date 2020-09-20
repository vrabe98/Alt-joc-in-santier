#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <thread>

#define CURRENT_SONG 9
#define PLAY_TIME 10

#define VOLW 200
#define VOLH 25

#define VOLX 212
#define VOLY 667

#define PROGW 600
#define PROGH 25

#define PROGX 212
#define PROGY 620

#define XPADM 112
#define YPADM 134

#define MUSICW 800
#define MUSICH 450

#define SONGNAMW 750
#define SONGNAMH 50

#define SONGORDW 50
#define SONGORDH 50

#define MAX_SONGS_VIEW 7

#define LIGHTEN_FACTOR 40

#define OUTOFBOX 0
#define SELECTDESELECT 1

class VolumeBar : public sf::Drawable {
	sf::VertexArray bounding_box, volume_box;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	bool MouseWithinBounds(sf::Vector2i pos);
	float Percent_click(sf::Vector2i pos) {
		return ((float)pos.x - VOLX) / VOLW * 100;
	}
	void Update(float percent) {
		sf::Vertex* quad = &volume_box[0];
		quad[1].position = sf::Vector2f(VOLX + (VOLW * percent) / 100, VOLY);
		quad[2].position = sf::Vector2f(VOLX + (VOLW * percent) / 100, VOLY + VOLH);
	}
	VolumeBar();
};

class ProgressBar :public sf::Drawable {
	sf::VertexArray bounding_box, progress_box;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	bool MouseWithinBounds(sf::Vector2i pos);
	float Percent_click(sf::Vector2i pos) {
		return ((float)pos.x - PROGX) / PROGW*100;
	}
	void Update(float percent) {
		sf::Vertex* quad = &progress_box[0];
		quad[1].position = sf::Vector2f(PROGX+(PROGW*percent)/100, PROGY);
		quad[2].position = sf::Vector2f(PROGX+(PROGW*percent)/100, PROGY + PROGH);
	}
	ProgressBar();
};

class Song :public sf::Drawable {
	bool highlighted;
	short int order, code;
	sf::Text name_text;
	sf::Text order_text;
	sf::VertexArray bounding_box;
	std::string name, description;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	bool MouseWithinBounds(sf::Vector2i pos);
	std::string Name() { return name; }
	short int Order() { return order; }
	bool Is_highlighted() { return highlighted; }
	void Change_state(short int action);
	void Draw(sf::RenderTarget& target, short int offset);
	void Init();
	void Update_order(int neword) {
		order = neword;
	}
	Song(std::string name,short int num) {
		this->name = name;
		this->order = num;
		bounding_box.setPrimitiveType(sf::Quads);
		bounding_box.resize(4);
		sf::Vertex* quad = &bounding_box[0];
		for (int i = 0; i < 4; i++) {
			quad[i].color = sf::Color(50, 50, 50,200);
		}
		highlighted = false;
	}
};

class MusicContainer :public sf::Drawable {
	short int offset, current_song_order;
	std::thread* play_time_calc;
	std::list<Song*> songs;
	sf::VertexArray bounding_box;
	ProgressBar prog_bar;
	VolumeBar vol_bar;
	sf::Music music;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	ProgressBar* Get_progbar() {
		return &prog_bar;
	}
	VolumeBar* Get_volbar() {
		return &vol_bar;
	}
	int Loop_change() {
		music.setLoop(!music.getLoop());
		return music.getLoop();
	}
	int Change_playing_state() {
		if (music.getStatus() == sf::Music::Playing) {
			music.pause();
			return 1;
		}
		else if (music.getStatus() == sf::Music::Paused) {
			music.play();
			return 0;
		}
		return 1;
	}
	sf::Music* Get_music() { return &music; }
	void Begin_timecalc();
	bool MouseWithinBounds(sf::Vector2i pos);
	void Add_song(std::string name);
	int Size() { return songs.size(); }
	void LMB_Pressed(sf::Vector2i pos);
	void Change_song(std::string name,int order);
	void Change_song(int direction);	//direction=-1 for prev, 1 for next
	void Wheel_scroll(sf::Vector2i pos,int delta) {
		if (MouseWithinBounds(pos)) {
			offset += delta;
			if (offset < 0) offset = 0;
			else if (offset >= Size()) offset = Size() - 1;
			for (std::list<Song*>::iterator i = songs.begin(); i != songs.end(); i++) {
				if ((*i)->Is_highlighted() && (((*i)->Order() < offset) || ((*i)->Order() >= (offset + MAX_SONGS_VIEW)))) {
					(*i)->Change_state(OUTOFBOX);
				}
			}
		}
	}
	MusicContainer();
};