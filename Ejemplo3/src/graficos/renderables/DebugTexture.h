#pragma once

#include "Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "Renderable.h"

class DebugTexture : public Renderable {
public:
	DebugTexture(unsigned int textureID, const mat4 &transform);

	void Render(FPSCamara *camera, const vec4& clipPlane) override;

	void SetTextureID(unsigned int id) { m_textureID = id; }
private:
	void Bind(FPSCamara *camera) override;
	void Unbind(FPSCamara* camera) override;

	unsigned int m_textureID;
};