#include "cubo.h"

#include "platform/Application.h"

#include <iostream>

Cubo::Cubo(float size) {
	vertData = new Maya();
	shader = new LightShaderClass("shaders/cubo.vs", "shaders/cubo.ps");

	// Initialize the light shader object.
	bool result = shader->Initialize();
	if (!result)
	{
		MessageBox(Application::GetApplication().GetHWnd(), (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return;
	}

	/* Empiece la generacion procesal */
	vertData->maya = new Vertices[8];

	memset(vertData->maya, 0, sizeof(Vertices) * 8);

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
		vertData->maya[i].Posx = posiciones[i].x;
		vertData->maya[i].Posy = posiciones[i].y;
		vertData->maya[i].Posz = posiciones[i].z;

		vertData->maya[i].Normx = normales[i].x;
		vertData->maya[i].Normy = normales[i].y;
		vertData->maya[i].Normz = normales[i].z;
	}

	vertData->indices = new unsigned int[36]
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

	// Allocate an OpenGL vertex array object.
	Application::GetApplication().GetOpenGL()->glGenVertexArrays(1, &m_vertexArrayId);

	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	Application::GetApplication().GetOpenGL()->glBindVertexArray(m_vertexArrayId);

	// Generate an ID for the vertex buffer.
	Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_vertexBufferId);

	// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	Application::GetApplication().GetOpenGL()->glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertices), vertData->maya, GL_STATIC_DRAW);

	// Generate an ID for the index buffer.
	Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_indexBufferId);

	// Bind the index buffer and load the index data into it.
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	Application::GetApplication().GetOpenGL()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int),
		vertData->indices, GL_STATIC_DRAW);
}

Cubo::~Cubo() {
	Application::GetApplication().GetOpenGL()->glDeleteBuffers(1, &m_vertexBufferId);
	Application::GetApplication().GetOpenGL()->glDeleteBuffers(1, &m_indexBufferId);
	Application::GetApplication().GetOpenGL()->glDeleteVertexArrays(1, &m_vertexArrayId);

	delete vertData->maya;
	delete vertData->indices;
	delete vertData;
	delete shader;
}

void Cubo::Bind() {
	shader->Bind();

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
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (3 * sizeof(float)));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (6 * sizeof(float)));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (8 * sizeof(float)));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	Application::GetApplication().GetOpenGL()->glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (11 * sizeof(float)));

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
}

void Cubo::Unbind() {
	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(0);  // Vertex position.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(1);  // Normals.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(2);  // Texture coordinates.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(3);  // Tangent.
	Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(4);  // BiNormals.

	Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, 0);

	Application::GetApplication().GetOpenGL()->glBindVertexArray(0);

	shader->Unbind();
}

void Cubo::Render(FPSCamara *camera) {
	Bind();

	shader->PonMatriz4x4("modelMatrix", mat4::Identity());
	shader->PonMatriz4x4("viewMatrix", camera->m_ViewMatrix);
	shader->PonMatriz4x4("projectionMatrix", camera->m_ProjectionMatrix);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	Unbind();
}