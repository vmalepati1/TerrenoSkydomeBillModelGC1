////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

#include <iostream>

extern int screenWidth;
extern int screenHeight;

GraphicsClass::GraphicsClass()
{
	m_OpenGL = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_LightShaderSky = 0;
	m_BillShader = 0;
	m_Light = 0;
	angx = angy = angz = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(OpenGLClass* OpenGL, HWND hwnd)
{
	bool result;


	// Store a pointer to the OpenGL class object.
	m_OpenGL = OpenGL;

	// Create the camera object.
	m_Camera = new FPSCamara(mat4::Perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f));
	if(!m_Camera)
	{
		return false;
	}

	m_Camera->m_Position = vec3(0.0, 0.0, -10.0);
	m_Camera->Focus();

	m_Cubo = new Cubo(m_OpenGL, hwnd, 1.0);

	terreno = new Terreno(hwnd, m_OpenGL, L"res/texturas/terreno.jpg", L"res/texturas/Zacatito.jpg", L"res/texturas/ZacatitoNorm.jpg",
		(float)400, (float)400, 0, 1);

	// Create the light shader object.
	m_LightShader = new LightShaderClass("shaders/light.vs", "shaders/light.ps");
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_OpenGL, hwnd);
	if(!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object. 1", (LPCSTR)"Error", MB_OK);
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

	sky = new SkyDome(hwnd, m_OpenGL, 32, 32, 256, L"res/texturas/earth.jpg");
	// Create the light shader object.
	m_LightShaderSky = new LightShaderClass("shaders/Sky.vs", "shaders/Sky.ps");
	if (!m_LightShaderSky)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShaderSky->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object. 2", (LPCSTR)"Error", MB_OK);
		return false;
	}

	bill = new Billboard(hwnd, m_OpenGL, L"arbol.png", 10.0f, 25.0f, 110.0f);
	// Create the light shader object.
	m_BillShader = new LightShaderClass("shaders/Bill.vs", "shaders/Bill.ps");
	if (!m_BillShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_BillShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object. 3", (LPCSTR)"Error", MB_OK);
		return false;
	}

	modelazo = new Modelos(hwnd, m_OpenGL, L"arbol.png", 0.5, 0, 0);

	m_ModeloShader = new LightShaderClass("shaders/Modelo.vs", "shaders/Modelo.ps");
	if (!m_ModeloShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_ModeloShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object. 4", (LPCSTR)"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown(m_OpenGL);
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the light shader object.
	if (m_LightShaderSky)
	{
		m_LightShaderSky->Shutdown(m_OpenGL);
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

	// Release the pointer to the OpenGL class object.
	m_OpenGL = 0;

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += 0.0174532925f * 2.0f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	float worldMatrixBill[16];

	glViewport(0, 0, screenWidth, screenHeight);

	// Clear the buffers to begin the scene.
	m_OpenGL->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	m_Camera->Update();

	m_Cubo->Render(m_Camera);

	// terreno->Superficie(0, 110);

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

	m_OpenGL->EndScene();

	return true;
}