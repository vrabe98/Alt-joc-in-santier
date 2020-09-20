#include "TextBox.h"
#include "Game.h"

#define LIGHTEN_FACTOR 40

extern Game* game;

void TextBox::Append_char(char c) {
	double coeff = 141.0 / 72.0, font_size;
	float width = corners[2].position.x - corners[0].position.x;
	float height = corners[2].position.y-corners[0].position.y;
	std::string str = text.getString();
	str += c;
	text.setString(str);
	font_size = width * coeff / (str.length() + 4.0f);
	if (font_size >= height) font_size = height;
	if (font_size>= 25.0) font_size=25.0;
	text.setCharacterSize(font_size);
	text.setPosition(corners[0].position.x + (width - text.getLocalBounds().width) / 2, corners[0].position.y + (height- text.getLocalBounds().height )/ 2-text.getLocalBounds().top);
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(corners);
	target.draw(text);
}

bool TextBox::MouseWithinBounds(sf::Vector2i pos) {
	return (corners[0].position.x <= (float)pos.x) && ((float)pos.x <= corners[1].position.x) && (corners[0].position.y <= (float)pos.y) && ((float)pos.y <= corners[3].position.y);
}

void TextBox::Change_state()
{
	sf::Color color = corners[0].color;
	highlighted = -1 * highlighted;
	color.r += highlighted* LIGHTEN_FACTOR;
	color.g += highlighted*LIGHTEN_FACTOR;
	color.b += highlighted*LIGHTEN_FACTOR;
	color.a = 200;
	for (int i = 0; i < 4; i++) {
		corners[i].color = color;
	}
}

void InputBox::Action() {
	std::string str = text.getString();
	game->Update_string(string_code, &str);
	text.setString("");
	Change_state();
}

void OutputBox::Action() {
}

void InputBox::Update(){
}

void OutputBox::Update(){
	double coeff = 141.0 / 72.0, font_size, max_word_length = 0, num_words = 0;
	float width = corners[2].position.x - corners[0].position.x;
	int height = corners[2].position.y - corners[0].position.y;
	std::string str = *game->Get_string_from_db(string_code);
	char* cstring = new char[str.length() + 1];
	std::strcpy(cstring, str.c_str());
	char* p = std::strtok(cstring, "\n");
	while (p != 0)
	{
		if (strlen(p) > max_word_length) max_word_length = strlen(p);
		num_words++;
		p = std::strtok(NULL, "\n");
	}
	delete[] cstring;
	text.setString(str);
	font_size = width * coeff / (max_word_length + 4.0f);
	text.setCharacterSize(font_size);
	if (text.getLocalBounds().height >= height) {
		font_size = (height) /num_words - text.getLineSpacing()*(num_words-4);
		text.setCharacterSize(font_size);
	}
	if (text.getCharacterSize() >= 25.0) text.setCharacterSize(25.0);
	text.setPosition(corners[0].position.x + (width - text.getLocalBounds().width) / 2, corners[0].position.y + (height - text.getLocalBounds().height) / 2-text.getLocalBounds().top);
}

TextBox::TextBox(std::ifstream& file, sf::Font& font) {
	std::string aux;
	sf::Color colr = sf::Color::Black;
	int r, g, b;
	float width, height, posx, posy;
	corners = sf::VertexArray(sf::Quads, 4);
	file >> width >> height;
	file >> posx >> posy;
	file >> r >> g >> b;
	file >> string_code;
	colr.r = r;
	colr.g = g;
	colr.b = b;
	colr.a = 200;
	corners[0].position = sf::Vector2f(posx, posy);
	corners[1].position = sf::Vector2f(posx + width, posy);
	corners[2].position = sf::Vector2f(posx + width, posy + height);
	corners[3].position = sf::Vector2f(posx, posy + height);
	for (int i = 0; i < 4; i++) {
		corners[i].color = colr;
	}
	text.setFont(font);
	text.setString("");
	text.setStyle(sf::Text::Regular);
	text.setFillColor(sf::Color::White);
	file >> aux;	//terminator string, reusing a string variable
	if (aux != ";;") {
		std::cout << aux;
		exit(-7);
	}
	highlighted = -1;
}