#include <windows.h>
#include "Game.h"
#include <ctime>

/// <summary>
/// Main entry point of the program.
/// </summary>
/// <param name="hInstance">A handle to the current instance of the application.</param>
/// <param name="hPrevInstance">A handle to the previous instance of the application. </param>
/// <param name="lpCmdLine ">The command line for the application, excluding the program name. </param>
/// <param name="nCmdShow">Controls how the window is to be shown.</param>
/// <returns></returns>
int WINAPI WinMain(
	HINSTANCE h_instance,
	HINSTANCE h_prev_instance,
	LPSTR lp_cmd_line,
	int n_cmd_show) {
	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

	//Seed the random
	srand(time(nullptr));

	if (SUCCEEDED(CoInitialize(NULL))) {
		{
			vs::game game;

			if (SUCCEEDED(game.initialize())) {
				game.run_game_loop();
			}
		}
		//This function closes the OLE Component Object Model (COM) library, freeing any resources that it maintains.
		CoUninitialize();
	}

	return 0;
}
