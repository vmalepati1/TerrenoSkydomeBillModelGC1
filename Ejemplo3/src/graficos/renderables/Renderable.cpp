#include "Renderable.h"

#include "platform/Application.h"

Renderable::Renderable(const mat4& transform, LightClass* lightSetup) {
	m_transform = transform;
	m_lightSetup = lightSetup;
}

Renderable::~Renderable() {
	Application::GetApplication().GetOpenGL()->glDeleteBuffers(1, &m_vertexBufferId);
	Application::GetApplication().GetOpenGL()->glDeleteBuffers(1, &m_indexBufferId);
	Application::GetApplication().GetOpenGL()->glDeleteVertexArrays(1, &m_vertexArrayId);

	delete m_data->maya;
	delete m_data->indices;
	delete m_data;
	delete m_shader;
}

void Renderable::PushBuffers() {
	// Allocate an OpenGL vertex array object.
	Application::GetApplication().GetOpenGL()->glGenVertexArrays(1, &m_vertexArrayId);

	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	Application::GetApplication().GetOpenGL()->glBindVertexArray(m_vertexArrayId);

	// Generate an ID for the vertex buffer.
	Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_vertexBufferId);

	// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	Application::GetApplication().GetOpenGL()->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(Vertices), m_data->maya, GL_STATIC_DRAW);

	// Generate an ID for the index buffer.
	Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_indexBufferId);

	// Bind the index buffer and load the index data into it.
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	Application::GetApplication().GetOpenGL()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, cantIndices * sizeof(unsigned int),
		m_data->indices, GL_STATIC_DRAW);
}

void Renderable::BindBuffers() {
	m_shader->Bind();

	Application::GetApplication().GetOpenGL()->glBindVertexArray(m_vertexArrayId);

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

	// Enable the three vertex array attributes.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(0);  // Vertex position.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(1);  // Normals.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(2);  // Texture coordinates.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(3);  // Tangent.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(4);  // BiNormals.

	// Specify the location and format of the position portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertices), 0);
	// Specify the location and format of the normal vector portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertices), (void*)offsetof(Vertices, Normx));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertices), (void*)offsetof(Vertices, u));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertices), (void*)offsetof(Vertices, Tx));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertices), (void*)offsetof(Vertices, Bx));

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
}

void Renderable::UnbindBuffers() {
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(0);  // Vertex position.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(1);  // Normals.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(2);  // Texture coordinates.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(3);  // Tangent.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(4);  // BiNormals.

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, 0);

	Application::GetApplication().GetOpenGL()->glBindVertexArray(0);

	m_shader->Unbind();
}

void Renderable::Render(FPSCamara* camera) {
	BindBuffers();
	Bind();

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