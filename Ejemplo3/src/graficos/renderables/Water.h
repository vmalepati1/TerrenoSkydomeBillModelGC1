#pragma once

#include "Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "Renderable.h"
#include "WaterFrameBuffers.h"

class Water : public Renderable
{
public:
	Water(const wchar_t* dudvMap, const wchar_t* normalMap, const mat4& transform, LightClass* lightSetup, WaterFrameBuffers *fbos);

	~Water();
private:
	void Bind(FPSCamara* camera) override;
	void Unbind(FPSCamara* camera) override;

	WaterFrameBuffers* m_fbos;

	unsigned int m_dudvTextureID;
	unsigned int m_normalMapID;

	float waveSpeed;
	float moveFactor;
};