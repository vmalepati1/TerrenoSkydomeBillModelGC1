#include "FPSCamara.h"
#include "InputGerente.h"

#include <math.h>

#include <iostream>

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

extern int screenWidth;
extern int screenHeight;

FPSCamara::FPSCamara(const mat4& projectionMatrix)
	: m_ProjectionMatrix(projectionMatrix), m_MouseSensitivity(0.002f), m_Speed(0.4f), m_SprintSpeed(m_Speed * 4.0f), m_MouseWasGrabbed(false)
{

	m_Position = vec3(0.0f, 25.0f, -25.0f);
	m_Rotation = vec3(90.0f, 0.0f, 0.0f);
	m_Yaw = 2.4f;
	m_Pitch = 0.7f;
}

FPSCamara::~FPSCamara() {

}

void FPSCamara::Focus()
{
	Input::GetInputManager()->SetMouseCursor(NO_CURSOR);
}

void FPSCamara::Update()
{
	vec2 windowCenter = vec2((float)screenWidth / 2, (float)(int32_t)screenHeight / 2);

	if (Input::IsMouseButtonPressed(MOUSE_RIGHT))
	{
		if (!Input::GetInputManager()->IsMouseGrabbed())
		{
			Input::GetInputManager()->SetMouseGrabbed(true);
			Input::GetInputManager()->SetMouseCursor(NO_CURSOR);
		}
	}

	if (Input::GetInputManager()->IsMouseGrabbed())
	{
		vec2 mouse = Input::GetInputManager()->GetMousePosition();
		mouse.x -= windowCenter.x;
		mouse.y -= windowCenter.y;
		if (m_MouseWasGrabbed)
		{
			m_Yaw += mouse.x * m_MouseSensitivity;
			m_Pitch += mouse.y * m_MouseSensitivity;
		}
		m_MouseWasGrabbed = true;
		Input::GetInputManager()->SetMousePosition(windowCenter);

		Quaternion orientation = GetOrientation();
		m_Rotation = orientation.ToEulerAngles() * (180.0f / M_PI);

		vec3 forward = GetForwardDirection(orientation);
		vec3 right = GetRightDirection(orientation);
		vec3 up = vec3::YAxis();
		float speed = Input::IsKeyPressed(KEY_SHIFT) ? m_SprintSpeed : m_Speed;
		if (Input::IsKeyPressed(KEY_W))
			m_Position += forward * speed;
		else if (Input::IsKeyPressed(KEY_S))
			m_Position -= forward * speed;

		if (Input::IsKeyPressed(KEY_A))
			m_Position -= right * speed;
		else if (Input::IsKeyPressed(KEY_D))
			m_Position += right * speed;

		if (Input::IsKeyPressed(KEY_SPACE))
			m_Position += up * speed;
		if (Input::IsKeyPressed(KEY_CONTROL))
			m_Position -= up * speed;

		mat4 rotation = mat4::Rotate(orientation.Conjugate());
		mat4 translation = mat4::Translate(-m_Position);
		m_ViewMatrix = rotation * translation;
	}

	if (Input::IsKeyPressed(KEY_ESCAPE))
	{
		Input::GetInputManager()->SetMouseGrabbed(false);
		Input::GetInputManager()->SetMouseCursor(1);
		m_MouseWasGrabbed = false;
	}
}

Quaternion FPSCamara::GetOrientation() const
{
	return Quaternion::RotationY(m_Yaw) * Quaternion::RotationX(-m_Pitch);
}

vec3 FPSCamara::GetForwardDirection(const Quaternion& orientation) const
{
	return Quaternion::Rotate(orientation, -vec3::ZAxis());
}

vec3 FPSCamara::GetUpDirection(const Quaternion& orientation) const
{
	return Quaternion::Rotate(orientation, vec3::YAxis());
}

vec3 FPSCamara::GetRightDirection(const Quaternion& orientation) const
{
	return Quaternion::Rotate(orientation, vec3::XAxis());
}