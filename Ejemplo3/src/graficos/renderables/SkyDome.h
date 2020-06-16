#pragma once

#include "Geometrias.h"
#include "graficos/Imagenes.h"
#include "graficos/renderables/Renderable.h"

class SkyDome : public Renderable
{
public:
	//variables locales de la clase para contener los stacks y los slices de la esfera
	int st, sl;
	unsigned int m_dayTextureID, m_nightTextureID;

	float time;

	//El constructor llama al metodo Esfera de la clase geometrias que generara los vertices
	//normales y uvs de la misma, nos regresa la estructura Maya.
	SkyDome(int stacks, int slices, float radio, const wchar_t dayTexture[], const wchar_t nightTexture[], const mat4 &transform, LightClass *lightSetup);

	~SkyDome();

	void Render(FPSCamara* camera, const vec4& clipPlane) override;
private:
	void Bind(FPSCamara* camera) override;
	void Unbind(FPSCamara* camera) override;
};