#pragma once

#include "matematicas/vec2.h"

#include <functional>

#define BIT(x) (1 << x)

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

class Window;

class InputGerente
{
private:
	friend class Window;
private:
	bool m_KeyState[MAX_KEYS];
	bool m_LastKeyState[MAX_KEYS];

	bool m_MouseButtons[MAX_BUTTONS];
	bool m_MouseState[MAX_BUTTONS];
	bool m_MouseClicked[MAX_BUTTONS];
	bool m_MouseGrabbed;
	int32_t m_KeyModifiers;

	vec2 m_MousePosition;
public:
	InputGerente();

	void Update();
	void PlatformUpdate();

	bool IsKeyPressed(uint32_t keycode) const;
	bool IsMouseButtonPressed(uint32_t button) const;
	bool IsMouseButtonClicked(uint32_t button) const;

	const vec2& GetMousePosition() const;
	void SetMousePosition(const vec2& position);
	const bool IsMouseGrabbed() const;
	void SetMouseGrabbed(bool grabbed);
	void SetMouseCursor(int32_t cursor);

	void ClearKeys();
	void ClearMouseButtons();
private:
	friend void KeyCallback(InputGerente* inputManager, int32_t flags, int32_t key, uint32_t message);
	friend void MouseButtonCallback(InputGerente* inputManager, int32_t button, int32_t x, int32_t y);
};

class Input
{
private:
	friend class InputGerente;
private:
	static InputGerente* s_InputManager;
public:
	inline static bool IsKeyPressed(uint32_t keycode) { return s_InputManager->IsKeyPressed(keycode); }
	inline static bool IsMouseButtonPressed(uint32_t button) { return s_InputManager->IsMouseButtonPressed(button); }
	inline static bool IsMouseButtonClicked(uint32_t button) { return s_InputManager->IsMouseButtonClicked(button); }

	inline static const vec2& GetMousePosition() { return s_InputManager->GetMousePosition(); }

	inline static InputGerente* GetInputManager() { return s_InputManager; }
};

#define MOUSE_LEFT	  0x00
#define MOUSE_MIDDLE	  0x01
#define MOUSE_RIGHT    0x02

#define NO_CURSOR	  NULL

#define MODIFIER_LEFT_CONTROL	BIT(0)
#define MODIFIER_LEFT_ALT		BIT(1)
#define MODIFIER_LEFT_SHIFT		BIT(2)
#define MODIFIER_RIGHT_CONTROL	BIT(3)
#define MODIFIER_RIGHT_ALT		BIT(4)
#define MODIFIER_RIGHT_SHIFT		BIT(5)

#define KEY_TAB			  0x09

#define KEY_0			  0x30
#define KEY_1			  0x31
#define KEY_2			  0x32
#define KEY_3			  0x33
#define KEY_4			  0x34
#define KEY_5			  0x35
#define KEY_6			  0x36
#define KEY_7			  0x37
#define KEY_8			  0x38
#define KEY_9			  0x39

#define KEY_A			  0x41
#define KEY_B			  0x42
#define KEY_C			  0x43
#define KEY_D			  0x44
#define KEY_E			  0x45
#define KEY_F			  0x46
#define KEY_G			  0x47
#define KEY_H			  0x48
#define KEY_I			  0x49
#define KEY_J			  0x4A
#define KEY_K			  0x4B
#define KEY_L			  0x4C
#define KEY_M			  0x4D
#define KEY_N			  0x4E
#define KEY_O			  0x4F
#define KEY_P			  0x50
#define KEY_Q			  0x51
#define KEY_R			  0x52
#define KEY_S			  0x53
#define KEY_T			  0x54
#define KEY_U			  0x55
#define KEY_V			  0x56
#define KEY_W			  0x57
#define KEY_X			  0x58
#define KEY_Y			  0x59
#define KEY_Z			  0x5A

#define KEY_LBUTTON        0x01
#define KEY_RBUTTON        0x02
#define KEY_CANCEL         0x03
#define KEY_MBUTTON        0x04

#define KEY_ESCAPE         0x1B
#define KEY_SHIFT          0x10
#define KEY_CONTROL        0x11
#define KEY_MENU           0x12
#define KEY_ALT	          KEY_MENU
#define KEY_PAUSE          0x13
#define KEY_CAPITAL        0x14

#define KEY_SPACE          0x20
#define KEY_PRIOR          0x21
#define KEY_NEXT           0x22
#define KEY_END            0x23
#define KEY_HOME           0x24
#define KEY_LEFT           0x25
#define KEY_UP             0x26
#define KEY_RIGHT          0x27
#define KEY_DOWN           0x28
#define KEY_SELECT         0x29
#define KEY_PRINT          0x2A
#define KEY_EXECUTE        0x2B
#define KEY_SNAPSHOT       0x2C
#define KEY_INSERT         0x2D
#define KEY_DELETE         0x2E
#define KEY_HELP           0x2F

#define KEY_NUMPAD0        0x60
#define KEY_NUMPAD1        0x61
#define KEY_NUMPAD2        0x62
#define KEY_NUMPAD3        0x63
#define KEY_NUMPAD4        0x64
#define KEY_NUMPAD5        0x65
#define KEY_NUMPAD6        0x66
#define KEY_NUMPAD7        0x67
#define KEY_NUMPAD8        0x68
#define KEY_NUMPAD9        0x69
#define KEY_MULTIPLY       0x6A
#define KEY_ADD            0x6B
#define KEY_SEPARATOR      0x6C
#define KEY_SUBTRACT       0x6D
#define KEY_DECIMAL        0x6E
#define KEY_DIVIDE         0x6F
#define KEY_F1             0x70
#define KEY_F2             0x71
#define KEY_F3             0x72
#define KEY_F4             0x73
#define KEY_F5             0x74
#define KEY_F6             0x75
#define KEY_F7             0x76
#define KEY_F8             0x77
#define KEY_F9             0x78
#define KEY_F10            0x79
#define KEY_F11            0x7A
#define KEY_F12            0x7B
#define KEY_F13            0x7C
#define KEY_F14            0x7D
#define KEY_F15            0x7E
#define KEY_F16            0x7F
#define KEY_F17            0x80
#define KEY_F18            0x81
#define KEY_F19            0x82
#define KEY_F20            0x83
#define KEY_F21            0x84
#define KEY_F22            0x85
#define KEY_F23            0x86
#define KEY_F24            0x87

#define KEY_NUMLOCK        0x90
#define KEY_SCROLL         0x91

#define KEY_LSHIFT         0xA0
#define KEY_RSHIFT         0xA1
#define KEY_LCONTROL       0xA2
#define KEY_RCONTROL       0xA3
#define KEY_LMENU          0xA4
#define KEY_RMENU          0xA5