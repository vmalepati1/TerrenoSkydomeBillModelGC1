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

	m_Camera->SetPosition(vec3(0, 42.4008, -4.95687));
	m_Camera->Focus();

	m_Light = new LightClass();
	m_Light->SetAmbientColor(vec4(0.7f, 0.7f, 0.7f, 1.0f));
	m_Light->SetDiffuseColor(vec4(0.7f, 0.7f, 0.7f, 1.0f));
	m_Light->SetSpecularColor(vec4(0.7f, 0.7f, 0.7f, 1.0f));
	m_Light->SetDirection(vec3(-5.0f, 0.5f, -1.0f));
	m_Light->SetPosition(vec3(5.0f, 100.0f, -200.0f));

	const float TILE_SIZE = 100;

	m_Cubo = new Cubo(1.0, mat4::Translate(vec3(0, 45, -45)) * mat4::Scale(vec3(10, 10, 10)), m_Light);
	terreno = new Terreno(L"res/texturas/terreno.jpg", L"res/texturas/Zacatito.jpg", L"res/texturas/ZacatitoNorm.jpg",
		(float)400, (float)400, 0, 1, mat4::Translate(vec3(0, 20, 0)), m_Light);
	dome = new SkyDome(32, 32, 256, L"res/texturas/SkyDay.jpg", L"res/texturas/SkyNight.png", mat4::Translate(vec3(0, 25, 0)), m_Light);
	waterAltura = 35;
	buffers = new WaterFrameBuffers();
	water = new Water(L"res/texturas/waterDUDV.png", L"res/texturas/waterNormal.png", mat4::Translate(vec3(7, waterAltura, -3)) * mat4::Scale(vec3(TILE_SIZE, 0, TILE_SIZE)), m_Light, buffers);

	refractionTex = new DebugTexture(buffers->getRefractionTexture(), mat4::Translate(vec3(5, 5, 0)) * mat4::Scale(vec3(16.0/2.5, 9.0/2.5, 0)));
	reflectionTex = new DebugTexture(buffers->getReflectionTexture(), mat4::Translate(vec3(-10, 5, 0)) * mat4::Scale(vec3(16.0 / 2.5, 9.0 / 2.5, 0)));

	// town = new OBJModel("res/modelos/modelo.obj");
	airship = new SimpleOBJModel("res/modelos/barco.obj", L"res/texturas/ship_diffuse.PNG", true, mat4::Translate(vec3(0, 35, 0)) * mat4::Rotate(180.0, vec3(0, 1, 0)) * mat4::Scale(vec3(5, 5, 5)), m_Light);
	balloon = new SimpleOBJModel("res/modelos/airship.obj", L"res/texturas/airship.PNG", false, mat4::Translate(vec3(0, 65, -100)) * mat4::Scale(vec3(0.1, 0.1, 0.1)), m_Light);
	// test = new OBJModel("res/modelos/barco.obj", true, mat4::Translate(vec3(0, 50, 0)) * mat4::Scale(vec3(5, 5, 5)), m_Light);
	b1 = new Billboard(L"res/texturas/bricks-png-2.png", mat4::Translate(vec3(0, 45, -70)) * mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * mat4::Scale(vec3(19, 0, 7.54)), m_Light);
	b2 = new Billboard(L"res/texturas/580b585b2edbce24c47b2ba0.png", mat4::Translate(vec3(30, 45, -70)) * mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * mat4::Scale(vec3(15.0, 0, 14.15)), m_Light);
	b3 = new Billboard(L"res/texturas/heavy-duty-no-maintenance-recycled-plastic-garden-furniture-collection-gunby-picnic-bench.png", mat4::Translate(vec3(65, 45, -70)) * mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * mat4::Scale(vec3(18.0, 0, 11.2)), m_Light);
	b4 = new Billboard(L"res/texturas/park-bench-clipart-2.png", mat4::Translate(vec3(-30, 45, -70)) * mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * mat4::Scale(vec3(12.5, 0, 7.0)), m_Light);
	b5 = new Billboard(L"res/texturas/street_light_PNG11512.png", mat4::Translate(vec3(-60, 45, -70)) * mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * mat4::Scale(vec3(10.24, 0, 12.8)), m_Light);
	// b3 = new Billboard(L"res/texturas/580b585b2edbce24c47b2ba0.png", mat4::Translate(vec3(50, 45, -70)) * mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * mat4::Scale(vec3(15.0, 0, 14.15)), m_Light);
	// = new Billboard(L"res/texturas/bricks-png-2.png", mat4::Translate(vec3(0, 45, -70)) * mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * mat4::Scale(vec3(19, 0, 7.54)), m_Light);
	// = new Billboard(L"res/texturas/bricks-png-2.png", mat4::Translate(vec3(0, 45, -70)) * mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * mat4::Scale(vec3(19, 0, 7.54)), m_Light);
	// = new Billboard(L"res/texturas/bricks-png-2.png", mat4::Translate(vec3(0, 45, -70)) * mat4::Rotate(90.0f, vec3(1.0f, 0.0f, 0.0f)) * mat4::Scale(vec3(19, 0, 7.54)), m_Light);
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
	dome->Render(m_Camera, clippingPlane);
	terreno->Render(m_Camera, clippingPlane);
	// m_Cubo->Render(m_Camera, clippingPlane);
	balloon->Render(m_Camera, clippingPlane);
	airship->Render(m_Camera, clippingPlane);
	b5->Render(m_Camera, clippingPlane);
	b4->Render(m_Camera, clippingPlane);
	b3->Render(m_Camera, clippingPlane);
	b2->Render(m_Camera, clippingPlane);
	b1->Render(m_Camera, clippingPlane);
	
	// test->Render(m_Camera, clippingPlane);
}

void GraphicsClass::Render()
{
	glViewport(0, 0, Application::GetApplication().GetWindowWidth(), Application::GetApplication().GetWindowHeight());

	// Clear the buffers to begin the scene.
	Application::GetApplication().GetOpenGL()->BeginScene(0.976, 0.953, 1.0, 1.0f);

	m_Camera->Update();

	// airship->SetTransform(mat4::Translate(vec3(0, 35, 0)) * mat4::Rotate(180.0, vec3(0, 1, 0)) * mat4::Rotate(-m_Camera->GetYaw() * 180.0 / M_PI * 0.5, vec3(0, 1, 0)) * mat4::Scale(vec3(5, 5, 5)));

	airship->SetTransform(mat4::Translate(vec3(m_Camera->GetPosition().x, 35, m_Camera->GetPosition().z + 4.95687)) * mat4::Rotate(180.0, vec3(0, 1, 0)) * mat4::Scale(vec3(5, 5, 5)));

	glEnable(GL_CLIP_DISTANCE0);

	buffers->bindReflectionFrameBuffer();
	float distance = 2 * (m_Camera->GetPosition().y - waterAltura);
	m_Camera->Translate(0, -distance, 0);
	m_Camera->InvertPitch();
	m_Camera->UpdateViewMatrix();
	RenderScene(vec4(0, 1, 0, -waterAltura+1.0f));
	m_Camera->Translate(0, distance, 0);
	m_Camera->InvertPitch();
	m_Camera->UpdateViewMatrix();

	buffers->bindRefractionFrameBuffer();
	RenderScene(vec4(0, -1, 0, waterAltura));

	buffers->unbindCurrentFrameBuffer();

	RenderScene(vec4(0, -1, 0, 10000));
	water->Render(m_Camera, vec4(0, -1, 0, 10000));

	// refractionTex->Render(m_Camera);
	// reflectionTex->Render(m_Camera);

	Application::GetApplication().GetOpenGL()->EndScene();
}