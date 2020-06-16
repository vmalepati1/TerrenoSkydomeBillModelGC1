#pragma once

#include "Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "Renderable.h"
#include "WaterFrameBuffers.h"

class Billboard : public Renderable
{
public:
	Billboard(const wchar_t* texture, const mat4& transform, LightClass* lightSetup);

	~Billboard();
private:
	void Bind(FPSCamara* camera) override;
	void Unbind(FPSCamara* camera) override;

	unsigned int m_textureID;
};