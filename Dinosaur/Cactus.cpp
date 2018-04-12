#include "Cactus.h"
#include "Utils.h"

using namespace dino;
using namespace vs;

/// <summary>
/// Constructor
/// </summary>
cactus::cactus() :
	game_object(0, 0, 25, 25) {

}

/// <summary>
/// Destructor
/// </summary>
cactus::~cactus() = default;

/// <summary>
/// Updates the cactus
/// </summary>
/// <param name="delta_time">Time since last frame</param>
void cactus::on_update(double delta_time) {
	if (is_dead()) return;
	const auto current_change = _speed * static_cast<float>(delta_time);

	_x -= current_change;

	if (_x + _width <= 0) inflict_damage(1);
}

/// <summary>
/// Renders the cactus' visuals
/// </summary>
/// <param name="render_target">Target to render to</param>
void cactus::on_render(ID2D1HwndRenderTarget* render_target) {
	if (!render_target) return;

	//Drawing the enemies' visuals
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(_color, &brush);

	//Get rect for visuals
	const auto rect = get_aabb();

	//Draw the lines
	render_target->FillRectangle(rect, brush);
	//render_target->DrawRectangle(get_aabb(), brush);

	utils::safe_release(&brush);
}

/// <summary>
/// Initializes the cactus
/// </summary>
void cactus::initialize() {
	_color = D2D1::ColorF(D2D1::ColorF::Green, 1.0f);
	_speed = 250.0f;
	_health = 1;
	set_layer(E_LAYER::cactus);
}

/// <summary>
/// Handles the collisions with other objects
/// </summary>
/// <param name="collided_object">Object that this object collided with</param>
void cactus::handle_collision(transform_2d* collided_object) {
	//Ignore collisions
}

/// <summary>
/// Returns the cactus' type
/// </summary>
/// <returns>Enemy type</returns>
cactus::E_CACTUS_TYPE cactus::get_type() const {
	return _type;
}

/// <summary>
/// Sets the cactus' type
/// </summary>
/// <param name="type"></param>
void cactus::set_type(E_CACTUS_TYPE type) {
	_type = type;
	switch (_type) {
		case normal: {
			_width = 25;
			_height = 35;
			break;
		}
		case wide: {
			_width = 50;
			_height = 35;
			break;
		}
		case high: {
			_height = 50;
			_width = 25;
			break;
		}
	}
}
