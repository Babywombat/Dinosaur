#ifndef VS_ENEMY_FACTORY_HPP
#define VS_ENEMY_FACTORY_HPP

#include "Cactus.h"

namespace vs {
	class game_logic;
}

namespace dino {


	class cactus_factory {
	private:
		ID2D1PathGeometry *				_triangle_geometry;
		ID2D1PathGeometry *				_hexagon_geometry;
		vs::game_logic*					_game_logic;
	public:

		cactus_factory										(vs::game_logic* game);
		~cactus_factory();

		cactus*							make_cactus			(cactus::E_CACTUS_TYPE type, float x, float y) const;
		void							initialize			();
	};
}

#endif //VS_ENEMY_FACTORY_HPP