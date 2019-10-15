#include <stdio.h>
#include <Windows.h>


bool running = true;



LRESULT CALLBACK window_callback(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	LRESULT result = 0;
	switch (uMsg) {
	
	case WM_CLOSE:

	case WM_DESTROY: {
		running = false;
	} break;
	
	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
	
	return result;
};


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// Create Window Class
	WNDCLASS windows_class = {};
	windows_class.style = CS_HREDRAW | CS_VREDRAW;
	windows_class.lpszClassName = "Game Window Class";
	windows_class.lpfnWndProc = window_callback;
	// Register Class
	RegisterClass(&windows_class);
	// Create Window
	HWND gameWindow = CreateWindowA(
		windows_class.lpszClassName,
		"Pong Game Window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1280,
		720,
		0,
		0,
		hInstance,
		0
	);

	while (running) {

		// Inpute
		MSG message;
		while (PeekMessage(&message, gameWindow, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		//Simulate

		//Render
	}
};