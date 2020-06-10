#include "InputGerente.h"
#include "platform/Application.h"

#include <Windows.h>
#include <Windowsx.h>

InputGerente::InputGerente()
{
	ClearKeys();
	ClearMouseButtons();

	m_MouseGrabbed = true;
	// m_KeyState = spnew bool[MAX_KEYS];
	// m_LastKeyState = spnew bool[MAX_KEYS];
}

void InputGerente::Update()
{
	for (int32_t i = 0; i < MAX_BUTTONS; i++)
		m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

	memcpy(m_LastKeyState, m_KeyState, MAX_KEYS);
	memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);
}

void InputGerente::ClearKeys()
{
	for (int32_t i = 0; i < MAX_KEYS; i++)
	{
		m_KeyState[i] = false;
		m_LastKeyState[i] = false;
	}
	m_KeyModifiers = 0;
}

void InputGerente::ClearMouseButtons()
{
	for (int32_t i = 0; i < MAX_BUTTONS; i++)
	{
		m_MouseButtons[i] = false;
		m_MouseState[i] = false;
		m_MouseClicked[i] = false;
	}
}

bool InputGerente::IsKeyPressed(uint32_t keycode) const
{
	// TODO: Log this!
	if (keycode >= MAX_KEYS)
		return false;

	return m_KeyState[keycode];
}

bool InputGerente::IsMouseButtonPressed(uint32_t button) const
{
	// TODO: Log this!
	if (button >= MAX_BUTTONS)
		return false;

	return m_MouseButtons[button];
}

bool InputGerente::IsMouseButtonClicked(uint32_t button) const
{
	// TODO: Log this!
	if (button >= MAX_BUTTONS)
		return false;

	return m_MouseClicked[button];
}

const vec2& InputGerente::GetMousePosition() const
{
	return m_MousePosition;
}

const bool InputGerente::IsMouseGrabbed() const
{
	return m_MouseGrabbed;
}

void InputGerente::SetMouseGrabbed(bool grabbed)
{
	m_MouseGrabbed = grabbed;
}

void InputGerente::PlatformUpdate()
{
	// Mouse Events
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(Application::GetApplication().GetHWnd(), &mouse);

	vec2 mousePos = vec2((float)mouse.x, (float)mouse.y);
	if (mousePos != m_MousePosition)
	{
		m_MousePosition = mousePos;
	}
}

void InputGerente::SetMousePosition(const vec2& position)
{
	POINT pt = { (LONG)position.x, (LONG)position.y };
	ClientToScreen(Application::GetApplication().GetHWnd(), &pt);
	SetCursorPos(pt.x, pt.y);
}

void InputGerente::SetMouseCursor(int32_t cursor)
{
	if (cursor == NO_CURSOR)
	{
		SetCursor(NO_CURSOR);
		while (ShowCursor(false) >= 0);
	}
	else
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		ShowCursor(true);
	}
}

void KeyCallback(InputGerente* inputManager, int32_t flags, int32_t key, uint32_t message)
{
	bool pressed = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
	inputManager->m_KeyState[key] = pressed;

	bool repeat = (flags >> 30) & 1;

	int32_t modifier = 0;
	switch (key)
	{
	case KEY_CONTROL:
		modifier = MODIFIER_LEFT_CONTROL;
		break;
	case KEY_ALT:
		modifier = MODIFIER_LEFT_ALT;
		break;
	case KEY_SHIFT:
		modifier = MODIFIER_LEFT_SHIFT;
		break;
	}
	if (pressed)
		inputManager->m_KeyModifiers |= modifier;
	else
		inputManager->m_KeyModifiers &= ~(modifier);
}

void MouseButtonCallback(InputGerente* inputManager, int32_t button, int32_t x, int32_t y)
{
	bool down = false;
	switch (button)
	{
	case WM_LBUTTONDOWN:
		SetCapture(Application::GetApplication().GetHWnd());
		button = MOUSE_LEFT;
		down = true;
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		button = MOUSE_LEFT;
		down = false;
		break;
	case WM_RBUTTONDOWN:
		SetCapture(Application::GetApplication().GetHWnd());
		button = MOUSE_RIGHT;
		down = true;
		break;
	case WM_RBUTTONUP:
		ReleaseCapture();
		button = MOUSE_RIGHT;
		down = false;
		break;
	case WM_MBUTTONDOWN:
		SetCapture(Application::GetApplication().GetHWnd());
		button = MOUSE_MIDDLE;
		down = true;
		break;
	case WM_MBUTTONUP:
		ReleaseCapture();
		button = MOUSE_MIDDLE;
		down = false;
		break;
	}
	inputManager->m_MouseButtons[button] = down;
	inputManager->m_MousePosition.x = (float)x;
	inputManager->m_MousePosition.y = (float)y;
}
