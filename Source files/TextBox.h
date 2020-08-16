#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class TextBox:public sf::Drawable
{
protected:
	sf::VertexArray corners;
	sf::Text text;
	int highlighted;
	int string_code;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
public:
	virtual void Action() = 0;
	virtual void Update() = 0;
	int Is_highlighted() { return highlighted + 1; }
	void Change_state();
	int MouseWithinBounds(sf::Vector2i);
	inline std::string GetText() {
		return text.getString();
	}
	void Append_char(char);
	TextBox(std::ifstream&, sf::Font&);
};

class InputBox :public TextBox {
public:
	void Action();
	void Update();
	InputBox(std::ifstream&f,sf::Font& font):TextBox(f,font){}
};

class OutputBox : public TextBox {
public:
	void Action();
	void Update();
	OutputBox(std::ifstream& f, sf::Font& font):TextBox(f, font) {}
};
