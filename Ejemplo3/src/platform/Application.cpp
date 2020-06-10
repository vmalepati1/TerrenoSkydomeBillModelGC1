#include "Application.h"

#include <iostream>

#pragma comment (lib,"Gdiplus.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
extern void MouseButtonCallback(InputGerente* inputManager, int32_t button, int32_t x, int32_t y);
extern void KeyCallback(InputGerente* inputManager, int32_t flags, int32_t key, uint32_t message);

Application* Application::s_Instance = nullptr;

Application::Application(const string& name, const WindowProperties& windowProperties)
	: m_Name(name), m_WindowProperties(windowProperties) {
	s_Instance = this;

	m_OpenGL = new OpenGLClass();
	m_InputManager = new InputGerente();
	m_Graphics = new GraphicsClass();
	m_Controller = new GamePadRR(1);
	m_Timer = new Timer();
}

Application::~Application() {

}

void Application::Start() {
	Init();
	m_Running = true;
	m_Suspended = false;
	Run();
	Shutdown();
}

void Application::Suspend()
{
	m_Suspended = true;
}

void Application::Resume()
{
	m_Suspended = false;
}

void Application::Stop()
{
	m_Running = false;
}

bool Application::Init() {
	bool result;

	// Create the window the application will be using and also initialize OpenGL.
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get the instance of this application.
	m_hInstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_Name.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Create a temporary window for the OpenGL extension setup.
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, m_Name.c_str(), WS_POPUP,
		0, 0, 640, 480, NULL, NULL, m_hInstance, NULL);

	if (m_hWnd == NULL)
	{
		MessageBox(m_hWnd, (LPCSTR)"Could not create window.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Don't show the window.
	ShowWindow(m_hWnd, SW_HIDE);

	// Initialize a temporary OpenGL window and load the OpenGL extensions.
	result = m_OpenGL->InitializeExtensions();
	if (!result)
	{
		MessageBox(m_hWnd, (LPCSTR)"Could not initialize the OpenGL extensions.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Release the temporary window now that the extensions have been initialized.
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// Determine the resolution of the clients desktop screen.
	int fullScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int fullScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_WindowProperties.fullscreen)
	{
		m_WindowProperties.width = fullScreenWidth;
		m_WindowProperties.height = fullScreenHeight;

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)fullScreenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)fullScreenHeight;
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
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_WindowProperties.width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_WindowProperties.height) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, m_Name.c_str(), WS_POPUP,
		posX, posY, m_WindowProperties.width, m_WindowProperties.height, NULL, NULL, m_hInstance, NULL);
	if (m_hWnd == NULL)
	{
		MessageBox(m_hWnd, (LPCSTR)"Could not create window.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Initialize OpenGL now that the window has been created.
	result = m_OpenGL->InitializeOpenGL();
	if (!result)
	{
		MessageBox(m_hWnd, (LPCSTR)"Could not initialize OpenGL, check if video card supports OpenGL 4.0.",
			(LPCSTR)"Error", MB_OK);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	// Initialize the graphics object.
	result = m_Graphics->Initialize();
	if (!result)
	{
		MessageBox(m_hWnd, (LPCSTR)"Could not initialize graphics.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	SetTimer(m_hWnd, 1000, 20, NULL);
}

void Application::Run() {
	float timer = 0.0f;
	float updateTimer = m_Timer->ElapsedMillis();
	float updateTick = 1000.0f / 60.0f;
	uint32_t frames = 0;
	uint32_t updates = 0;
	float frametime = 0.0;

	// Loop until there is a quit message from the window or the user.
	while (m_Running)
	{
		MSG message;
		while (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE) > 0)
		{
			if (message.message == WM_QUIT)
			{
				m_Running = false;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		float now = m_Timer->ElapsedMillis();
		if (now - updateTimer > updateTick)
		{
			OnUpdate();
			updates++;
			updateTimer += updateTick;
		}

		Timer frametime;
		OnRender();

		frames++;
		m_Frametime = frametime.ElapsedMillis();

		if (m_Timer->Elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			m_FramesPerSecond = frames;
			m_UpdatesPerSecond = updates;
			frames = 0;
			updates = 0;

			OnTick();
		}
	}
}

void Application::OnTick() {
	std::cout << "FPS: " << m_FramesPerSecond << ", UPS: " << m_UpdatesPerSecond << std::endl;
}

void Application::OnUpdate() {

}

void Application::OnRender() {
	m_InputManager->Update();
	m_InputManager->PlatformUpdate();
	m_Graphics->Render();
}

void Application::Shutdown() {
	m_Graphics->Shutdown();
	m_OpenGL->Shutdown();

	// Fix the display settings if leaving full screen mode.
	if (m_WindowProperties.fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_Name.c_str(), m_hInstance);
	m_hInstance = NULL;

	delete m_OpenGL;
	delete m_InputManager;
	delete m_Graphics;
	delete m_Controller;
	delete m_Timer;
}

void FocusCallback(Application &app, bool focused)
{
	if (!focused)
	{
		app.m_InputManager->ClearKeys();
		app.m_InputManager->ClearMouseButtons();
	}
}

void ResizeCallback(Application &app, int32_t width, int32_t height)
{
	app.m_WindowProperties.width = width;
	app.m_WindowProperties.height = height;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = NULL;
	Application app = Application::GetApplication();

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
		FocusCallback(app, true);
		break;
	case WM_KILLFOCUS:
		FocusCallback(app, false);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		KeyCallback(app.GetInputManager(), lParam, wParam, message);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		MouseButtonCallback(app.GetInputManager(), message, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_SIZE:
		ResizeCallback(app, LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		result = DefWindowProc(hWnd, message, wParam, lParam);
	}
	return result;
}
