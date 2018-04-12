#include "Transform2d.h"

bool vs::transform_2d::collision_matrix[4][4] = {
	false,	false,	false,	false,
	false,	false,	true,	false,
	false,	true,	false,	true,
	false,	false,	true,	false,
};

/// <summary>
/// Constructor
/// </summary>
/// <param name="x">Position on the x axis</param>
/// <param name="y">Position on the y axis</param>
/// <param name="width">Width of the object</param>
/// <param name="height">Height of the object</param>
vs::transform_2d::transform_2d(const float x, const float y, const float width, const float height):
	_x			(x),
	_y			(y),
	_width		(width),
	_height		(height) {
	
}

/// <summary>
/// Destructor
/// </summary>
vs::transform_2d::~transform_2d() = default;

/// <summary>
/// Returns the x position
/// </summary>
/// <returns></returns>
float vs::transform_2d::get_x() const { return _x; }

/// <summary>
/// Returns the y position
/// </summary>
/// <returns></returns>
float vs::transform_2d::get_y() const { return _y; }

/// <summary>
/// Returns the width
/// </summary>
/// <returns></returns>
float vs::transform_2d::get_width() const { return _width; }

/// <summary>
/// Returns the height
/// </summary>
/// <returns></returns>
float vs::transform_2d::get_height() const { return _height; }

/// <summary>
/// Returns the axis-aligned bounding box of this object
/// </summary>
/// <returns>Rect</returns>
D2D1_RECT_F vs::transform_2d::get_aabb() const {
	const auto rect = D2D1::RectF(_x, _y - _height, _x + _width, _y);
	return rect;
}

/// <summary>
/// Sets the size of this object
/// </summary>
/// <param name="width">Width of the object</param>
/// <param name="height">Height of the object</param>
void vs::transform_2d::set_size(const float width, const float height) {
	_width = width;
	_height = height;
}

/// <summary>
/// Sets the position of this object
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void vs::transform_2d::set_position(const float x, const float y) {
	_x = x;
	_y = y;
}

/// <summary>
/// Returns true if the transforms are colliding, false if not
/// </summary>
/// <param name="other">Transform to check against</param>
/// <returns></returns>
bool vs::transform_2d::is_colliding(transform_2d* other) const {
	if (other == nullptr) return false;
	const auto other_aabb = other->get_aabb();
	const auto my_aabb = get_aabb();

	//Collision tests
	if (my_aabb.right < other_aabb.left || my_aabb.left > other_aabb.right) return false;
	if (my_aabb.top > other_aabb.bottom || my_aabb.bottom < other_aabb.top) return false;

	return true;
}

/// <summary>
/// Sets this object's layer
/// </summary>
/// <param name="layer">Layer to set</param>
void vs::transform_2d::set_layer(const E_LAYER layer) {
	_layer = layer;
}

/// <summary>
/// Returns this object's layer
/// </summary>
/// <returns>Layer of the object</returns>
vs::transform_2d::E_LAYER vs::transform_2d::get_layer() const {
	return _layer;
}
