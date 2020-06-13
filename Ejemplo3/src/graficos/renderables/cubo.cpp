#include "cubo.h"

#include "platform/Application.h"

#include <iostream>

Cubo::Cubo(float size, const mat4 &transform, LightClass *lightSetup)
	: Renderable(transform, lightSetup) {

	m_data = new Maya();
	m_shader = new LightShaderClass("shaders/cubo.vs", "shaders/cubo.ps");
	cantVert = 8;
	cantIndices = 36;

	// Initialize the light shader object.
	bool result = m_shader->Initialize();
	if (!result)
	{
		MessageBox(Application::GetApplication().GetHWnd(), (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return;
	}

	/* Empiece la generacion procesal */
	m_data->maya = new Vertices[8];

	memset(m_data->maya, 0, sizeof(m_data->maya));

	vec3 posiciones[8];
	vec3 normales[8];

	posiciones[0] = vec3(-size / 2.0f, -size / 2.0f, size / 2.0f);
	posiciones[1] = vec3(size / 2.0f, -size / 2.0f, size / 2.0f);
	posiciones[2] = vec3(size / 2.0f, size / 2.0f, size / 2.0f);
	posiciones[3] = vec3(-size / 2.0f, size / 2.0f, size / 2.0f);
	posiciones[4] = vec3(-size / 2.0f, -size / 2.0f, -size / 2.0f);
	posiciones[5] = vec3(size / 2.0f, -size / 2.0f, -size / 2.0f);
	posiciones[6] = vec3(size / 2.0f, size / 2.0f, -size / 2.0f);
	posiciones[7] = vec3(-size / 2.0f, size / 2.0f, -size / 2.0f);

	normales[0] = vec3(-1.0f, -1.0f, 1.0f);
	normales[1] = vec3(1.0f, -1.0f, 1.0f);
	normales[2] = vec3(1.0f, 1.0f, 1.0f);
	normales[3] = vec3(-1.0f, 1.0f, 1.0f);
	normales[4] = vec3(-1.0f, -1.0f, -1.0f);
	normales[5] = vec3(1.0f, -1.0f, -1.0f);
	normales[6] = vec3(1.0f, 1.0f, -1.0f);
	normales[7] = vec3(-1.0f, 1.0f, -1.0f);

	for (int i = 0; i < 8; i++) {
		m_data->maya[i].Posx = posiciones[i].x;
		m_data->maya[i].Posy = posiciones[i].y;
		m_data->maya[i].Posz = posiciones[i].z;

		m_data->maya[i].Normx = normales[i].x;
		m_data->maya[i].Normy = normales[i].y;
		m_data->maya[i].Normz = normales[i].z;
	}

	m_data->indices = new unsigned int[36]
	{
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	PushBuffers();
}

void Cubo::RenderClipped(FPSCamara* camera, const vec4& clipPlane) {
	BindBuffers();
	Bind();

	m_shader->PonVec4("plane", clipPlane);
	m_shader->PonMatriz4x4("modelMatrix", m_transform);
	m_shader->PonMatriz4x4("viewMatrix", camera->m_ViewMatrix);
	m_shader->PonMatriz4x4("projectionMatrix", camera->m_ProjectionMatrix);
	m_shader->PonVec3("lightDirection", m_lightSetup->GetDirection());
	m_shader->PonVec4("diffuseLightColor", m_lightSetup->GetDiffuseColor());

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, NULL);

	Unbind();
	UnbindBuffers();
}