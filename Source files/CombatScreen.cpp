#include "CombatScreen.h"
#include <Windows.h>
#include "Game.h"

extern Game* game;

#define BOX_Y 143
#define BOX_X 75

#define BOX_W 700
#define BOX_H 550

#define HP_W 200
#define HP_H 25

#define NPC_HP 11
#define MAINCHAR_HP 12

#define ROLL_MIN 0
#define ROLL_MAX 20

int Roll_d20_() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution <int> distrib(ROLL_MIN, ROLL_MAX);
	return distrib(gen);
}

int Attack(Button* buton) {
	double attacker_roll, crit = 1, glance = 1, hit = 0, counter = 1;
	CombatScreen* screen = game->Get_combatscreen();
	int dualwield = screen->Attacker()->DualWield() + 1;		//the do...while doesn't work well without this
	do {
		attacker_roll = Roll_d20_() + (screen->Attacker()->DEX() - 10.0) * 0.5 + screen->Attacker()->Has_1h() * 0.5 - screen->Defender()->Evasion();
		if (attacker_roll < 0) attacker_roll = 0;
		else if (attacker_roll > 20) attacker_roll = 20;
		if (attacker_roll == 0) {
			screen->Add_entry(screen->Attacker()->Name() + " face o mare greseala, care nu il va urmari\nchiar toata viata. Phew. Sau?");
			screen->Defender()->SetDmgBonus();
			hit = 0;
		}
		else if (attacker_roll < 5) {
			screen->Add_entry(screen->Attacker()->Name() + " habar n-are cum sa dea cu sabia-n dujman.\nHAAI BAA, VREI SA TE INVAT IO CUM SA TE BATI? A da nu is platit pentru asta.");
			hit = 0;
		}
		else if (attacker_roll < 10) {
			screen->Add_entry(screen->Attacker()->Name()+" da o lovitura anemica, dar macar\nil loveste pe "+screen->Defender()->Name());
			glance = 0.5;
			hit = 1;
		}
		else if (attacker_roll < 19) {
			screen->Add_entry(screen->Attacker()->Name() + " ii trage una serioasa lu " + screen->Defender()->Name() + ".\nRamaneti pe canal pentru a vedea deznodamantul!");
			hit = 1;
		}
		else if (attacker_roll == 20) {
			screen->Add_entry(screen->Attacker()->Name()+" IL DISTRUGE PE "+screen->Defender()->Name()+".\nCE LOVITURA, CE LOVItura, dragi telespectatori!");
			hit = 1;
			crit = 2.0f;
		}
		if (hit) {
			float base_dmg = 0;
			if (dualwield == 2) {
				base_dmg = screen->Attacker()->Offhand_Damage();
			}
			else if (dualwield == 1) {
				base_dmg = screen->Attacker()->Mainhand_Damage();
			}
			base_dmg = base_dmg * (1.0f + 0.1f * (screen->Attacker()->STR() - 10.0f)) * crit * glance * (1.0f + screen->Attacker()->HasDmgBonus() * 0.2f);
			if (crit == 1) {
				screen->Defender()->Defend(base_dmg, screen->Attacker(), 0, 0);
			}
			else if (crit == 1.5) {
				screen->Defender()->Defend(base_dmg, screen->Attacker(), 0, 1);
			}
		}
		if (screen->Attacker()->HasDmgBonus())	screen->Attacker()->ResetDmgBonus();
		dualwield--;
		if (game->Get_mainchar()->Died()) {
			game->Get_combatscreen()->Activate_deactivate();
			game->Change_state(DEATH);
			return 0;
		}
		else if (game->Get_char(screen->NPC_id())->Died()) {
			game->Get_combatscreen()->Activate_deactivate();
			return 0;
		}
	} while (dualwield);
	screen->Change_round();
	//Sleep(500);
	if (screen->Attacker() != game->Get_mainchar()) {
		Attack(buton);
	}
	return 0;
}

int Run(Button* buton) {
	game->Get_combatscreen()->Activate_deactivate();
	Sleep(100);
	return 0;
}

void HP_fill_mainchar() {
	int maxhp, hp_c;
	std::string hp = "";
	Character* mainchar = game->Get_mainchar();
	while (game->Get_combatscreen()->Active()) {
		maxhp = mainchar->Max_HP();
		hp_c = mainchar->HP();
		HP_Bar* bar = game->Get_combatscreen()->Bar(CHAR_);
		hp = std::to_string(hp_c) + " / " + std::to_string(maxhp);
		game->Update_string(MAINCHAR_HP, &hp);
		bar->Update_prog(100 * hp_c / maxhp);
		Sleep(100);
	}
}

void HP_fill_npc(int npc_id) {
	int maxhp, hp_c;
	std::string hp = "";
	Character* npc = game->Get_char(npc_id);
	while (game->Get_combatscreen()->Active()) {
		maxhp = npc->Max_HP();
		hp_c = npc->HP();
		HP_Bar* bar = game->Get_combatscreen()->Bar(NPC_);
		hp = std::to_string(hp_c) + " / " + std::to_string(maxhp);
		game->Update_string(NPC_HP, &hp);
		bar->Update_prog(100 * hp_c / maxhp);
		Sleep(100);
	}
}

void CombatScreen::LMB_Press(sf::Vector2i pos){
	for (int i = 0; i < 2; i++) {
		if (buttons[i].MouseWithinBounds(pos)) buttons[i].Change_state();
	}
}

void CombatScreen::LMB_Released(sf::Vector2i pos){
	for (int i = 0; i < 2; i++) {
		if (buttons[i].MouseWithinBounds(pos)) {
			buttons[i].Change_state();
			buttons[i].function_pointer(&buttons[i]);
		}
	}
}

void CombatScreen::Draw(sf::RenderTarget& target) {
	texts[ROUND].setString("Round: " + std::to_string(round));
	texts[ATTACKER_NAME].setString("Attacker: ");

	target.draw(box);
	target.draw(separator);
	target.draw(pics[MAINCHAR_PIC]);
	target.draw(pics[NPC_PIC]);
	target.draw(texts[ROUND]);
	target.draw(texts[ATTACKER_NAME]);
	target.draw(combat_log_title);
	hp_bars[NPC_].Draw(target, NPC_);
	hp_bars[CHAR_].Draw(target, CHAR_);
	target.draw(buttons[ATTACK]);
	target.draw(buttons[RUN]);
	target.draw(log);
}

CombatScreen::CombatScreen(){
	active = -1;

	separator.setPrimitiveType(sf::Lines);
	separator.resize(2);

	sf::Vertex* quad = &separator[0];

	separator[0].position = sf::Vector2f(BOX_X, BOX_Y + BOX_H / 2);
	separator[1].position = sf::Vector2f(BOX_X + BOX_W, BOX_Y + BOX_H / 2);

	box = RoundedRectangle(25, sf::Vector2f(BOX_X,BOX_Y), sf::Vector2f(BOX_W, BOX_H));

	pics[MAINCHAR_PIC] = PictureBox(sf::Vector2f(BOX_X+25,BOX_Y+50),sf::Vector2f(100,100));
	pics[NPC_PIC] = PictureBox(sf::Vector2f(BOX_X + 575, BOX_Y + 50), sf::Vector2f(100, 100));

	for (int i = 0; i < 2; i++) {
		texts[i].setStyle(sf::Text::Regular);
		texts[i].setFillColor(sf::Color::White);
	}
	texts[ATTACKER_NAME].setPosition(BOX_X + 125, BOX_Y + 25);
	texts[ROUND].setPosition(BOX_X + 375, BOX_Y + 25);

	combat_log_title.setStyle(sf::Text::Underlined);
	combat_log_title.setFillColor(sf::Color::White);
	combat_log_title.setString("Combat log");
}

void CombatScreen::Create_screen(){
	sf::Vector2f pos, size = sf::Vector2f(200,50);
	combat_log_title.setFont(*game->Get_font());
	combat_log_title.setCharacterSize(35);
	combat_log_title.setPosition(BOX_X + BOX_W / 2 - combat_log_title.getLocalBounds().width / 2, BOX_Y+BOX_H/2+5);

	for (int i = 0; i < 2; i++) {
		texts[i].setFont(*game->Get_font());
		texts[i].setCharacterSize(15);
	}
	texts[ATTACKER_NAME].setPosition(BOX_X + 125, BOX_Y + 10);
	texts[ROUND].setPosition(BOX_X + 375, BOX_Y + 10);

	pos = sf::Vector2f(box.Position().x + 125, box.Position().y + 175);
	buttons[ATTACK] = Button(pos, size, "Da-i una (teoretic)", sf::Color(100, 100, 100));
	buttons[ATTACK].function_pointer = Attack;

	pos = sf::Vector2f(box.Position().x + 375, box.Position().y + 175);
	buttons[RUN] = Button(pos, size, "Fugi", sf::Color(100, 100, 100));
	buttons[RUN].function_pointer = Run;

	hp_bars[NPC_] = HP_Bar(NPC_);
	hp_bars[CHAR_] = HP_Bar(CHAR_);

	log.Init();
}

void CombatScreen::Enter_combat(int npc_id){
	this->npc_id = npc_id;
	//for now, the main character will attack first
	attacker = game->Get_mainchar();
	defender = game->Get_char(npc_id);

	hp_thread[NPC_] = new std::thread(HP_fill_npc,npc_id);
	hp_thread[CHAR_] = new std::thread(HP_fill_mainchar);

	pics[MAINCHAR_PIC].Set_texture(game->Get_mainchar()->Diag_texture());
	pics[NPC_PIC].Set_texture(game->Get_char(npc_id)->Diag_texture());

	round = 0;

	log.Erase();
}

HP_Bar::HP_Bar(int type){
	bounding_box.setPrimitiveType(sf::Quads);
	bounding_box.resize(4);

	sf::Vertex* quad = &bounding_box[0];
	for (int i = 0; i < 4; i++) {
		quad[i].color = sf::Color(100, 100, 100);
	}

	hp_box.setPrimitiveType(sf::Quads);
	hp_box.resize(4);

	quad = &hp_box[0];
	for (int i = 0; i < 4; i++) {
		quad[i].color = sf::Color::Red;
	}

	if (type == CHAR_) {
		quad = &bounding_box[0];
		quad[0].position = sf::Vector2f(BOX_X + 125, BOX_Y + 50 + 10);
		quad[1].position = sf::Vector2f(BOX_X + 125+HP_W, BOX_Y + 50 + 10);
		quad[2].position = sf::Vector2f(BOX_X + 125+HP_W, BOX_Y + 50 + 10+HP_H);
		quad[3].position = sf::Vector2f(BOX_X + 125, BOX_Y + 50+ 10+HP_H);

		quad = &hp_box[0];
		quad[0].position = sf::Vector2f(BOX_X + 125, BOX_Y + 50 + 10);
		quad[1].position = sf::Vector2f(BOX_X + 125, BOX_Y + 50 + 10);
		quad[2].position = sf::Vector2f(BOX_X + 125, BOX_Y + 50 + 10 + HP_H);
		quad[3].position = sf::Vector2f(BOX_X + 125, BOX_Y + 50 + 10 + HP_H);
	}
	else if (type == NPC_) {
		quad = &bounding_box[0];
		quad[0].position = sf::Vector2f(BOX_X + 375, BOX_Y + 50 + 10);
		quad[1].position = sf::Vector2f(BOX_X + 375 + HP_W, BOX_Y + 50 + 10);
		quad[2].position = sf::Vector2f(BOX_X + 375 + HP_W, BOX_Y + 50 + 10 + HP_H);
		quad[3].position = sf::Vector2f(BOX_X + 375, BOX_Y + 50 + 10 + HP_H);

		quad = &hp_box[0];
		quad[0].position = sf::Vector2f(BOX_X + 375, BOX_Y + 50 + 10);
		quad[1].position = sf::Vector2f(BOX_X + 375, BOX_Y + 50 + 10);
		quad[2].position = sf::Vector2f(BOX_X + 375, BOX_Y + 50 + 10 + HP_H);
		quad[3].position = sf::Vector2f(BOX_X + 375, BOX_Y + 50 + 10 + HP_H);
	}
	text.setFont(*game->Get_font());
	text.setStyle(sf::Text::Regular);
	text.setCharacterSize(20);
	text.setString("");
	text.setPosition(Position().x + (Size().x - text.getLocalBounds().width) / 2, Position().y + (Size().y - text.getLocalBounds().height) / 2 - text.getLocalBounds().top);
}

void HP_Bar::Draw(sf::RenderTarget& target,int type){
	if (type == NPC_) text.setString(*game->Get_string_from_db(NPC_HP));
	else if (type == CHAR_) text.setString(*game->Get_string_from_db(MAINCHAR_HP));
	text.setPosition(Position().x + (Size().x - text.getLocalBounds().width) / 2, Position().y + (Size().y - text.getLocalBounds().height) / 2 - text.getLocalBounds().top);
	target.draw(bounding_box);
	target.draw(hp_box);
	target.draw(text);
}

void HP_Bar::Update_prog(float percent){
	sf::Vertex* quad = &hp_box[0];
	quad[1].position = sf::Vector2f(Position().x + (Size().x * percent) / 100, Position().y);
	quad[2].position = sf::Vector2f(Position().x + (Size().x * percent) / 100, Position().y +Size().y);
}

void Combat_log::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	for (std::list<Combat_log_entry*>::const_iterator i = entries.begin(); i != entries.end() && ((*i)->Order() - offset) < MAX_ENTRIES_VIEW; i++) {
		if ((*i)->Order() >= offset) {
			(*i)->Draw(target, offset);
		}
	}
}

void Combat_log::Add(std::string text){
	Combat_log_entry* entry = new Combat_log_entry(std::to_string(game->Get_combatscreen()->Round())+". "+text,Size());
	entries.push_back(entry);
}

void Combat_log::Init(){
	sf::Vertex* quad = &bounding_box[0];

	quad[0].position = sf::Vector2f(BOX_X + game->Get_combatscreen()->Radius(), BOX_Y + 325);
	quad[1].position = sf::Vector2f(BOX_X + BOX_W - game->Get_combatscreen()->Radius(), BOX_Y + 325);
	quad[2].position = sf::Vector2f(BOX_X + BOX_W - game->Get_combatscreen()->Radius(), BOX_Y + 325 + LOG_H);
	quad[3].position = sf::Vector2f(BOX_X + game->Get_combatscreen()->Radius(), BOX_Y + 325 + LOG_H);
}

bool Combat_log::MouseWithinBounds(sf::Vector2i pos)
{
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

Combat_log::Combat_log(){
	bounding_box.resize(4);
}

void Combat_log_entry::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(bounding_box);
	target.draw(text);
}

void Combat_log_entry::Draw(sf::RenderTarget& target, int offset) {
	sf::Vector2f pos;
	sf::Vertex* quad = &bounding_box[0];

	quad[0].position = sf::Vector2f(BOX_X + game->Get_combatscreen()->Radius(), BOX_Y + 325 + YOFFSET + (order - offset) * LOGENTRY_H);
	quad[1].position = sf::Vector2f(BOX_X + BOX_W - game->Get_combatscreen()->Radius(), BOX_Y + 325 + YOFFSET + (order - offset) * LOGENTRY_H);
	quad[2].position = sf::Vector2f(BOX_X + BOX_W - game->Get_combatscreen()->Radius(), BOX_Y + 325 + YOFFSET + (order - offset + 1) * LOGENTRY_H);
	quad[3].position = sf::Vector2f(BOX_X + game->Get_combatscreen()->Radius(), BOX_Y + 325 + YOFFSET + (order - offset + 1) * LOGENTRY_H);
	quad[4].position = sf::Vector2f(BOX_X + game->Get_combatscreen()->Radius(), BOX_Y + 325 + YOFFSET + (order - offset) * LOGENTRY_H);

	pos.x = quad[0].position.x + (quad[1].position.x-quad[0].position.x)/2-text.getLocalBounds().width/2;
	pos.y = quad[0].position.y + (quad[2].position.y - quad[1].position.y)/ 2 - text.getLocalBounds().height / 2-text.getLocalBounds().top;

	text.setPosition(pos);

	target.draw(*this);
}

bool Combat_log_entry::MouseWithinBounds(sf::Vector2i pos)
{
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

Combat_log_entry::Combat_log_entry(std::string text, int order){
	this->order = order;

	this->text.setString(text);
	this->text.setStyle(sf::Text::Regular);
	this->text.setFillColor(sf::Color::White);
	this->text.setCharacterSize(15);
	this->text.setFont(*game->Get_font());
	
	bounding_box.setPrimitiveType(sf::LineStrip);
	bounding_box.resize(5);

	sf::Vertex* quad = &bounding_box[0];

	for (int i = 0; i < 5; i++) {
		quad[i].color = sf::Color(0, 0, 0);
	}
}
