#pragma once

#include "Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "Renderable.h"

class Water : public Renderable
{
public:
	Water(const mat4& transform, LightClass* lightSetup);

	~Water();
};