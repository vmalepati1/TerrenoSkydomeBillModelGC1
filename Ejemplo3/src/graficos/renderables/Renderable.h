#pragma once

#include "graficos/camera/FPSCamara.h"

class Renderable {
protected:
	virtual void Bind() = 0;
	virtual void Unbind() = 0;
public:
	virtual void Render(FPSCamara *camera) = 0;
};