#include "Water.h"
#include "platform/Application.h"

Water::Water(const mat4& transform, LightClass* lightSetup) 
	: Renderable(transform, lightSetup) {

	m_shader = new LightShaderClass("shaders/water.vs", "shaders/water.ps");
	// Initialize the light shader object.
	bool result = m_shader->Initialize();
	if (!result)
	{
		MessageBox(Application::GetApplication().GetHWnd(), (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return;
	}

	m_data = new Maya();

	cantVert = 4;
	cantIndices = 6;

	m_data->maya = new Vertices[4];

	memset(m_data->maya, 0, sizeof(m_data->maya));

	m_data->maya[0].Posx = -1;
	m_data->maya[0].Posz = -1;

	m_data->maya[1].Posx = -1;
	m_data->maya[1].Posz = 1;

	m_data->maya[2].Posx = 1;
	m_data->maya[2].Posz = -1;

	m_data->maya[3].Posx = 1;
	m_data->maya[3].Posz = 1;

	m_data->indices = new unsigned int[6] {
		0, 1, 2,
		2, 1, 3
	};

	PushBuffers();
}

Water::~Water() {

}