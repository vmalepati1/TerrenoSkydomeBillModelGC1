#include "SkyDome.h"
#include "platform/Application.h"

SkyDome::SkyDome(int stacks, int slices, float radio, const wchar_t dayTexture[], const wchar_t nightTexture[], const mat4& transform, LightClass* lightSetup)
	: Renderable(transform, lightSetup) {

	m_shader = new LightShaderClass("shaders/Sky.vs", "shaders/Sky.ps");
	// Initialize the light shader object.
	bool result = m_shader->Initialize();
	if (!result)
	{
		MessageBox(Application::GetApplication().GetHWnd(), (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return;
	}

	m_data = new Maya();
	*m_data = Esfera(stacks, slices, radio, 0.5, 1);
	st = stacks;
	sl = slices;

	PushBuffers();
	
	m_dayTextureID = LoadTexture(dayTexture);
	m_nightTextureID = LoadTexture(nightTexture);

	time = 0;
}

SkyDome::~SkyDome() {
	glDeleteTextures(1, &m_dayTextureID);
	glDeleteTextures(1, &m_nightTextureID);
}

void SkyDome::Bind(FPSCamara* camera) {
	time += Application::GetApplication().GetFrametime();
	time = fmod(time, 24000);
	int texture1 = m_dayTextureID;
	int texture2 = m_dayTextureID;
	float blendFactor = 0.0;

	if (time >= 0 && time < 5000) {
		texture1 = m_nightTextureID;
		texture2 = m_nightTextureID;
		blendFactor = (time - 0) / (5000 - 0);
	}
	else if (time >= 5000 && time < 8000) {
		texture1 = m_nightTextureID;
		texture2 = m_dayTextureID;
		blendFactor = (time - 5000) / (8000 - 5000);
	}
	else if (time >= 8000 && time < 21000) {
		texture1 = m_dayTextureID;
		texture2 = m_dayTextureID;
		blendFactor = (time - 8000) / (21000 - 8000);
	}
	else {
		texture1 = m_dayTextureID;
		texture2 = m_nightTextureID;
		blendFactor = (time - 21000) / (24000 - 21000);
	}

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	m_shader->Pon1Entero("texture1", 0);
	m_shader->Pon1Entero("texture2", 1);
	m_shader->Pon1Flota("blendFactor", blendFactor);
}

void SkyDome::Unbind(FPSCamara* camera) {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SkyDome::Render(FPSCamara* camera, const vec4& clipPlane) {
	BindBuffers();
	Bind(camera);

	m_shader->PonMatriz4x4("modelMatrix", m_transform);
	m_shader->PonMatriz4x4("viewMatrix", camera->GetViewMatrix());
	m_shader->PonMatriz4x4("projectionMatrix", camera->GetProjectionMatrix());
	m_shader->PonVec3("lightDirection", m_lightSetup->GetDirection());
	m_shader->PonVec4("diffuseLightColor", m_lightSetup->GetDiffuseColor());
	m_shader->PonVec3("lightPosition", m_lightSetup->GetPosition());

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, NULL);

	glEnable(GL_DEPTH_TEST);

	Unbind(camera);
	UnbindBuffers();
}