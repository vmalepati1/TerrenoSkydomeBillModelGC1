#include "DebugTexture.h"

#include "platform/Application.h"

DebugTexture::DebugTexture(unsigned int textureID, const mat4& transform) 
	: Renderable(transform, NULL) {

	m_textureID = textureID;

	m_shader = new LightShaderClass("shaders/DebugTexture.vs", "shaders/DebugTexture.ps");
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

	m_data->maya[0].Posx = 1;
	m_data->maya[0].Posy = 0;

	m_data->maya[1].Posx = 1;
	m_data->maya[1].Posy = 1;

	m_data->maya[2].Posx = 0;
	m_data->maya[2].Posy = 1;

	m_data->maya[3].Posx = 0;
	m_data->maya[3].Posy = 0;

	m_data->indices = new unsigned int[6]{
		0, 1, 2,
		2, 3, 0
	};

	PushBuffers();
}

void DebugTexture::Bind() {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	m_shader->Pon1Entero("tex", 0);
}

void DebugTexture::Unbind() {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DebugTexture::Render(FPSCamara* camera) {
	BindBuffers();
	Bind();

	m_shader->PonMatriz4x4("transformation", m_transform);
	m_shader->PonMatriz4x4("projectionMatrix", mat4::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, NULL);

	Unbind();
	UnbindBuffers();
}