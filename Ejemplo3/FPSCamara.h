#pragma once

#include "mat4.h"
#include "Quaternion.h"

class FPSCamara {
public:
	mat4 m_ProjectionMatrix, m_ViewMatrix;
	
	vec3 m_Position, m_Rotation;

	float m_MouseSensitivity;
	float m_Speed, m_SprintSpeed;
	float m_Pitch, m_Yaw;

	bool m_MouseWasGrabbed;

	void Focus();
	void Update();

public:
	FPSCamara(const mat4& projectionMatrix);
	~FPSCamara();

	inline void Translate(const vec3& translation) { m_Position += translation; }
	inline void Rotate(const vec3& rotation) { m_Rotation += rotation; }

	inline void Translate(float x, float y, float z) { m_Position += vec3(x, y, z); }
	inline void Rotate(float x, float y, float z) { m_Rotation += vec3(x, y, z); }

	Quaternion GetOrientation() const;
	vec3 GetForwardDirection(const Quaternion& orientation) const;
	vec3 GetUpDirection(const Quaternion& orientation) const;
	vec3 GetRightDirection(const Quaternion& orientation) const;
};