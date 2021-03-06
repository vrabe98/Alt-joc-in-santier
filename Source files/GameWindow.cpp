#include "GameWindow.h"
#include "Game.h"

extern Game* game;

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

void GameWindow::Render(DialogueBox* diag,int code){
	sf::RenderTarget& target = render_wnd;
	diag->Draw(target,code);
}

void GameWindow::Render(CombatScreen* combat){
	sf::RenderTarget& target = render_wnd;
	combat->Draw(target);
}

void GameWindow::Render(QuestContainer* quest_container){
	sf::RenderTarget& target = render_wnd;
	target.draw(*quest_container);
}

void GameWindow::Render(Inventory* inventory) {
	sf::RenderTarget& target = render_wnd;
	target.draw(*inventory);
}

void GameWindow::Render(short int current_state) {
	sf::Event e;
	sf::RenderTarget& target = render_wnd;
	retval = current_state;
	while (render_wnd.pollEvent(e)) {
		sf::Vector2i pos;
		switch (e.type) {
		case sf::Event::MouseWheelScrolled:
			pos.x = e.mouseWheelScroll.x;
			pos.y = e.mouseWheelScroll.y;
			if (current_state == CHARACTER_VIEW||current_state==ENCYCLOPEDIA||current_state==INV_TRANSFER||current_state==TRADE_SCREEN) {
					game->Get_mainchar()->Wheel_Scroll(pos, -1*e.mouseWheelScroll.delta,current_state);
			}
			else if (current_state == MUSIC) {
				game->Get_playlist()->Wheel_scroll(pos, -1 * e.mouseWheelScroll.delta);
			}
			else if (current_state == MAP_VIEW) {
				if (game->Get_dialoguebox()->Active())
					game->Get_dialoguebox()->Wheel_scroll(-1 * e.mouseWheelScroll.delta);
				else if (game->Get_combatscreen()->Active())
					game->Get_combatscreen()->Wheel_scroll(-1 * e.mouseWheelScroll.delta);
			}
			else if (current_state == QUEST_SCREEN) {
				game->Get_quest_container()->Wheel_scroll(-1 * e.mouseWheelScroll.delta);
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button == sf::Mouse::Left)
				pos.x = e.mouseButton.x;
				pos.y = e.mouseButton.y;
				states[current_state].LMB_Pressed(pos);
				if (current_state == CHARACTER_VIEW || current_state == ENCYCLOPEDIA||current_state==INV_TRANSFER||current_state==TRADE_SCREEN) {
					game->Get_mainchar()->LMB_Pressed(pos, current_state);
				}
				else if (current_state == MUSIC) {
					game->Get_playlist()->LMB_Pressed(pos);
				}
				else if (current_state == MAP_VIEW) {
					game->LMB_Pressed(pos);
				}
				else if (current_state == QUEST_SCREEN) {
					game->Get_quest_container()->LMB_Pressed(pos);
				}
				break;
		case sf::Event::MouseButtonReleased:
			if (e.mouseButton.button == sf::Mouse::Left)
				pos.x = e.mouseButton.x;
				pos.y = e.mouseButton.y;
				if (current_state == MAP_VIEW) {
					if (game->Get_dialoguebox()->Active())
						game->Get_dialoguebox()->LMB_Released(pos);
					else if (game->Get_combatscreen()->Active())
						game->Get_combatscreen()->LMB_Released(pos);
					else{ retval = states[current_state].LMB_Released(pos); }
				}
				else { retval = states[current_state].LMB_Released(pos); }
				break;
		case sf::Event::MouseMoved:
			pos.x = e.mouseMove.x;
			pos.y = e.mouseMove.y;
			game->MouseMoved(pos);
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