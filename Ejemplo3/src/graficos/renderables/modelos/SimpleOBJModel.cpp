#include "SimpleOBJModel.h"

#include "platform/Application.h"

SimpleOBJModel::SimpleOBJModel(const std::string& filepath, const wchar_t* textura, bool ccw, const mat4& transform, LightClass* lightSetup)
	: Renderable(transform, lightSetup) {
	m_ccw = ccw;

	m_shader = new LightShaderClass("shaders/SimpleObject.vs", "shaders/SimpleObject.ps");
	// Initialize the light shader object.
	bool result = m_shader->Initialize();
	if (!result)
	{
		MessageBox(Application::GetApplication().GetHWnd(), (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return;
	}

	m_loader = new objl::Loader();
	m_loader->LoadFile(filepath);

	cantVert = m_loader->LoadedVertices.size();
	cantIndices = m_loader->LoadedIndices.size();

	PushBuffers();

	m_textureID = LoadTexture(textura);
}

SimpleOBJModel::~SimpleOBJModel() {
	delete m_loader;
}

void SimpleOBJModel::PushBuffers() {
	// Allocate an OpenGL vertex array object.
	Application::GetApplication().GetOpenGL()->glGenVertexArrays(1, &m_vertexArrayId);

	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	Application::GetApplication().GetOpenGL()->glBindVertexArray(m_vertexArrayId);

	// Generate an ID for the vertex buffer.
	Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_vertexBufferId);

	// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	Application::GetApplication().GetOpenGL()->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(objl::Vertex), 
		&m_loader->LoadedVertices.front(), GL_STATIC_DRAW);

	// Generate an ID for the index buffer.
	Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_indexBufferId);

	// Bind the index buffer and load the index data into it.
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	Application::GetApplication().GetOpenGL()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, cantIndices * sizeof(unsigned int),
		&m_loader->LoadedIndices.front(), GL_STATIC_DRAW);
}

void SimpleOBJModel::BindBuffers() {
	m_shader->Bind();

	Application::GetApplication().GetOpenGL()->glBindVertexArray(m_vertexArrayId);

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

	// Enable the three vertex array attributes.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(0);  // Vertex position.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(1);  // Normals.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(2);  // Texture coordinates.

	// Specify the location and format of the position portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(objl::Vertex), 0);
	// Specify the location and format of the normal vector portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Normal));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, TextureCoordinate));

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
}

void SimpleOBJModel::UnbindBuffers() {
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(0);  // Vertex position.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(1);  // Normals.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(2);  // Texture coordinates.

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, 0);

	Application::GetApplication().GetOpenGL()->glBindVertexArray(0);

	m_shader->Unbind();
}

void SimpleOBJModel::Bind(FPSCamara* camera) {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	m_shader->Pon1Entero("diffuseTexture", 0);
}

void SimpleOBJModel::Unbind(FPSCamara* camera) {
	Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SimpleOBJModel::Render(FPSCamara* camera, const vec4& clipPlane) {
	BindBuffers();
	Bind(camera);

	m_shader->PonVec4("plane", clipPlane);
	m_shader->PonMatriz4x4("modelMatrix", m_transform);
	m_shader->PonMatriz4x4("viewMatrix", camera->GetViewMatrix());
	m_shader->PonMatriz4x4("projectionMatrix", camera->GetProjectionMatrix());
	m_shader->PonVec3("lightDirection", m_lightSetup->GetDirection());
	m_shader->PonVec4("diffuseLightColor", m_lightSetup->GetDiffuseColor());
	m_shader->PonVec3("lightPosition", m_lightSetup->GetPosition());

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace((m_ccw ? GL_CCW : GL_CW));

	glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, NULL);

	Unbind(camera);
	UnbindBuffers();
}