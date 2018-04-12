#ifndef VS_GAME_LOGIC_HPP
#define VS_GAME_LOGIC_HPP

#include <d2d1.h>
#include "Player.h"
#include "Quadtree.h"
#include "CactusFactory.h"

namespace vs {
	class game;

	class game_logic {
	private:
		player						_player;
		quadtree					_quadtree;
		std::vector<game_object*>	_objects;

		dino::cactus_factory		_cactus_factory;

		game*						_game;

		float						_time_since_spawn;
		float						_min_spawn_speed;
		float						_max_spawn_speed;

		float						_points;

		void						check_collisions				();
		void						cleanup							(bool end = false);
		void						create_cactus					(dino::cactus::E_CACTUS_TYPE type, float x, float y);
		void						on_update_spawn					(const float delta);
	public:
		game_logic													(game* game);
		~game_logic													();

		void						initialize						();

		void						on_render						(ID2D1HwndRenderTarget* render_target,
																	IDWriteTextFormat* text_format);
		bool						on_update						(double delta);

		game*						get_game						() const;

	};
}

#endif //VS_GAME_LOGIC_HPP