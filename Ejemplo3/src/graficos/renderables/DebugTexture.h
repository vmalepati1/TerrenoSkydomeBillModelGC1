#pragma once

#include "Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "Renderable.h"

class DebugTexture : public Renderable {
public:
	DebugTexture(unsigned int textureID, const mat4 &transform);

	void Render(FPSCamara *camera) override;

	void SetTextureID(unsigned int id) { m_textureID = id; }
private:
	void Bind() override;
	void Unbind() override;

	unsigned int m_textureID;
};