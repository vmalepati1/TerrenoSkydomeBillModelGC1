#include "OBJMesh.h"

#include "platform/Application.h"

OBJMesh::OBJMesh(const objl::Mesh& mesh, bool ccw, const mat4& transform, LightClass* lightSetup) 
	: Renderable(transform, lightSetup) {
	m_ccw = ccw;
	m_shader = new LightShaderClass("shaders/Modelo.vs", "shaders/Modelo.ps");
	// Initialize the light shader object.
	bool result = m_shader->Initialize();
	if (!result)
	{
		MessageBox(Application::GetApplication().GetHWnd(), (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return;
	}

	m_vertices = &mesh.Vertices;
	m_indices = &mesh.Indices;

	cantVert = (*m_vertices).size();
	cantIndices = (*m_indices).size();

	PushBuffers();

	m_usingKaMap = 0.0f;
	m_usingKdMap = 0.0f;
	m_usingKsMap = 0.0f;
	m_usingNormalMap = 0.0f;

	std::wstring map_KaPath = GetWString(mesh.MeshMaterial.map_Ka);
	std::wstring map_KdPath = GetWString(mesh.MeshMaterial.map_Kd);
	std::wstring map_KsPath = GetWString(mesh.MeshMaterial.map_Ks);
	std::wstring normalMapPath = GetWString(mesh.MeshMaterial.map_bump);

	if (!map_KaPath.empty()) {
		m_usingKaMap = 1.0f;
		m_kaID = LoadTexture(map_KaPath.c_str());
	}

	if (!map_KdPath.empty()) {
		m_usingKdMap = 1.0f;
		m_kdID = LoadTexture(map_KdPath.c_str());
	}

	if (!map_KsPath.empty()) {
		m_usingKsMap = 1.0f;
		m_ksID = LoadTexture(map_KsPath.c_str());
	}

	if (!normalMapPath.empty()) {
		m_usingNormalMap = 1.0f;
		m_normalID = LoadTexture(normalMapPath.c_str());
	}

	m_shininess = mesh.MeshMaterial.Ns;
	m_ka = vec3(mesh.MeshMaterial.Ka.X, mesh.MeshMaterial.Ka.Y, mesh.MeshMaterial.Ka.Z);
	m_kd = vec3(mesh.MeshMaterial.Kd.X, mesh.MeshMaterial.Kd.Y, mesh.MeshMaterial.Kd.Z);
	m_ks = vec3(mesh.MeshMaterial.Ks.X, mesh.MeshMaterial.Ks.Y, mesh.MeshMaterial.Ks.Z);
}

std::wstring OBJMesh::GetWString(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}

void OBJMesh::PushBuffers() {
	// Allocate an OpenGL vertex array object.
	Application::GetApplication().GetOpenGL()->glGenVertexArrays(1, &m_vertexArrayId);

	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	Application::GetApplication().GetOpenGL()->glBindVertexArray(m_vertexArrayId);

	// Generate an ID for the vertex buffer.
	Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_vertexBufferId);

	// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	Application::GetApplication().GetOpenGL()->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(objl::Vertex),
		&(*m_vertices).front(), GL_STATIC_DRAW);

	// Generate an ID for the index buffer.
	Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_indexBufferId);

	// Bind the index buffer and load the index data into it.
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	Application::GetApplication().GetOpenGL()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, cantIndices * sizeof(unsigned int),
		&(*m_indices).front(), GL_STATIC_DRAW);
}

void OBJMesh::BindBuffers() {
	m_shader->Bind();

	Application::GetApplication().GetOpenGL()->glBindVertexArray(m_vertexArrayId);

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

	// Enable the three vertex array attributes.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(0);  // Vertex position.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(1);  // Normals.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(2);  // Texture coordinates.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(3);  // Tangents.
	Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(4);  // Binormals.

	// Specify the location and format of the position portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(objl::Vertex), 0);
	// Specify the location and format of the normal vector portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Normal));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, TextureCoordinate));
	// Specify the location and format of the tangent portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Tangent));
	// Specify the location and format of the binormal portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Binormal));

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
}

void OBJMesh::UnbindBuffers() {
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

void OBJMesh::Bind(FPSCamara* camera) {
	if (m_usingKaMap) {
		Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_kaID);
	}

	if (m_usingKdMap) {
		Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_kdID);
	}

	if (m_usingKsMap) {
		Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_ksID);
	}

	if (m_usingNormalMap) {
		Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_normalID);
	}

	m_shader->Pon1Entero("u_mapKa", 0);
	m_shader->Pon1Entero("u_mapKd", 1);
	m_shader->Pon1Entero("u_mapKs", 2);
	m_shader->Pon1Entero("u_normalMap", 3);

	m_shader->Pon1Flota("u_usingKaMap", m_usingKaMap);
	m_shader->Pon1Flota("u_usingKdMap", m_usingKdMap);
	m_shader->Pon1Flota("u_usingKsMap", m_usingKsMap);
	m_shader->Pon1Flota("u_usingNormalMap", m_usingNormalMap);

	m_shader->Pon1Flota("u_shininess", m_shininess);

	m_shader->PonVec3("u_ka", m_ka);
	m_shader->PonVec3("u_kd", m_kd);
	m_shader->PonVec3("u_ks", m_ks);
}

void OBJMesh::Unbind(FPSCamara* camera) {

}

void OBJMesh::Render(FPSCamara* camera, const vec4& clipPlane) {
	BindBuffers();
	Bind(camera);

	m_shader->PonVec4("plane", clipPlane);
	m_shader->PonMatriz4x4("modelMatrix", m_transform);
	m_shader->PonMatriz4x4("viewMatrix", camera->GetViewMatrix());
	m_shader->PonMatriz4x4("projectionMatrix", camera->GetProjectionMatrix());
	m_shader->PonVec3("lightDirection", m_lightSetup->GetDirection());
	m_shader->PonVec4("ambientLightColor", m_lightSetup->GetAmbientColor());
	m_shader->PonVec4("diffuseLightColor", m_lightSetup->GetDiffuseColor());
	m_shader->PonVec4("specularLightColor", m_lightSetup->GetSpecularColor());
	m_shader->PonVec3("lightPosition", m_lightSetup->GetPosition());

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace((m_ccw ? GL_CCW : GL_CW));

	glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, NULL);

	Unbind(camera);
	UnbindBuffers();
}