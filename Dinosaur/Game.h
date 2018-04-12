#ifndef VS_GAME_HPP
#define VS_GAME_HPP

#include <windows.h>
#include <d2d1.h>
#include <Dwrite.h>

#include "StepTimer.h"
#include "GameLogic.h"


//Base address of dos module, same as the address of the current instance
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

namespace vs {
	class game {
	private:
		//Variables
		HWND								_hwnd;
		ID2D1Factory*						_direct2d_factory;
		ID2D1HwndRenderTarget*				_render_target;
		IDWriteFactory*						_write_factory;
		IDWriteTextFormat*					_text_format;
		DX::StepTimer						_timer;
		game_logic							_logic;

		//Functions
		HRESULT								create_device_independant_resources		();
		HRESULT								create_device_resources					();
		void								discard_device_resources				();

		HRESULT								on_render								();
		void								on_update								(const DX::StepTimer& timer);

		void								on_resize								(UINT width, UINT height);

		static LRESULT CALLBACK				wnd_proc								(HWND hWnd,
																					UINT message,
																					WPARAM wParam,
																					LPARAM lParam);

	public:
		game																		();
		~game																		();

		HRESULT								initialize								();

		void								run_game_loop							();
		ID2D1Factory*						get_direct2d_factory					() const;
	};
}

#endif //VS_GAME_HPP