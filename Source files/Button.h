#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include "TextBox.h"

class Button: public sf::Drawable {
    sf::VertexArray corners;
    sf::Text button_text;
    int pressed, nextstate;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
    int MouseWithinBounds(sf::Vector2i);
    void UpdateText(std::string);
    void Change_state();
    int Nextstate() { return nextstate; }
    inline int IsPressed() { return pressed; }
    Button(std::ifstream&,sf::Font&);
    Button() {}
    int (*function_pointer)(Button*);
};