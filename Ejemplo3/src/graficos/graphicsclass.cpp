////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

#include "platform/Application.h"
#include "graficos/renderables/Water.h"

#include <iostream>

GraphicsClass::GraphicsClass()
{
	m_Camera = 0;
	/*
	m_LightShader = 0;
	m_LightShaderSky = 0;
	m_BillShader = 0;
	m_Light = 0;
	*/
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize()
{
	Application app = Application::GetApplication();

	// Create the camera object.
	m_Camera = new FPSCamara(mat4::Perspective(45.0f, (float)app.GetWindowWidth() / (float)app.GetWindowHeight(), 
		app.GetWindowNear(), app.GetWindowFar()));
	if(!m_Camera)
	{
		return false;
	}

	m_Camera->m_Position = vec3(0.0, 50, 40.0);
	m_Camera->Focus();

	m_Light = new LightClass();
	m_Light->SetDiffuseColor(vec4(1.0f));
	m_Light->SetDirection(vec3(-5.0f, 0.5f, -1.0f));

	const float TILE_SIZE = 25;

	m_Cubo = new Cubo(1.0, mat4::Translate(vec3(0, 45, -45)) * mat4::Scale(vec3(10, 10, 10)), m_Light);
	terreno = new Terreno(L"res/texturas/terreno.jpg", L"res/texturas/Zacatito.jpg", L"res/texturas/ZacatitoNorm.jpg",
		(float)400, (float)400, 0, 1, mat4::Translate(vec3(0, 20, 0)), m_Light);
	topDome = new SkyDome(32, 32, 256, L"res/texturas/earth.jpg", mat4::Translate(vec3(0, 35, 0)), m_Light);
	bottomDome = new SkyDome(32, 32, 256, L"res/texturas/earth.jpg", mat4::Translate(vec3(0, 35, 0)) , m_Light);
	waterAltura = 35;
	water = new Water(mat4::Translate(vec3(7, waterAltura, -3)) * mat4::Scale(vec3(TILE_SIZE, 0, TILE_SIZE)), m_Light);

	buffers = new WaterFrameBuffers();
	refractionTex = new DebugTexture(buffers->getRefractionTexture(), mat4::Translate(vec3(5, 5, 0)) * mat4::Scale(vec3(16.0/2.5, 9.0/2.5, 0)));
	reflectionTex = new DebugTexture(buffers->getReflectionTexture(), mat4::Translate(vec3(-10, 5, 0)) * mat4::Scale(vec3(16.0 / 2.5, 9.0 / 2.5, 0)));

	/*


	// Create the light shader object.
	m_LightShader = new LightShaderClass("shaders/light.vs", "shaders/light.ps");
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize();
	if(!result)
	{
		MessageBox(app.GetHWnd(), (LPCSTR)"Could not initialize the light shader object. 1", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(vec4(1.0f));
	m_Light->SetDirection(vec3(-5.0f, 0.5f, -1.0f));

	sky = new SkyDome(32, 32, 256, L"res/texturas/earth.jpg");
	// Create the light shader object.
	m_LightShaderSky = new LightShaderClass("shaders/Sky.vs", "shaders/Sky.ps");
	if (!m_LightShaderSky)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShaderSky->Initialize();
	if (!result)
	{
		MessageBox(app.GetHWnd(), (LPCSTR)"Could not initialize the light shader object. 2", (LPCSTR)"Error", MB_OK);
		return false;
	}

	bill = new Billboard(L"arbol.png", 10.0f, 25.0f, 110.0f);
	// Create the light shader object.
	m_BillShader = new LightShaderClass("shaders/Bill.vs", "shaders/Bill.ps");
	if (!m_BillShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_BillShader->Initialize();
	if (!result)
	{
		MessageBox(app.GetHWnd(), (LPCSTR)"Could not initialize the light shader object. 3", (LPCSTR)"Error", MB_OK);
		return false;
	}

	modelazo = new Modelos(L"arbol.png", 0.5, 0, 0);

	m_ModeloShader = new LightShaderClass("shaders/Modelo.vs", "shaders/Modelo.ps");
	if (!m_ModeloShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_ModeloShader->Initialize();
	if (!result)
	{
		MessageBox(app.GetHWnd(), (LPCSTR)"Could not initialize the light shader object. 4", (LPCSTR)"Error", MB_OK);
		return false;
	}
	*/

	return true;
}


void GraphicsClass::Shutdown()
{
	/*
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the light shader object.
	if (m_LightShaderSky)
	{
		m_LightShaderSky->Shutdown();
		delete m_LightShaderSky;
		m_LightShaderSky = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the terreno object.
	if (terreno)
	{
		delete terreno;
		m_Camera = 0;
	}
	*/
	return;
}

void GraphicsClass::RenderScene(const vec4& clippingPlane) {
	topDome->Render(m_Camera);
	terreno->RenderClipped(m_Camera, clippingPlane);
	m_Cubo->RenderClipped(m_Camera, clippingPlane);
}

void GraphicsClass::Render()
{
	glViewport(0, 0, Application::GetApplication().GetWindowWidth(), Application::GetApplication().GetWindowHeight());

	// Clear the buffers to begin the scene.
	Application::GetApplication().GetOpenGL()->BeginScene(0.976, 0.953, 1.0, 1.0f);

	m_Camera->Update();

	glEnable(GL_CLIP_DISTANCE0);

	buffers->bindReflectionFrameBuffer();
	float distance = 2 * (m_Camera->m_Position.y - waterAltura);
	m_Camera->m_Position.y -= distance;
	m_Camera->m_Pitch = -m_Camera->m_Pitch;
	m_Camera->UpdateViewMatrix();
	RenderScene(vec4(0, 1, 0, -waterAltura));
	m_Camera->m_Position.y += distance;
	m_Camera->m_Pitch = -m_Camera->m_Pitch;
	m_Camera->UpdateViewMatrix();

	buffers->bindRefractionFrameBuffer();
	RenderScene(vec4(0, -1, 0, waterAltura));

	buffers->unbindCurrentFrameBuffer();

	RenderScene(vec4(0, -1, 0, 10000));
	water->Render(m_Camera);

	refractionTex->Render(m_Camera);
	reflectionTex->Render(m_Camera);

	Application::GetApplication().GetOpenGL()->EndScene();

	/*
	glDisable(GL_DEPTH_TEST);
	m_LightShaderSky->SetShader(m_OpenGL);
	m_LightShaderSky->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", mat4::Identity());
	m_LightShaderSky->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", m_Camera->m_ViewMatrix);
	m_LightShaderSky->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", m_Camera->m_ProjectionMatrix);
	m_LightShaderSky->Pon1Entero(m_OpenGL, (char*)"cielo", 2);
	m_LightShaderSky->PonVec3(m_OpenGL, (char*)"lightDirection", m_Light->GetDirection().);
	m_LightShaderSky->PonVec4(m_OpenGL, (char*)"diffuseLightColor", diffuseLightColor);
	sky->Render(m_OpenGL);
	glEnable(GL_DEPTH_TEST);
	*/
	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//m_OpenGL->MatrixRotationY(worldMatrix, rotation);

	//// Set the light shader as the current shader program and set the matrices that it will use for rendering.
	/*
	m_LightShader->SetShader(m_OpenGL);	
	m_LightShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", mat4::Identity());
	m_LightShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", m_Camera->m_ViewMatrix);
	m_LightShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", m_Camera->m_ProjectionMatrix);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderTexture", 0);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderTexture2", 1);
	m_LightShader->PonVec3(m_OpenGL, (char*)"lightDirection", m_Light->GetDirection());
	m_LightShader->PonVec4(m_OpenGL, (char*)"diffuseLightColor", m_Light->GetDiffuseColor());
	// Render the model using the light shader.
	terreno->Render(m_OpenGL);
	*/

	/*
	//// Set the light shader as the current shader program and set the matrices that it will use for rendering.
	//
	m_OpenGL->MatrixTranslation(worldMatrixBill, bill->x, terreno->Superficie(bill->x, bill->z) - 1, bill->z);	
	float rotay[16];
	m_OpenGL->MatrixRotationY(rotay, -bill->angBill(m_Camera->m_Position.x, m_Camera->m_Position.z));
	m_OpenGL->MatrixMultiply(worldMatrixBill, rotay, worldMatrixBill);
	m_BillShader->SetShader(m_OpenGL);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", worldMatrixBill);
	m_BillShader->PonMatriz4x4T(m_OpenGL, (char*)"viewMatrix", m_Camera->m_ViewMatrix.elements);
	m_BillShader->PonMatriz4x4T(m_OpenGL, (char*)"projectionMatrix", m_Camera->m_ProjectionMatrix.elements);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billtext", 3);
	m_BillShader->PonVec3(m_OpenGL, (char*)"lightDirection", lightDirection);
	m_BillShader->PonVec4(m_OpenGL, (char*)"diffuseLightColor", diffuseLightColor);
	// Render the model using the light shader.
	bill->Render(m_OpenGL);
	
	// Set the light shader as the current shader program and set the matrices that it will use for rendering.
	m_ModeloShader->SetShader(m_OpenGL);
	float modmatrix[16];
	m_OpenGL->GetWorldMatrix(modmatrix);
	m_OpenGL->MatrixTranslation(modmatrix, modelazo->x, terreno->Superficie(modelazo->x, modelazo->z), modelazo->z);
	m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modmatrix);
	m_ModeloShader->PonMatriz4x4T(m_OpenGL, (char*)"viewMatrix", m_Camera->m_ViewMatrix.elements);
	m_ModeloShader->PonMatriz4x4T(m_OpenGL, (char*)"projectionMatrix", m_Camera->m_ProjectionMatrix.elements);
	m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"modtext", 4);
	m_ModeloShader->PonVec3(m_OpenGL, (char*)"lightDirection", lightDirection);
	m_ModeloShader->PonVec4(m_OpenGL, (char*)"diffuseLightColor", diffuseLightColor);
	// Render the model using the light shader.
	modelazo->Render(m_OpenGL);
	// Present the rendered scene to the screen.
	*/
}