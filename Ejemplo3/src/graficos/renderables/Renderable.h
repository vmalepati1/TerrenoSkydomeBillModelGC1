#pragma once

#include "graficos/camera/FPSCamara.h"
#include "Geometrias.h"
#include "graficos/lightshaderclass.h"
#include "graficos/lightclass.h"

class Renderable : public Primitivos {
protected:
	Renderable(const mat4& transform, LightClass* lightSetup);
	~Renderable();

	virtual void PushBuffers();
	virtual void BindBuffers();
	virtual void UnbindBuffers();
	virtual void Bind(FPSCamara *camera) { }
	virtual void Unbind(FPSCamara *camera) { }
protected:
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;

	Maya* m_data;

	LightShaderClass* m_shader;
	LightClass* m_lightSetup;

	mat4 m_transform;
public:
	virtual void Render(FPSCamara *camera, const vec4& clipPlane);

	inline LightShaderClass* GetShader() { return m_shader; }

	inline void SetTransform(const mat4 &transform) { m_transform = transform; }
	inline const mat4& GetTransform() const { return m_transform; }
};