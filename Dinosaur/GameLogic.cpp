#include <algorithm>

#include "GameLogic.h"
#include "Game.h"
#include "Defines.h"
#include <sstream>
#include "Utils.h"
#include <iomanip>

using namespace vs;

/// <summary>
/// Constructor
/// </summary>
game_logic::game_logic(game* game):
	_player					(this),
	_quadtree				(0.0f, 0.0f, RESOLUTION_X, RESOLUTION_Y, 0, 2, nullptr), 
	_cactus_factory			(this),
	_game					(game),
	_time_since_spawn		(0.0f),
	_min_spawn_speed		(1.0f),
	_max_spawn_speed		(2.0f),
	_points					(0) { }

/// <summary>
/// Destructor
/// </summary>
game_logic::~game_logic() = default;

/// <summary>
/// Initializes the game
/// </summary>
void game_logic::initialize() {
	_player.initialize();
	_player.set_position(40, RESOLUTION_Y - 200);

	_quadtree.add_object(&_player);

	_cactus_factory.initialize();
}

/// <summary>
/// Renders all the game's visuals
/// </summary>
/// <param name="render_target">Target to render to</param>
/// <param name="text_format">Format for writing texts</param>
void game_logic::on_render(ID2D1HwndRenderTarget* render_target, IDWriteTextFormat* text_format) {
	//_quadtree.render(render_target, nullptr);
	//Render the player
	_player.on_render(render_target);

	//Render objects
	for (auto& obj : _objects) {
		obj->on_render(render_target); 
	}

	//Create brush
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
	

	//Create string to display
	std::stringstream ss;
	ss << "Score: ";
	ss << std::setw(4) << std::setfill('0') << static_cast<int>(_points);

	wstring widestr;
	for (auto i = 0; static_cast<int>(i) < static_cast<int>(ss.str().length()); ++i) {
		widestr += wchar_t(ss.str()[i]);
	}

	const auto str = widestr.c_str();

	//Render kills
	render_target->DrawText(
		str,
		ss.str().length(),
		text_format,
		D2D1::RectF(0, 0, 150, 50),
		brush
	);

	utils::safe_release(&brush);
}

/// <summary>
/// Updates all the game's states and logic
/// </summary>
/// <param name="delta">Time since last frame in seconds</param>
/// <returns>True if game has ended</returns>
bool game_logic::on_update(const double delta) {
	on_update_spawn(delta);

	_quadtree.update(_objects);
	_quadtree.add_object(&_player);

	_player.on_update(delta);

	_points += delta * 4;

	//Update enemies
	for (size_t i = 0; i < _objects.size(); i++) {
		_objects[i]->on_update(delta);
	}

	check_collisions();

	if(_player.is_dead()) {
		//Player is dead, game ended
		return true;
	}

	cleanup();
	return false;
}


/// <summary>
/// Checks for collisions and handles them
/// </summary>
void game_logic::check_collisions() {
	//Check if the object can collide at all
	if (_player.get_layer() != 0) {
		auto near_objects = _quadtree.get_objects_at(_player.get_x(), _player.get_y() - 1);
		for (auto& near_object : near_objects) {
			if (near_object != &_player && near_object->is_colliding(&_player)) {
				//Objects are colliding, so let them handle it
				_player.on_collision(near_object);
			}
		}
	}
}

/// <summary>
/// Cleans up dead objects
/// </summary>
/// <param name="end">True if game ended, false if not</param>
void game_logic::cleanup(bool end) {
	if(end) {
		for (auto& _object : _objects) {
			delete _object;
		}
		return;
	}
	//Delete dead objects
	_objects.erase(std::remove_if(_objects.begin(), _objects.end(), [this](game_object* obj) {
		if(obj->is_dead()) {
			delete obj;
			obj = nullptr;
			return true;
		};
		return false;
	}), _objects.end());
}

/// <summary>
/// Creates a new cactus of the specified type
/// </summary>
/// <param name="type">Type of cactus</param>
/// <param name="x">x Position</param>
/// <param name="y">y Position</param>
/// <returns></returns>
void game_logic::create_cactus(dino::cactus::E_CACTUS_TYPE type, float x, float y) {
	const auto enemy = _cactus_factory.make_cactus(type, x, y);
	_objects.push_back(enemy);
}

/// <summary>
/// Updates the spawning process
/// </summary>
/// <param name="delta">Time since last frame</param>
void game_logic::on_update_spawn(const float delta) {
	//Try to spawn new enemies
	if (_time_since_spawn > 0.0f) {
		_time_since_spawn -= delta;
	}

	//Spawn enemies
	if (_time_since_spawn <= 0.0f) {
		_time_since_spawn =  _min_spawn_speed + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_max_spawn_speed - _min_spawn_speed)));

		//Cacti spawn with different probabilities
		//60% Normal
		//20% wide
		//20% High
		dino::cactus::E_CACTUS_TYPE type = dino::cactus::E_CACTUS_TYPE::normal;
		const int percent = (rand() % 99) + 1;
		if (percent > 60 && percent <= 80) {
			type = dino::cactus::E_CACTUS_TYPE::wide;
		} else if(percent > 80) {
			type = dino::cactus::E_CACTUS_TYPE::high;
		}

		create_cactus(type, RESOLUTION_X, RESOLUTION_Y - 1);
	}
}

/// <summary>
/// Returns the game instance
/// </summary>
/// <returns>Game pointer</returns>
game* game_logic::get_game() const {
	return _game;
}
