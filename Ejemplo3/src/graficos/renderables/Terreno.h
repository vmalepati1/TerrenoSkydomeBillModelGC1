#pragma once

#include "Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "Renderable.h"

class Terreno : public Renderable
{

private:
	float anchof;
	float proff;
	float deltax, deltaz;
public:
	int verx, verz;
	//el nombre numerico de la textura en cuestion, por lo pronto una
	unsigned int planoTextura;
	unsigned int m_textureID1;
	unsigned int m_textureID2;
	unsigned int slot1;
	unsigned int slot2;

	Terreno(const wchar_t* alturas, const wchar_t* textura, const wchar_t* textura2,
		float ancho, float prof, int numtext, int numtext2, const mat4 &transform, LightClass *lightSetup);

	~Terreno();
	
	/* Obtener la altura desde posicion (x, z) */
	float Superficie(float x, float z);
private:
	void Bind(FPSCamara* camera) override;
	void Unbind(FPSCamara* camera) override;
};