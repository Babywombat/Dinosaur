#include <iostream>


#include "Game.h"
#include "Input.h"
#include "Defines.h"
#include "Utils.h"

using namespace D2D1;
using namespace vs;

/// <summary>
/// Constructor
/// </summary>
game::game() :
	_hwnd					(nullptr),
	_direct2d_factory		(nullptr),
	_render_target			(nullptr), 
	_write_factory			(nullptr), 
	_text_format			(nullptr),
	_logic					(this) {}

/// <summary>
/// Destructor
/// </summary>
game::~game() {
	utils::safe_release(&_direct2d_factory);
	utils::safe_release(&_render_target);
}

/// <summary>
/// Initializes the window and resources of the game
/// </summary>
/// <returns>HRESULT</returns>
HRESULT game::initialize() {

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	auto hr = create_device_independant_resources();

	if (SUCCEEDED(hr)) {
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = wnd_proc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDI_APPLICATION);
		wcex.lpszClassName = "VSApp";

		RegisterClassEx(&wcex);

		//Calculate the real size of the window for the overlapped style
		RECT rect = { 0, 0, RESOLUTION_X, RESOLUTION_Y };
		AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

		auto dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);

		// Create the window.
		_hwnd = CreateWindow(
			"VSApp",
			"Vertical Shooter",
			dwStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);
		hr = _hwnd ? S_OK : E_FAIL;

		//Show the window if it was created
		if (SUCCEEDED(hr)) {
			ShowWindow(_hwnd, SW_SHOWNORMAL);
			UpdateWindow(_hwnd);
		}
	}

	return hr;
}

/// <summary>
/// Runs the message loop of the window
/// </summary>
void game::run_game_loop() {
	MSG msg;
	msg.message = WM_NULL;

	_logic.initialize();
	//Get all the messages from this thread's windows
	while(msg.message != WM_QUIT) {
		//Check for message
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			auto was_handled = false;
			//Handle all keyboard messages
			was_handled = input::get_instance().try_handle_keyboard_message(msg);

			if (!was_handled) {
				TranslateMessage(&msg);
				//Dispatch the message to the callback
				DispatchMessage(&msg);
			}
		} else {
			if(input::get_instance().is_key_down(input::Escape)) {
				break;
			}

			//Update and render the game
			_timer.Tick([&]() {
				on_update(_timer);
			});

			on_render();
		}
	}
}

/// <summary>
/// Returns the direct2d factory
/// </summary>
/// <returns></returns>
ID2D1Factory* game::get_direct2d_factory() const {
	return _direct2d_factory;
}

/// <summary>
/// Creates the device independant resources such as the factory
/// </summary>
/// <returns>HRESULT</returns>
HRESULT game::create_device_independant_resources() {
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_direct2d_factory);

	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 20;

	if (SUCCEEDED(hr)) {

		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(_write_factory),
			reinterpret_cast<IUnknown **>(&_write_factory)
		);
	}
	if (SUCCEEDED(hr)) {
		// Create a DirectWrite text format object.
		hr = _write_factory->CreateTextFormat(
			msc_fontName,
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&_text_format
		);
	}
	if (SUCCEEDED(hr)) {
		// Center the text horizontally and vertically.
		_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	}

	return hr;
}

/// <summary>
/// Creates the device resources such as the render target
/// </summary>
/// <returns>HRESULT</returns>
HRESULT game::create_device_resources() {
	HRESULT hr = S_OK;

	if (!_render_target) {
		RECT rc;

		//Get the client area of the window (1024x768 if not changed)
		GetClientRect(_hwnd, &rc);

		const D2D1_SIZE_U size = SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = _direct2d_factory->CreateHwndRenderTarget(
			RenderTargetProperties(),
			HwndRenderTargetProperties(_hwnd, size),
			&_render_target
		);
	}
	return hr;
}

/// <summary>
/// Releases the device resources
/// </summary>
void game::discard_device_resources() {
	utils::safe_release(&_render_target);
}

/// <summary>
/// Called to render to the window
/// </summary>
/// <returns></returns>
HRESULT game::on_render() {
	HRESULT hr = S_OK;

	hr = create_device_resources();

	if (SUCCEEDED(hr)) {
		//Begin drawing
		_render_target->BeginDraw();

		_render_target->SetTransform(Matrix3x2F::Identity());

		//clear the window to white
		_render_target->Clear(ColorF(ColorF::LightSkyBlue));

		//Render the game
		_logic.on_render(_render_target, _text_format);

		//End drawing
		hr = _render_target->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET) {
		hr = S_OK;
		discard_device_resources();
	}

	return hr;
}

/// <summary>
/// Updates the game logic
/// </summary>
/// <param name="timer">Timer to get delta time</param>
void game::on_update(const DX::StepTimer& timer) {
	auto delta = timer.GetElapsedSeconds();
	if(_logic.on_update(delta)) {
		//Game ended
		PostQuitMessage(0);
	}
}

/// <summary>
/// Called when the window resizes
/// </summary>
/// <param name="width">width of the window</param>
/// <param name="height">height of the window</param>
void game::on_resize(UINT width, UINT height) {
	if (_render_target != nullptr) {
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		_render_target->Resize(SizeU(width, height));
	}
}

/// <summary>
/// Handles the messages for a window
/// </summary>
/// <param name="hwnd">The window handle</param>
/// <param name="message">The message</param>
/// <param name="w_param">Additional parameters</param>
/// <param name="l_param">Additional parameters</param>
/// <returns></returns>
LRESULT game::wnd_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
	LRESULT result = 0;

	if (message == WM_CREATE) {
		//Store the game instance on creation
		auto pcs = reinterpret_cast<LPCREATESTRUCT>(l_param);
		auto g = static_cast<game*>(pcs->lpCreateParams);

		::SetWindowLongPtr(
			hwnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(g)
		);

		result = 1;
	} else {
		//Get the game instance
		game* g = reinterpret_cast<game*>(
			::GetWindowLongPtr(
				hwnd,
				GWLP_USERDATA
			));

		auto was_handled = false;

		if (g) {
			switch (message) {
				case WM_SIZE: {
					//Resizing event
					const UINT width = LOWORD(l_param);
					const UINT height = HIWORD(l_param);
					g->on_resize(width, height);

					result = 0;
					was_handled = true;
					break;
				}
				case WM_DISPLAYCHANGE:{
					//Resolution has changed
					InvalidateRect(hwnd, nullptr, FALSE);

					result = 0;
					was_handled = true;
					break;

				}
				/*case WM_PAINT:{
					//Paint the window
					g->on_render();
					ValidateRect(hwnd, nullptr);

					result = 0;
					was_handled = true;
					break;
				}*/
				case WM_DESTROY: {
					//Destroy the window
					PostQuitMessage(0);

					result = 1;
					was_handled = true;
					break;
				}
			}
		}

		if (!was_handled) {
			//Handle with default behaviour
			result = DefWindowProc(hwnd, message, w_param, l_param);
		}
	}

	return result;
}

