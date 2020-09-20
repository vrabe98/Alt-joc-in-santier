#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include "TextBox.h"

#define BUTTON_TYPES_NUMBER 14

class Button: public sf::Drawable {
    sf::VertexArray corners;
    sf::Color button_colors[2];
    sf::Text button_text;
    bool pressed;
    int nextstate;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
    enum Action {   //screen file button action codes
        sell = -1 * BUTTON_TYPES_NUMBER,
        buy,minus10sec,
        plus10sec, nonpltopl, pltononpl, loopseq, next,
        prev, playpause, inc_cha, inc_con, inc_dex, inc_str,
        ext, main_menu, char_creation, map_view, char_view,
        encyclopedia, music, inventory_transfer
    };
    int MouseWithinBounds(sf::Vector2i);
    void UpdateText(std::string);
    void Change_state();
    int Nextstate() { return nextstate; }
    bool IsPressed() { return pressed; }
    void Create_colors(sf::Color color);
    Button(std::ifstream&);
    Button(sf::Vector2f pos, sf::Vector2f size,std::string text,sf::Color color);
    Button() {}
    int (*function_pointer)(Button*);
};