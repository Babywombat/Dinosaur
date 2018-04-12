#ifndef VS_TRANSFORMTWOD_HPP
#define VS_TRANSFORMTWOD_HPP
#include <d2d1.h>

namespace vs {
	class transform_2d {
	protected:
		float			_x;
		float			_y;
		float			_width;
		float			_height;

	public:
		/// <summary>
		/// Collision matrix to determine which layers should collide
		/// </summary>
		static bool collision_matrix[4][4];

		enum E_LAYER {
			no_collisions = 0,
			player_bullet = 1,
			cactus = 2,
			player = 3
		};

		transform_2d							(float x, float y, float width, float height);
		virtual ~transform_2d();

		float			get_x					() const;
		float			get_y					() const;
		float			get_width				() const;
		float			get_height				() const;
		D2D1_RECT_F		get_aabb				() const;

		void			set_size				(float width, float height);

		void			set_position			(float x, float y);
		bool			is_colliding			(transform_2d* other) const;

		void			set_layer				(E_LAYER layer);
		E_LAYER			get_layer				() const;
	private:
		E_LAYER			_layer;
	};
}

#endif //VS_TRANSFORMTWOD_HPP