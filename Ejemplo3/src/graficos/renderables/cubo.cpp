#include "cubo.h"

#include <iostream>

Cubo::Cubo(OpenGLClass *glInstance, HWND hwnd, float size) {
	this->glInstance = glInstance;

	vertData = new Maya();
	shader = new LightShaderClass("shaders/cubo.vs", "shaders/cubo.ps");

	// Initialize the light shader object.
	bool result = shader->Initialize(glInstance, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return;
	}

	/* Empiece la generacion procesal */
	vertData->maya = new Vertices[8];

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
			0, 1, 2, 2, 3, 0, // top
			6, 2, 3, 3, 7, 6, // back 
			7, 4, 5, 5, 6, 7, // bottom
			4, 0, 3, 3, 7, 4, // left
			0, 4, 5, 5, 1, 0, // front 
			1, 5, 6, 6, 2, 1  // right
	};

	// Generate an ID for the vertex buffer.
	glInstance->glGenBuffers(1, &m_vertexBufferId);

	// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
	glInstance->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glInstance->glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertices), vertData->maya, GL_STATIC_DRAW);

	// Generate an ID for the index buffer.
	glInstance->glGenBuffers(1, &m_indexBufferId);

	// Bind the index buffer and load the index data into it.
	glInstance->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glInstance->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int),
		vertData->indices, GL_STATIC_DRAW);
}

Cubo::~Cubo() {
	delete vertData->maya;
	delete vertData->indices;
	delete vertData;
	delete shader;
}

void Cubo::Bind() {
	shader->Bind(glInstance);

	glInstance->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

	// Enable the three vertex array attributes.
	glInstance->glEnableVertexAttribArray(0);  // Vertex position.
	glInstance->glEnableVertexAttribArray(1);  // Normals.
	glInstance->glEnableVertexAttribArray(2);  // Texture coordinates.
	glInstance->glEnableVertexAttribArray(3);  // Tangent.
	glInstance->glEnableVertexAttribArray(4);  // BiNormals.

	// Specify the location and format of the position portion of the vertex buffer.
	glInstance->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertices), 0);
	// Specify the location and format of the normal vector portion of the vertex buffer.
	glInstance->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (3 * sizeof(float)));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	glInstance->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (6 * sizeof(float)));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	glInstance->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (8 * sizeof(float)));
	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	glInstance->glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (11 * sizeof(float)));

	glInstance->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
}

void Cubo::Unbind() {
	glInstance->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glInstance->glDisableVertexAttribArray(0);  // Vertex position.
	glInstance->glDisableVertexAttribArray(1);  // Normals.
	glInstance->glDisableVertexAttribArray(2);  // Texture coordinates.
	glInstance->glDisableVertexAttribArray(3);  // Tangent.
	glInstance->glDisableVertexAttribArray(4);  // BiNormals.

	glInstance->glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader->Unbind(glInstance);
}

void Cubo::Render(FPSCamara *camera) {
	Bind();

	shader->PonMatriz4x4(glInstance, "modelMatrix", mat4::Identity());
	shader->PonMatriz4x4(glInstance, "viewMatrix", camera->m_ViewMatrix);
	shader->PonMatriz4x4(glInstance, "projectionMatrix", camera->m_ProjectionMatrix);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	Unbind();
}