#ifndef VS_PLAYER_HPP
#define VS_PLAYER_HPP

#include "GameObject.h"

namespace vs {
	class game_logic;

	class player : public game_object {
	private:
		game_logic*		_logic;
		float			_y_velocity;
		bool			_is_jumping;
	public:
		explicit player						(game_logic* logic);
		~player								();

		void			on_update			(double delta_time) override;
		void			on_render			(ID2D1HwndRenderTarget* render_target) override;
		void			initialize			() override;
		void			handle_collision	(transform_2d* collided_object) override;
	};
}

#endif //VS_PLAYER_HPP