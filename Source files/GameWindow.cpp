#include "GameWindow.h"
#include <iostream>
#include "Map.h"
#include "Character.h"
#include "Inventory.h"
#include "Game.h"

extern Game* game;

int GameWindow::IsOpen() {
	return render_wnd.isOpen();
}

void GameWindow::Render(Map* map) {
	sf::RenderTarget& target = render_wnd;
	target.draw(*map);
}

void GameWindow::Render(Character* character) {
	sf::RenderTarget& target = render_wnd;
	target.draw(*character);
}

void GameWindow::Render(ItemSlot** slots){
	sf::RenderTarget& target = render_wnd;
	for (int i = 0; i < NUM_SLOTS; i++) {
		target.draw(*slots[i]);
	}
}

void GameWindow::Render(Encyclopedia* encyclopedia){
	sf::RenderTarget& target = render_wnd;
	target.draw(*encyclopedia);
}

void GameWindow::Render(MusicContainer* playlist){
	sf::RenderTarget& target = render_wnd;
	target.draw(*playlist);
}

void GameWindow::Render(Inventory* inventory) {
	sf::RenderTarget& target = render_wnd;
	target.draw(*inventory);
}

void GameWindow::Render(int current_state) {
	sf::Event e;
	sf::RenderTarget& target = render_wnd;
	retval = current_state;
	while (render_wnd.pollEvent(e)) {
		sf::Vector2i pos;
		switch (e.type) {
		case sf::Event::MouseWheelScrolled:
		if (current_state == CHARACTER_VIEW||current_state==ENCYCLOPEDIA||current_state==INV_TRANSFER) {
				pos.x=e.mouseWheelScroll.x;
				pos.y=e.mouseWheelScroll.y;
				game->Get_mainchar()->Wheel_Scroll(pos, -1*e.mouseWheelScroll.delta,current_state);
		}
		else if (current_state == MUSIC) {
			pos.x = e.mouseWheelScroll.x;
			pos.y = e.mouseWheelScroll.y;
			game->Get_playlist()->Wheel_scroll(pos, -1 * e.mouseWheelScroll.delta);
		}
		break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button == sf::Mouse::Left)
				pos.x = e.mouseButton.x;
				pos.y = e.mouseButton.y;
				states[current_state].LMB_Pressed(pos);
				if (current_state == CHARACTER_VIEW || current_state == ENCYCLOPEDIA||current_state==INV_TRANSFER) {
					game->Get_mainchar()->LMB_Pressed(pos, current_state);
				}
				else if (current_state == MUSIC) {
					game->Get_playlist()->LMB_Pressed(pos);
				}
				break;
		case sf::Event::MouseButtonReleased:
			if (e.mouseButton.button == sf::Mouse::Left)
				pos.x = e.mouseButton.x;
				pos.y = e.mouseButton.y;
				retval = states[current_state].LMB_Released(pos);
				break;
		case sf::Event::TextEntered:
			states[current_state].KeyPress(static_cast<char>(e.text.unicode));
			break;
		case sf::Event::Closed:
			render_wnd.close();
			retval = EXIT;
			break;
		}
	}
	render_wnd.clear();
	states[current_state].Draw(render_wnd);
}

void GameWindow::Load_screen(std::ifstream& file, int id, int num_panels, int num_buttons,int num_textboxes) {
	states[id].Load(file, num_panels, num_buttons,num_textboxes,*font);
}