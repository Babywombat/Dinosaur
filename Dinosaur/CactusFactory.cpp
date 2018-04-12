#include "CactusFactory.h"
#include "Game.h"
#include "Utils.h"

using namespace vs;
using namespace dino;

/// <summary>
/// Constructor
/// </summary>
/// <param name="game">The game logic instance</param>
cactus_factory::cactus_factory(game_logic* game) :
	_triangle_geometry		(nullptr),
	_game_logic				(game){
}

/// <summary>
/// Destructor
/// </summary>
cactus_factory::~cactus_factory() = default;

/// <summary>
/// Initializes the factory
/// </summary>
void cactus_factory::initialize() {
	if (!_game_logic) return;
	const auto factory = _game_logic->get_game()->get_direct2d_factory();

	if (!factory) return;
	
	ID2D1GeometrySink* sink;

	//Create the triangle geometry
	auto hr = factory->CreatePathGeometry(&_triangle_geometry);
	if (SUCCEEDED(hr)) {
		//Write to the path geometry using the geometry sink.
		hr = _triangle_geometry->Open(&sink);

		if (SUCCEEDED(hr)) {

			sink->BeginFigure(
				D2D1::Point2F(0, 0),
				D2D1_FIGURE_BEGIN_FILLED
			);

			//Add triangle lines
			sink->AddLine(D2D1::Point2F(20, 0));
			sink->AddLine(D2D1::Point2F(10, 20));
			sink->AddLine(D2D1::Point2F(0, 0));

			sink->EndFigure(D2D1_FIGURE_END_CLOSED);

			sink->Close();
		}
		utils::safe_release(&sink);
	}

	//Create the hexagon geometry
	hr = factory->CreatePathGeometry(&_hexagon_geometry);
	if (SUCCEEDED(hr)) {
		//Write to the path geometry using the geometry sink.
		hr = _hexagon_geometry->Open(&sink);

		if (SUCCEEDED(hr)) {

			sink->BeginFigure(
				D2D1::Point2F(8, 0),
				D2D1_FIGURE_BEGIN_FILLED
			);

			//Add hexagon lines
			sink->AddLine(D2D1::Point2F(16, 0));
			sink->AddLine(D2D1::Point2F(24, 8));
			sink->AddLine(D2D1::Point2F(24, 16));
			sink->AddLine(D2D1::Point2F(16, 24));
			sink->AddLine(D2D1::Point2F(8, 24));
			sink->AddLine(D2D1::Point2F(0, 16));
			sink->AddLine(D2D1::Point2F(0, 8));
			sink->AddLine(D2D1::Point2F(8, 0));

			sink->EndFigure(D2D1_FIGURE_END_CLOSED);

			sink->Close();
		}
		utils::safe_release(&sink);
	}
	
}

/// <summary>
/// Creates a new cactus of the specified type
/// </summary>
/// <param name="type">Type of cactus</param>
/// <param name="x">x Position</param>
/// <param name="y">y Position</param>
/// <returns></returns>
cactus* cactus_factory::make_cactus(const cactus::E_CACTUS_TYPE type, const float x, const float y) const {
	cactus* new_cactus = new cactus();
	if (new_cactus) {
		new_cactus->initialize();
		new_cactus->set_position(x, y);
		new_cactus->set_type(type);
	}
	return new_cactus;
}
