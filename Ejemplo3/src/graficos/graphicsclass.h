////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "api/openglclass.h"
#include "camera/cameraclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "renderables/Terreno.h"
#include "renderables/SkyDome.h"
#include "renderables/Billboard.h"
#include "renderables/Modelos.h"
#include "camera/FPSCamara.h"
#include "graficos/renderables/cubo.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(OpenGLClass*, HWND);
	void Shutdown();
	bool Frame();
	float angx, angy, angz, vel;

private:
	bool Render(float);

private:
	OpenGLClass* m_OpenGL;
	FPSCamara* m_Camera;	
	Terreno* terreno;
	SkyDome* sky;
	Billboard* bill;
	Modelos* modelazo;
	LightShaderClass* m_LightShader;
	LightShaderClass* m_LightShaderSky;
	LightShaderClass* m_BillShader;
	LightShaderClass* m_ModeloShader;
	LightClass* m_Light;
	Cubo* m_Cubo;
};

#endif