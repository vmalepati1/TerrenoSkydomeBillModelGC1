#pragma once

#include "Renderable.h"
// #include "Geometrias.h"
#include "graficos/lightshaderclass.h"

class Cubo : public Renderable {
public:
	Cubo(float size, const mat4 &transform, LightClass *lightSetup);
};