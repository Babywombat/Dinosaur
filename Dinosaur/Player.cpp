#include "Player.h"
#include "Input.h"
#include "GameLogic.h"
#include "Utils.h"
#include "Defines.h"

using namespace vs;

/// <summary>
/// Constructor
/// </summary>
player::player(game_logic* logic) : 
	game_object			(0, 0, 30.0f, 50.0f),
	_logic				(logic),
	_y_velocity			(0) {}

/// <summary>
/// Destructor
/// </summary>
player::~player() = default;

/// <summary>
/// Updates the player's position
/// </summary>
/// <param name="delta_time">Time since last update</param>
void player::on_update(double delta_time) {
	if (is_dead()) return;

	if (input::get_instance().is_key_down(input::Space) && _y >= RESOLUTION_Y) {
		//Jump
		_y_velocity = -5;
		_is_jumping = true;
	}

	_y += _y_velocity;
	if(_y >= RESOLUTION_Y && !_is_jumping) {
		_y = RESOLUTION_Y;
		if(_y_velocity < 0) {
			_y_velocity = 0.0f;
		}
	} else {
		_y_velocity -= GRAVITY * delta_time;
	}
	_is_jumping = false;

	
}

/// <summary>
/// Renders the player
/// </summary>
/// <param name="render_target">Target to render to</param>
void player::on_render(ID2D1HwndRenderTarget* render_target) {
	if (!render_target) return;

	//Drawing the player's visuals
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(_color, &brush);


	//Draw the lines
	render_target->FillRectangle(get_aabb(), brush);
	//render_target->DrawRectangle(get_aabb(), brush);

	utils::safe_release(&brush);
}

/// <summary>
/// Initializes the player's variables
/// </summary>
void player::initialize() {
	_color = D2D1::ColorF(D2D1::ColorF::White, 1.0f);
	_speed = 200.0f;
	_health = 1;
	set_layer(E_LAYER::player);
}

/// <summary>
/// Handles the collisions with other objects
/// </summary>
/// <param name="collided_object">Object that this object collided with</param>
void player::handle_collision(transform_2d* collided_object) {
	inflict_damage(1);
}
