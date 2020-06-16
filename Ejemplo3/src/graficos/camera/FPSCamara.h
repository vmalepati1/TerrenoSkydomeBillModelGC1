#pragma once

#include "matematicas/mat4.h"
#include "matematicas/Quaternion.h"

class FPSCamara {
private:
	mat4 m_ProjectionMatrix, m_ViewMatrix;
	
	vec3 m_Position, m_Rotation;

	float m_MouseSensitivity;
	float m_Speed, m_SprintSpeed;
	float m_Pitch, m_Yaw;

	bool m_MouseWasGrabbed;

public:
	FPSCamara(const mat4& projectionMatrix);
	~FPSCamara();

	inline const vec3& GetPosition() const { return m_Position; }
	inline void SetPosition(const vec3& position) { m_Position = position; }

	inline const vec3& GetRotation() const { return m_Rotation; }
	inline void SetRotation(const vec3& rotation) { m_Rotation = rotation; }

	inline const mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	inline void SetProjectionMatrix(const mat4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }

	inline void Translate(const vec3& translation) { m_Position += translation; }
	inline void Rotate(const vec3& rotation) { m_Rotation += rotation; }

	inline void Translate(float x, float y, float z) { m_Position += vec3(x, y, z); }
	inline void Rotate(float x, float y, float z) { m_Rotation += vec3(x, y, z); }

	inline void InvertPitch() { m_Pitch = -m_Pitch; }

	inline float GetYaw() { return m_Yaw; }

	inline const mat4& GetViewMatrix() { return m_ViewMatrix; }

	void UpdateViewMatrix();
	void Focus();
	void Update();

	Quaternion GetOrientation() const;
	vec3 GetForwardDirection(const Quaternion& orientation) const;
	vec3 GetUpDirection(const Quaternion& orientation) const;
	vec3 GetRightDirection(const Quaternion& orientation) const;
};