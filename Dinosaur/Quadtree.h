#ifndef VS_QUADTREE_HPP
#define VS_QUADTREE_HPP

#include <vector>
#include "GameObject.h"

using namespace std;

namespace vs {
	class quadtree {
	private:
		float									_x;
		float									_y;
		float									_width;
		float									_height;
		int										_level;
		int										_max_level;
		vector<game_object*>					_objects;

		quadtree *								_parent;
		quadtree *								_nw;
		quadtree *								_ne;
		quadtree *								_sw;
		quadtree *								_se;

		bool									contains			(quadtree* child, game_object* object);
		bool									has_any_layer		(game_object* object, int layer) const;
		std::vector<game_object*>				get_objects_at_layer(int layer) const;

	public:
		quadtree													(float x, float y,
																	float width, float height,
																	int level, int maxLevel, quadtree* parent);

		~quadtree													();

		void									add_object			(game_object *object);
		vector<game_object*>					get_objects_at		(float x, float y, int layer = 0) const;
		vector<game_object*>					get_all_objects		() const;
		void									clear				();
		void									update				(const std::vector<game_object*>& objects);
		void									render				(ID2D1HwndRenderTarget* render_target, ID2D1SolidColorBrush* brush);
	};
}
#endif //VS_QUADTREE_HPP
