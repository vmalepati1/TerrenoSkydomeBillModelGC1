#include <windows.h>
#include <windowsx.h>
#include "openglclass.h"
#include "graphicsclass.h"
#include "GamepadRR.h"
#include "Timer.h"
#include "InputGerente.h"

#include <iostream>
#include <sstream>
#include <string>

#pragma comment (lib,"Gdiplus.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
extern void MouseButtonCallback(InputGerente* inputManager, int32_t button, int32_t x, int32_t y);
extern void KeyCallback(InputGerente* inputManager, int32_t flags, int32_t key, uint32_t message);

LPCSTR m_applicationName;
HINSTANCE m_hinstance;
HWND m_hwnd;

OpenGLClass* m_OpenGL;
GraphicsClass* m_Graphics;
InputGerente* m_InputManager;
GamePadRR* control;

Timer *m_Timer;
uint32_t m_UpdatesPerSecond, m_FramesPerSecond;
float m_Frametime;

int screenWidth, screenHeight;

int main()
{	
	int screenWidth, screenHeight;
	bool result;
	m_OpenGL = 0;
	m_Graphics = 0;

	screenWidth = 0;
	screenHeight = 0;

	// Create the OpenGL object.
	m_OpenGL = new OpenGLClass;
	if (!m_OpenGL)
	{
		return false;
	}

	m_InputManager = new InputGerente();


	// Create the window the application will be using and also initialize OpenGL.
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = (LPCSTR)"Barco de Agua";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCSTR)m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Create a temporary window for the OpenGL extension setup.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, (LPCSTR)m_applicationName, (LPCSTR)m_applicationName, WS_POPUP,
		0, 0, 640, 480, NULL, NULL, m_hinstance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	// Don't show the window.
	ShowWindow(m_hwnd, SW_HIDE);

	// Initialize a temporary OpenGL window and load the OpenGL extensions.
	result = m_OpenGL->InitializeExtensions(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, (LPCSTR)"Could not initialize the OpenGL extensions.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Release the temporary window now that the extensions have been initialized.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, (LPCSTR)m_applicationName, (LPCSTR)m_applicationName, WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	// Initialize OpenGL now that the window has been created.
	result = m_OpenGL->InitializeOpenGL(m_hwnd, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED);
	if (!result)
	{
		MessageBox(m_hwnd, (LPCSTR)"Could not initialize OpenGL, check if video card supports OpenGL 4.0.",
			(LPCSTR)"Error", MB_OK);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(m_OpenGL, m_hwnd);
	if (!result)
	{
		return false;
	}

	control = new GamePadRR(1);

	MSG msg;
	bool done;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	SetTimer(m_hwnd, 1000, 20, NULL);
	// Loop until there is a quit message from the window or the user.
	done = false;

	m_Timer = new Timer();

	float timer = 0.0f;
	float updateTimer = m_Timer->ElapsedMillis();
	float updateTick = 1000.0f / 60.0f;
	uint32_t frames = 0;
	uint32_t updates = 0;
	float frametime = 0.0;

	// Loop until there is a quit message from the window or the user.
	while (!done)
	{
		MSG message;
		while (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE) > 0)
		{
			if (message.message == WM_QUIT)
			{
				done = true;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		float now = m_Timer->ElapsedMillis();
		if (now - updateTimer > updateTick)
		{
			updates++;
			updateTimer += updateTick;
		}

		Timer frametime;

		m_InputManager->Update();
		m_InputManager->PlatformUpdate();

		m_Graphics->Frame();

		frames++;
		m_Frametime = frametime.ElapsedMillis();

		if (m_Timer->Elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			m_FramesPerSecond = frames;
			m_UpdatesPerSecond = updates;
			frames = 0;
			updates = 0;

			std::cout << "FPS: " << m_FramesPerSecond << ", UPS: " << m_UpdatesPerSecond << std::endl;
		}
	}

	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}	

	// Release the OpenGL object.
	if (m_OpenGL)
	{
		m_OpenGL->Shutdown(m_hwnd);
		delete m_OpenGL;
		m_OpenGL = 0;
	}

	if (control)
	{		
		delete control;
		control = 0;
	}

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass((LPCSTR)m_applicationName, m_hinstance);
	m_hinstance = NULL;

	return 0;
}

void FocusCallback(bool focused)
{
	if (!focused)
	{
		m_InputManager->ClearKeys();
		m_InputManager->ClearMouseButtons();
	}
}

void ResizeCallback(int32_t width, int32_t height)
{
	screenWidth = width;
	screenHeight = height;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = NULL;

	switch (message)
	{
	case WM_ACTIVATE:
	{
		if (!HIWORD(wParam)) // Is minimized
		{
			// active
		}
		else
		{
			// inactive
		}

		return 0;
	}
	case WM_SYSCOMMAND:
	{
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		result = DefWindowProc(hWnd, message, wParam, lParam);
	} break;
	case WM_SETFOCUS:
		FocusCallback(true);
		break;
	case WM_KILLFOCUS:
		FocusCallback(false);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		KeyCallback(m_InputManager, lParam, wParam, message);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		MouseButtonCallback(m_InputManager, message, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_SIZE:
		ResizeCallback(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		result = DefWindowProc(hWnd, message, wParam, lParam);
	}
	return result;
}
