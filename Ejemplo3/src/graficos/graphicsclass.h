////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "api/openglclass.h"
#include "camera/cameraclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
/*
#include "renderables/Billboard.h"
#include "renderables/Modelos.h"
*/
#include "renderables/SkyDome.h"
#include "camera/FPSCamara.h"
#include "graficos/renderables/cubo.h"
#include "graficos/renderables/Terreno.h"
#include "graficos/renderables/Water.h"
#include "graficos/renderables/WaterFrameBuffers.h"
#include "graficos/renderables/DebugTexture.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize();
	void Shutdown();
	void RenderScene(const vec4& clippingPlane);
	void Render();
private:
	FPSCamara* m_Camera;	
	/*
	
	SkyDome* sky;
	Billboard* bill;
	Modelos* modelazo;
	LightShaderClass* m_LightShader;
	LightShaderClass* m_LightShaderSky;
	LightShaderClass* m_BillShader;
	LightShaderClass* m_ModeloShader;
	*/
	float waterAltura;
	LightClass* m_Light;
	Terreno* terreno;
	SkyDome* topDome;
	SkyDome* bottomDome;
	Water* water;
	Cubo* m_Cubo;
	WaterFrameBuffers* buffers;
	DebugTexture* refractionTex;
	DebugTexture* reflectionTex;
};