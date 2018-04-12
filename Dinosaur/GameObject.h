#ifndef VS_GAME_OBJECT_HPP
#define VS_GAME_OBJECT_HPP

#include <d2d1.h>
#include "Transform2d.h"

namespace vs {
	class quadtree;

	class game_object : public transform_2d {
	protected:
		ID2D1SolidColorBrush *				_brush;
		D2D1::ColorF						_color;
		float								_speed;
		int									_health;

		virtual void						handle_collision			(transform_2d* collided_object) = 0;

	public:
		game_object														(float x, float y, float width, float height);
		virtual ~game_object											();

		virtual void						on_update					(double delta_time) = 0;
		virtual void						on_render					(ID2D1HwndRenderTarget* render_target) = 0;
		virtual void						initialize					() = 0;
		
		void								on_collision				(transform_2d* collided_object);
		void								set_speed					(float speed);
		bool								inflict_damage				(int damage);
		bool								is_dead						() const;
		int									get_health					() const;

		void								set_color					(D2D1::ColorF color);
	};
}

#endif //VS_GAME_OBJECT_HPP