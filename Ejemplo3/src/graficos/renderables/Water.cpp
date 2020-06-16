#include "Water.h"
#include "platform/Application.h"

Water::Water(const wchar_t* dudvMap, const wchar_t* normalMap, const mat4& transform, LightClass* lightSetup, WaterFrameBuffers* fbos)
	: Renderable(transform, lightSetup) {
	m_fbos = fbos;

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

	m_dudvTextureID = LoadTexture(dudvMap);
	m_normalMapID = LoadTexture(normalMap);

	waveSpeed = 0.06;
	moveFactor = 0;
}

Water::~Water() {

}

void Water::Bind(FPSCamara* camera) {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbos->getReflectionTexture());

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fbos->getRefractionTexture());

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_dudvTextureID);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_normalMapID);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_fbos->getRefractionDepthTexture());

	m_shader->Pon1Entero("reflectionTexture", 0);
	m_shader->Pon1Entero("refractionTexture", 1);
	m_shader->Pon1Entero("dudvMap", 2);
	m_shader->Pon1Entero("normalMap", 3);
	m_shader->Pon1Entero("depthMap", 4);

	moveFactor += waveSpeed * (Application::GetApplication().GetFrametime() / 1000.0);
	moveFactor = fmod(moveFactor, 1);
	m_shader->Pon1Flota("moveFactor", moveFactor);

	m_shader->PonVec3("cameraPosition", camera->GetPosition());
	m_shader->Pon1Flota("near", Application::GetApplication().GetWindowNear());
	m_shader->Pon1Flota("far", Application::GetApplication().GetWindowFar());
}

void Water::Unbind(FPSCamara* camera) {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);
}