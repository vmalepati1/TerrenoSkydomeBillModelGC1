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
/*
#include "renderables/Terreno.h"
#include "renderables/SkyDome.h"
#include "renderables/Billboard.h"
#include "renderables/Modelos.h"
*/
#include "camera/FPSCamara.h"
#include "graficos/renderables/cubo.h"


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
	void Render();
private:
	FPSCamara* m_Camera;	
	/*
	Terreno* terreno;
	SkyDome* sky;
	Billboard* bill;
	Modelos* modelazo;
	LightShaderClass* m_LightShader;
	LightShaderClass* m_LightShaderSky;
	LightShaderClass* m_BillShader;
	LightShaderClass* m_ModeloShader;
	LightClass* m_Light;
	*/
	Cubo* m_Cubo;
};

#endif