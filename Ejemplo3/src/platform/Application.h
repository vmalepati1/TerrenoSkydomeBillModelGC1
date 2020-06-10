#pragma once

#include <string>
#include <windows.h>
#include <windowsx.h>

using namespace std;

#include "matematicas/vec2.h"
#include "api/openglclass.h"
#include "graficos/graphicsclass.h"
#include "platform/GamepadRR.h"
#include "platform/Timer.h"
#include "platform/InputGerente.h"

struct WindowProperties
{
	unsigned int width, height;
	bool fullscreen;
	bool vsync;
	float screenNear;
	float screenFar;
};

class Application {
private:
	static Application* s_Instance;

	bool m_Running, m_Suspended;

	string m_Name;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	OpenGLClass* m_OpenGL;
	InputGerente* m_InputManager;
	GraphicsClass* m_Graphics;
	GamePadRR* m_Controller;

	Timer* m_Timer;
	uint32_t m_UpdatesPerSecond, m_FramesPerSecond;
	float m_Frametime;

	WindowProperties m_WindowProperties;
public:
	Application(const string& name, const WindowProperties &windowProperties);
	~Application();

	void Start();
	void Suspend();
	void Resume();
	void Stop();

	inline HINSTANCE GetHInstance() const { return m_hInstance; }
	inline HWND GetHWnd() const { return m_hWnd; }
	inline OpenGLClass* GetOpenGL() const { return m_OpenGL; }
	inline InputGerente* GetInputManager() const { return m_InputManager; }
	inline GraphicsClass* GetGraphics() const { return m_Graphics; }
	inline GamePadRR* GetController() const { return m_Controller; }

	inline unsigned int GetFPS() const { return m_FramesPerSecond; }
	inline unsigned int GetUPS() const { return m_UpdatesPerSecond; }
	inline float GetFrametime() const { return m_Frametime; }

	inline unsigned int GetWindowWidth() const { return m_WindowProperties.width; }
	inline unsigned int GetWindowHeight() const { return m_WindowProperties.height; }
	inline vec2 GetWindowSize() const { return vec2((float)m_WindowProperties.width, (float)m_WindowProperties.height); }
	inline bool IsWindowVsync() const { return m_WindowProperties.vsync; }
	inline float GetWindowNear() const { return m_WindowProperties.screenNear; }
	inline float GetWindowFar() const { return m_WindowProperties.screenFar; }

private:
	bool Init();
	void Run();
	void OnTick();
	void OnUpdate();
	void OnRender();
	void Shutdown();

	friend void ResizeCallback(Application &app, int32_t width, int32_t height);
	friend void FocusCallback(Application &app, bool focused);
public:
	inline static Application& GetApplication() { return *s_Instance; }
};