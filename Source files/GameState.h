#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include "Panel.h"
#include "Button.h"
#include "TextBox.h"

class GameState
{
	std::list<Button*> butoane;
	std::list<Panel> panels;
	std::list<TextBox*> textboxes;
public:
	void LMB_Pressed(sf::Vector2i);
	int LMB_Released(sf::Vector2i);
	void KeyPress(char);
	void Load(std::ifstream&,int,int,int,sf::Font&);
	void Draw(sf::RenderTarget&);
};

