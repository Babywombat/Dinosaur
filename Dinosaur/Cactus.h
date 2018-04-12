#ifndef DINO_CACTUS_HPP
#define DINO_CACTUS_HPP

#include "GameObject.h"

namespace dino {
	class cactus : public vs::game_object {
	public:
		enum E_CACTUS_TYPE {
			normal = 0,
			wide = 1,
			high = 2
		};

		cactus();
		~cactus();

		void										on_update(double delta_time) override;
		void										on_render(ID2D1HwndRenderTarget* render_target) override;
		void										initialize() override;
		void										handle_collision(transform_2d* collided_object) override;

		virtual E_CACTUS_TYPE						get_type() const;
		void										set_type(E_CACTUS_TYPE type);
	private:
		E_CACTUS_TYPE								_type;
	};
}

#endif //DINO_CACTUS_HPP

