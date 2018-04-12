#include <iostream>
#include <sstream>

#include "Quadtree.h"
#include "Utils.h"

using namespace std;
using namespace vs;

/// <summary>
/// Constructor
/// </summary>
/// <param name="x">Position on the x axis</param>
/// <param name="y">Position on the y axis</param>
/// <param name="width">Width of the tree</param>
/// <param name="height">Height of the tree</param>
/// <param name="level">Level of the tree</param>
/// <param name="maxLevel">Maximum level of the tree</param>
quadtree::quadtree(float x, float y, float width, float height, int level, int maxLevel, quadtree* parent) :
	_x			(x),
	_y			(y),
	_width		(width),
	_height		(height),
	_level		(level),
	_max_level	(maxLevel),
	_parent		(parent)
{
	if (level == maxLevel) {
		return;
	}

	_nw = new quadtree(x, y, width / 2.0f, height / 2.0f, level+1, maxLevel, this);
	_ne = new quadtree(x + width / 2.0f, y, width / 2.0f, height / 2.0f, level+1, maxLevel, this);
	_sw = new quadtree(x, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, maxLevel, this);
	_se = new quadtree(x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, maxLevel, this);
}

/// <summary>
/// Destructor
/// </summary>
quadtree::~quadtree() {
	if (_level == _max_level)
		return;

	delete _nw;
	delete _ne;
	delete _sw;
	delete _se;
}

/// <summary>
/// Adds a new object to the tree
/// </summary>
/// <param name="object">Object to add</param>
void quadtree::add_object(game_object* object) {
	if (_level == _max_level) {
		_objects.push_back(object);
		return;
	}
	if (contains(_nw, object)) {
		_nw->add_object(object); return;
	} else if (contains(_ne, object)) {
		_ne->add_object(object); return;
	} else if (contains(_sw, object)) {
		_sw->add_object(object); return;
	} else if (contains(_se, object)) {
		_se->add_object(object); return;
	}
	if (contains(this, object)) {
		_objects.push_back(object);
	}
}

/// <summary>
/// Returns the objects in the quadtree that are inside the area with the given point
/// </summary>
/// <param name="x">Position on the x axis</param>
/// <param name="y">Position on the y axis</param>
/// <param name="layer">Accepted layers of the object</param>
/// <returns>List of collision objects</returns>
vector<game_object*> quadtree::get_objects_at(float x, float y, int layer) const {
	if (_level == _max_level) {
		if(layer == 0) {
			return _objects;
		}
		return get_objects_at_layer(layer);
	}

	vector<game_object*> return_objects, child_return_objects;
	if (!_objects.empty()) {
		if(layer == 0) {
			return_objects = _objects;
		} else {
			return_objects = get_objects_at_layer(layer);
		}
	}

	//Check each subtree and add results
	if (x > _x + _width / 2.0f && x <= _x + _width) {
		if (y > _y + _height / 2.0f && y <= _y + _height) {
			child_return_objects = _se->get_objects_at(x, y);
			return_objects.insert(return_objects.end(), child_return_objects.begin(), child_return_objects.end());
			return return_objects;
		}
		if (y > _y && y <= _y + _height / 2.0f) {
			child_return_objects = _ne->get_objects_at(x, y);
			return_objects.insert(return_objects.end(), child_return_objects.begin(), child_return_objects.end());
			return return_objects;
		}
	} else if (x > _x && x <= _x + _width / 2.0f) {
		if (y > _y + _height / 2.0f && y <= _y + _height) {
			child_return_objects = _sw->get_objects_at(x, y);
			return_objects.insert(return_objects.end(), child_return_objects.begin(), child_return_objects.end());
			return return_objects;
		}
		if (y > _y && y <= _y + _height / 2.0f) {
			child_return_objects = _nw->get_objects_at(x, y);
			return_objects.insert(return_objects.end(), child_return_objects.begin(), child_return_objects.end());
			return return_objects;
		}
	}
	return return_objects;
}

/// <summary>
/// Returns all the objects in the tree
/// </summary>
/// <returns></returns>
vector<game_object*> quadtree::get_all_objects() const {
	if (_level == _max_level) {
		return _objects;
	}

	vector<game_object*> return_objects;
	if (!_objects.empty()) {
		return_objects = _objects;
	}

	auto child_return_objects = _se->get_all_objects();
	return_objects.insert(return_objects.end(), child_return_objects.begin(), child_return_objects.end());
	
	child_return_objects = _ne->get_all_objects();
	return_objects.insert(return_objects.end(), child_return_objects.begin(), child_return_objects.end());
	
	child_return_objects = _sw->get_all_objects();
	return_objects.insert(return_objects.end(), child_return_objects.begin(), child_return_objects.end());

	child_return_objects = _nw->get_all_objects();
	return_objects.insert(return_objects.end(), child_return_objects.begin(), child_return_objects.end());

	return return_objects;
}

/// <summary>
/// Clears the tree
/// </summary>
void quadtree::clear() {
	if (_level == _max_level) {
		_objects.clear();
		return;
	}
	_nw->clear();
	_ne->clear();
	_sw->clear();
	_se->clear();

	if (!_objects.empty()) {
		_objects.clear();
	}
}

/// <summary>
/// Updates the quadtree, so that moved objects are sorted correctly
/// </summary>
void quadtree::update(const std::vector<game_object*>& objects) {
	//Only allow calling on the root
	if (_parent != nullptr) return;
	
	//Rebuild the tree
	clear();
	for (auto* object : objects) {
		add_object(object);
	}
}

/// <summary>
/// Renders the quadtree
/// </summary>
/// <param name="render_target">Target to render to</param>
void quadtree::render(ID2D1HwndRenderTarget* render_target, ID2D1SolidColorBrush* brush) {
	if (render_target == nullptr) return;
	if(brush == nullptr) {
		render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AntiqueWhite), &brush);
	}
	if(_level == _max_level) {
		render_target->DrawRectangle(D2D1::RectF(_x, _y, _x + _width, _y + _height), brush);
		return;
	}
	_se->render(render_target, brush);
	_ne->render(render_target, brush);
	_nw->render(render_target, brush);
	_sw->render(render_target, brush);

	if (_parent == nullptr) {
		utils::safe_release(&brush);
	}
}

/// <summary>
/// Returns true if the tree contains the given object, false if not
/// </summary>
/// <param name="child">Tree to check</param>
/// <param name="object">Object to check</param>
/// <returns></returns>
bool quadtree::contains(quadtree *child, game_object *object) {
	if (child == nullptr || object == nullptr) return false;
	//Classic aabb collision check
	return	 !(object->get_x() < child->_x ||
				object->get_y() < child->_y ||
				object->get_x() > child->_x + child->_width  ||
				object->get_y() > child->_y + child->_height ||
				object->get_x() + object->get_width() < child->_x ||
				object->get_y() - object->get_height() < child->_y ||
				object->get_x() + object->get_width() > child->_x + child->_width ||
				object->get_y() - object->get_height() > child->_y + child->_height);
}

/// <summary>
/// Returns true if the object has a layer contained in the given layer
/// </summary>
/// <param name="object">Object to test layer of</param>
/// <param name="layer">Layers to check</param>
/// <returns></returns>
bool quadtree::has_any_layer(game_object* object, int layer) const {
	if (object == nullptr) return false;
	const auto obj_layer = object->get_layer();

	//Test if bit is set
	return ((obj_layer & layer) == obj_layer);
}

/// <summary>
/// Returns a vector of all the objects that match the given layers
/// </summary>
/// <param name="layer">Layers to check for</param>
/// <returns></returns>
std::vector<game_object*> quadtree::get_objects_at_layer(int layer) const {
	vector<game_object*> return_objects;
	for (auto object : _objects) {
		if (has_any_layer(object, layer)) {
			return_objects.push_back(object);
		}
	}
	return return_objects;
}
