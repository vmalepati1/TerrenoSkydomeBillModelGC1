#pragma once

#include "Renderable.h"
#include "Geometrias.h"
#include "graficos/lightshaderclass.h"

class Cubo : Renderable {
private:
	Maya *vertData;
	LightShaderClass* shader;

	OpenGLClass* glInstance;
	unsigned int m_vertexBufferId, m_indexBufferId;

	void Bind() override;
	void Unbind() override;

public:
	Cubo(OpenGLClass *glInstance, HWND hwnd, float size);
	~Cubo();

	void Render(FPSCamara *camera) override;

	inline LightShaderClass* GetShader() { return shader; }
};