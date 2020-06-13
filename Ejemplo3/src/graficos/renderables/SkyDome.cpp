#include "SkyDome.h"
#include "platform/Application.h"

SkyDome::SkyDome(int stacks, int slices, float radio, const wchar_t nombre[], const mat4 &transform, LightClass *lightSetup)
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

	//cargamos la textura de la figura
	Carga(nombre);
	int an = Ancho();
	int alt = Alto();

	// Generate an ID for the texture.
	glGenTextures(1, &m_textureID);

	// Bind the texture as a 2D texture.
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set the texture filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Load the image data into the texture unit.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Ancho(), Alto(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, Dir_Imagen());

	// Generate mipmaps for the texture.
	Application::GetApplication().GetOpenGL()->glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	Descarga();
}

SkyDome::~SkyDome() {
	glDeleteTextures(1, &m_textureID);
}

void SkyDome::Bind() {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	m_shader->Pon1Entero("cielo", 0);
}

void SkyDome::Unbind() {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SkyDome::Render(FPSCamara* camera) {
	BindBuffers();
	Bind();

	m_shader->PonMatriz4x4("modelMatrix", m_transform);
	m_shader->PonMatriz4x4("viewMatrix", camera->m_ViewMatrix);
	m_shader->PonMatriz4x4("projectionMatrix", camera->m_ProjectionMatrix);
	m_shader->PonVec3("lightDirection", m_lightSetup->GetDirection());
	m_shader->PonVec4("diffuseLightColor", m_lightSetup->GetDiffuseColor());

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, NULL);

	glEnable(GL_DEPTH_TEST);

	Unbind();
	UnbindBuffers();
}