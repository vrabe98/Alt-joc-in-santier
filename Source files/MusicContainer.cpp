#include "MusicContainer.h"
#include "Game.h"
#include <Windows.h>

#define FWD 1
#define BACK -1

extern Game* game;

void Update_play_time() {
	while (1) {
		if (*game->Get_string_from_db(CURRENT_SONG) != "") {
			std::string timestr="";
			std::string seconds_str;
			float percent;
			sf::Music* music = game->Get_playlist()->Get_music();
			sf::Time current_time = music->getPlayingOffset(), duration = music->getDuration();
			seconds_str = std::to_string((int)floor(current_time.asSeconds()) % 60);
			if (seconds_str.length() == 1) {
				seconds_str = "0" + seconds_str;
			}
			timestr += std::to_string((int)floor(current_time.asSeconds() / 60)) + ":"+seconds_str;
			timestr += " / ";
			seconds_str = std::to_string((int)floor(duration.asSeconds()) % 60);
			if (seconds_str.length() == 1) {
				seconds_str = "0" + seconds_str;
			}
			timestr += std::to_string((int)floor(duration.asSeconds() / 60)) + ":" + seconds_str;
			game->Update_string(PLAY_TIME, &timestr);
			percent = current_time.asSeconds() / duration.asSeconds() * 100;
			game->Get_playlist()->Get_progbar()->Update(percent);
			if (!music->getLoop() && music->getStatus() == sf::Music::Stopped) {
				game->Get_playlist()->Change_song(FWD);
			}
			game->Get_playlist()->Get_volbar()->Update(music->getVolume());
		}
		Sleep(100);
	}
}

void Song::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(bounding_box);
	target.draw(name_text);
	target.draw(order_text);
}

bool Song::MouseWithinBounds(sf::Vector2i pos){
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

void Song::Change_state(short int action){
	int modif;
	sf::Color color = bounding_box[0].color;
	highlighted = !highlighted;
	highlighted == false ? modif = -1 : modif = 1;
	color.r += modif * LIGHTEN_FACTOR;
	color.g += modif * LIGHTEN_FACTOR;
	color.b += modif * LIGHTEN_FACTOR;
	for (int i = 0; i < 4; i++) {
		bounding_box[i].color = color;
	}
	if (highlighted) {
		game->Update_string(ENCYCLO_DESCR, &description);
		game->Get_playlist()->Change_song(name,order);
	}
	else if (action == OUTOFBOX) game->Update_string(ENCYCLO_DESCR, &std::string(""));
}

void Song::Init() {
	order_text.setFont(*game->Get_font());
	name_text.setFont(*game->Get_font());
	order_text.setString(std::to_string(order) + ". ");
	name_text.setString(name.substr(0,name.length()-4));
	name_text.setStyle(sf::Text::Regular);
	name_text.setFillColor(sf::Color::White);
	order_text.setStyle(sf::Text::Regular);
	order_text.setFillColor(sf::Color::White);
}

void Song::Draw(sf::RenderTarget& target, short int offset){
	double coeff = 141.0 / 72.0, font_size;
	sf::Vertex* quad = &bounding_box[0];

	name_text.setString(name.substr(0, name.length() - 4));
	font_size = SONGNAMW * coeff / (((std::string)name_text.getString()).length() + 3.0f);
	if (font_size >= SONGNAMH) font_size = SONGNAMH;
	name_text.setCharacterSize(font_size);
	name_text.setPosition(XPADM + SONGORDW + (SONGNAMW - name_text.getLocalBounds().width) / 2, YPADM + (order - offset) * SONGNAMH + (SONGNAMH - name_text.getLocalBounds().height) / 2 - name_text.getLocalBounds().top);

	order_text.setString(std::to_string(order) + ". ");
	font_size = SONGORDW * coeff / (((std::string)order_text.getString()).length() + 3.0f);
	if (font_size >= SONGORDH) font_size = SONGORDH;
	order_text.setCharacterSize(font_size);
	order_text.setPosition(XPADM + (SONGORDW - order_text.getLocalBounds().width) / 2, YPADM + (order - offset) * SONGORDH + (SONGORDH - order_text.getLocalBounds().height) / 2 - order_text.getLocalBounds().top);

	quad[0].position = sf::Vector2f(XPADM, YPADM + (order - offset) * SONGNAMH);
	quad[1].position = sf::Vector2f(XPADM + SONGORDW + SONGNAMW, YPADM+ (order - offset) * SONGNAMH);
	quad[2].position = sf::Vector2f(XPADM + SONGORDW + SONGNAMW, YPADM + (order - offset + 1) * SONGNAMH);
	quad[3].position = sf::Vector2f(XPADM, YPADM + (order - offset + 1) * SONGNAMH);

	target.draw(*this);
}

void MusicContainer::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	for (std::list<Song*>::const_iterator i = songs.begin(); i != songs.end() && ((*i)->Order() - offset) < MAX_SONGS_VIEW; i++) {
		if ((*i)->Order() >= offset) {
			(*i)->Draw(target, offset);
		}
	}
	target.draw(prog_bar);
	target.draw(vol_bar);
}

void MusicContainer::Begin_timecalc(){
	play_time_calc = new std::thread(Update_play_time);
}

bool MusicContainer::MouseWithinBounds(sf::Vector2i pos){
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

void MusicContainer::Add_song(std::string name){
	Song* new_song = new Song(name, songs.size());
	new_song->Init();
	songs.push_back(new_song);
}

void MusicContainer::LMB_Pressed(sf::Vector2i pos){
	std::list<Song*>::iterator i = songs.begin();
	for (; i != songs.end(); i++) {
		if (!(*i)->MouseWithinBounds(pos) && (*i)->Is_highlighted()) {
			(*i)->Change_state(SELECTDESELECT);
		}
	}
	i = songs.begin();
	std::advance(i, offset);
	for (; i != songs.end(); i++) {
		if ((*i)->MouseWithinBounds(pos)) {
			(*i)->Change_state(SELECTDESELECT);
		}
	}
	if (prog_bar.MouseWithinBounds(pos)) {
		music.setPlayingOffset(music.getDuration() * prog_bar.Percent_click(pos) / (float)100);
	}
	else if (vol_bar.MouseWithinBounds(pos)) {
		float percent = vol_bar.Percent_click(pos);
		music.setVolume(percent);
	}
}

void MusicContainer::Change_song(std::string name,int order) {
	game->Update_string(CURRENT_SONG, &name.substr(0, name.length() - 4));
	name = "Music/" + name;
	current_song_order = order;
	music.stop();
	if (!music.openFromFile(name))
		exit(-20);
	music.play();
}

void MusicContainer::Change_song(int direction){
	std::list<Song*>::iterator i = songs.begin();
	int oldorder = current_song_order;
	current_song_order += direction;
	if (current_song_order < 0) current_song_order = songs.size() - 1;
	else if (current_song_order == songs.size()) current_song_order = 0;
	std::advance(i, current_song_order);
	(*i)->Change_state(SELECTDESELECT);
}

MusicContainer::MusicContainer() {
	bounding_box.setPrimitiveType(sf::Quads);
	bounding_box.resize(4);
	sf::Vertex* quad = &bounding_box[0];
	quad[0].position = sf::Vector2f(XPADM, YPADM);
	quad[1].position = sf::Vector2f(XPADM + MUSICW, XPADM);
	quad[2].position = sf::Vector2f(XPADM + MUSICW, YPADM + MUSICH);
	quad[3].position = sf::Vector2f(XPADM, YPADM + MUSICH);
	music.setLoop(false);
	current_song_order = 0;
	offset = 0;
}

void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(bounding_box);
	target.draw(progress_box);
}

bool ProgressBar::MouseWithinBounds(sf::Vector2i pos){
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

ProgressBar::ProgressBar() {
	bounding_box.setPrimitiveType(sf::Quads);
	bounding_box.resize(4);

	sf::Vertex* quad = &bounding_box[0];
	for (int i = 0; i < 4; i++) {
		quad[i].color = sf::Color(50, 50, 50);
	}
	quad[0].position = sf::Vector2f(PROGX, PROGY);
	quad[1].position = sf::Vector2f(PROGX + PROGW, PROGY);
	quad[2].position = sf::Vector2f(PROGX + PROGW, PROGY + PROGH);
	quad[3].position = sf::Vector2f(PROGX, PROGY + PROGH);

	progress_box.setPrimitiveType(sf::Quads);
	progress_box.resize(4);

	quad = &progress_box[0];
	for (int i = 0; i < 4; i++) {
		quad[i].color = sf::Color(255, 255, 255);
	}
	quad[0].position = sf::Vector2f(PROGX, PROGY);
	quad[1].position = sf::Vector2f(PROGX, PROGY);
	quad[2].position = sf::Vector2f(PROGX, PROGY + PROGH);
	quad[3].position = sf::Vector2f(PROGX, PROGY + PROGH);

}

void VolumeBar::draw(sf::RenderTarget& target, sf::RenderStates state) const{
	target.draw(bounding_box);
	target.draw(volume_box);
}

bool VolumeBar::MouseWithinBounds(sf::Vector2i pos)
{
	return (bounding_box[0].position.x <= (float)pos.x) && ((float)pos.x <= bounding_box[1].position.x) && (bounding_box[0].position.y <= (float)pos.y) && ((float)pos.y <= bounding_box[3].position.y);
}

VolumeBar::VolumeBar(){
	bounding_box.setPrimitiveType(sf::Quads);
	bounding_box.resize(4);

	sf::Vertex* quad = &bounding_box[0];
	for (int i = 0; i < 4; i++) {
		quad[i].color = sf::Color(50, 50, 50,200);
	}
	quad[0].position = sf::Vector2f(VOLX, VOLY);
	quad[1].position = sf::Vector2f(VOLX + VOLW, VOLY);
	quad[2].position = sf::Vector2f(VOLX + VOLW, VOLY + VOLH);
	quad[3].position = sf::Vector2f(VOLX, VOLY + VOLH);

	volume_box.setPrimitiveType(sf::Quads);
	volume_box.resize(4);

	quad = &volume_box[0];
	for (int i = 0; i < 4; i++) {
		quad[i].color = sf::Color(255, 255, 255,200);
	}
	quad[0].position = sf::Vector2f(VOLX, VOLY);
	quad[1].position = sf::Vector2f(VOLX , VOLY);
	quad[2].position = sf::Vector2f(VOLX, VOLY + VOLH);
	quad[3].position = sf::Vector2f(VOLX, VOLY + VOLH);
}
