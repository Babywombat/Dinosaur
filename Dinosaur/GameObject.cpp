#include "GameObject.h"

using namespace vs;

/// <summary>
/// Constructor
/// </summary>
/// <param name="x">Position on the x axis</param>
/// <param name="y">Position on the y axis</param>
/// <param name="width">Width of the object</param>
/// <param name="height">Height of the object</param>
game_object::game_object(const float x, const float y, const float width, const float height) : 
	transform_2d	(x, y, width, height),
	_brush				(nullptr), 
	_color				(0.0f, 0.0f, 0.0f, 1.0f) {}

/// <summary>
/// Destructor
/// </summary>
game_object::~game_object() = default;

/// <summary>
/// Checks if the collision should be handled and calls the appropriate functions
/// </summary>
/// <param name="collided_object">Object that this object collided with</param>
void game_object::on_collision(transform_2d* collided_object) {
	if (!collided_object) return;
	if(collision_matrix[get_layer()][collided_object->get_layer()]) {
		handle_collision(collided_object);
	}
}

/// <summary>
/// Sets the game_object's speed
/// </summary>
/// <param name="speed">Speed to use</param>
void game_object::set_speed(float speed) {
	_speed = speed;
}

/// <summary>
/// Inflicts damage on the game_object and returns if it is dead
/// </summary>
/// <param name="damage">Damage to inflict</param>
/// <returns>True if game_object died, false if not</returns>
bool game_object::inflict_damage(int damage) {
	_health -= damage;
	return is_dead();
}

/// <summary>
/// Returns if the game_object is dead
/// </summary>
/// <returns>True if game_object is dead, false if not</returns>
bool game_object::is_dead() const {
	return _health <= 0;
}

/// <summary>
/// Returns the game_object's health
/// </summary>
/// <returns></returns>
int game_object::get_health() const {
	return _health;
}

/// <summary>
/// Sets the color of this gameobject
/// </summary>
/// <param name="color">Color to set</param>
void game_object::set_color(D2D1::ColorF color) {
	_color = color;
}


