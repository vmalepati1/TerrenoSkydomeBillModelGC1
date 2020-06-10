#pragma once

#include "Renderable.h"
// #include "Geometrias.h"
#include "graficos/lightshaderclass.h"

//estructura para manejar primitivos con posicion, normal y uv's
struct Vertices {
	float Posx, Posy, Posz;
	float Normx, Normy, Normz;
	float u, v;
	float Tx, Ty, Tz;
	float Bx, By, Bz;
};

//En honor a nuestros ancestros llamaremos "Maya" a la malla
//estructura que contiene datos de los vertices y sus indices
struct Maya {
	Vertices* maya;
	unsigned int* indices;
};

class Cubo : Renderable {
private:
	Maya *vertData;
	LightShaderClass* shader;

	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;

	void Bind() override;
	void Unbind() override;

public:
	Cubo(float size);
	~Cubo();

	void Render(FPSCamara *camera) override;

	inline LightShaderClass* GetShader() { return shader; }
};