#include <stdio.h>
#include <Windows.h>


bool running = true;

void* bufferMemory;
int bufferWidth;
int bufferHeight;
BITMAPINFO bufferBitmapInfo;


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

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		bufferWidth = rect.right - rect.left;
		bufferHeight = rect.bottom - rect.top;

		int bufferSize = bufferWidth * bufferHeight * sizeof(unsigned int);

		if (bufferMemory) {
			VirtualFree(bufferMemory, 0, MEM_RELEASE);
		}
		bufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		bufferBitmapInfo.bmiHeader.biSize = sizeof(bufferBitmapInfo.bmiHeader);
		bufferBitmapInfo.bmiHeader.biWidth = bufferWidth;
		bufferBitmapInfo.bmiHeader.biHeight = bufferHeight;
		bufferBitmapInfo.bmiHeader.biPlanes = 1;
		bufferBitmapInfo.bmiHeader.biBitCount = 32;
		bufferBitmapInfo.bmiHeader.biCompression = BI_RGB;

	} break;
	
	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
	
	return result;
};


int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// Create Window Class
	WNDCLASS windows_class = {};
	windows_class.style = CS_HREDRAW | CS_VREDRAW;
	windows_class.lpszClassName = "Game Window Class";
	windows_class.lpfnWndProc = window_callback;
	// Register Class
	RegisterClass(&windows_class);
	// Create Window
	HWND gameWindow = CreateWindow(
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

	HDC hdc = GetDC(gameWindow);

	while (running) {

		// Inpute
		MSG message;
		while (PeekMessage(&message, gameWindow, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		//Simulate
		unsigned int* pixel = (unsigned int*)bufferMemory;
		for (int y = 0; y < bufferHeight; y++) {
			for (int x = 0; x < bufferWidth; x++) {
				*pixel++ = x * y;
			}

		}

		//Render
		StretchDIBits(hdc, 0, 0, bufferWidth, bufferHeight, 0, 0, bufferWidth, bufferHeight, bufferMemory, &bufferBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
};