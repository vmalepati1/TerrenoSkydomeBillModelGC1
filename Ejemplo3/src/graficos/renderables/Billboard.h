#pragma once

#include "graficos/Imagenes.h"
#include "matematicas/vec3.h"
#include "Geometrias.h"
#include "platform/Application.h"

class Billboard : public Primitivos
{

public:	
	unsigned int billbTextura;
	vec3 Up, derecha;
	float x, y, z, escala;
	vec3 BillCam;
	//En honor a nuestros ancestros llamaremos "Maya" a la malla
	//e suna estructura que contiene a los indices y vertices de la figura
	Maya billbo;
	//variables locales de la clase para contener los stacks y los slices de la esfera
	int st, sl;
	//el nombre numerico de la textura en cuestion, por lo pronto una
	unsigned int billboTextura;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	unsigned int m_textureID;

	Billboard(const wchar_t textura[], float escala, float x, float z)
	{
		this->escala = escala;		
		this->x = x;
		this->y = y;
		this->z = z;

		Up = vec3::Up();

		//este vector es el que nos dara el ancho del billboard
		//cada vez que lo giremos segun la camara

		Carga(textura);
		// Set the unique texture unit in which to store the data.
		Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0 + 3);

		// Generate an ID for the texture.
		glGenTextures(1, &m_textureID);

		// Bind the texture as a 2D texture.
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		// Load the image data into the texture unit.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Ancho(), Alto(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, Dir_Imagen());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set the texture filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Generate mipmaps for the texture.
		Application::GetApplication().GetOpenGL()->glGenerateMipmap(GL_TEXTURE_2D);

		Descarga();

		billbo = BillBoardBasico(escala);


		// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		Application::GetApplication().GetOpenGL()->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(Vertices), billbo.maya, GL_STATIC_DRAW);

		// Enable the three vertex array attributes.
		Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(0);  // Vertex position.
		Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(1);  // Normals.
		Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(2);  // Texture coordinates.
		Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(3);  // Tangent.
		Application::GetApplication().GetOpenGL()->glEnableVertexAttribArray(4);  // BiNormals.

		// Specify the location and format of the position portion of the vertex buffer.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		Application::GetApplication().GetOpenGL()->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertices), 0);

		// Specify the location and format of the normal vector portion of the vertex buffer.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		Application::GetApplication().GetOpenGL()->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (3 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		Application::GetApplication().GetOpenGL()->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (6 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		Application::GetApplication().GetOpenGL()->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (8 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		Application::GetApplication().GetOpenGL()->glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (11 * sizeof(float)));

		// Generate an ID for the index buffer.
		Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_indexBufferId);

		// Bind the index buffer and load the index data into it.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		Application::GetApplication().GetOpenGL()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, cantIndices * sizeof(unsigned int),
			billbo.indices, GL_STATIC_DRAW);


	}

	~Billboard()
	{
		// Now that the buffers have been loaded we can release the array data.
		delete[] billbo.maya;
		billbo.maya = 0;

		delete[] billbo.indices;
		billbo.indices = 0;
		glDeleteTextures(1, &billbTextura);
		Shutdown();
	}

	void Render()
	{
		//habilitamos el culling para reducir tiempo de procesamiento de las texturas
		//las texturas se analizan pixel a pixel para determinar que se imprimen o no
		//por lo que aunque no la veamos nos costo, por eso la eliminamos.	
		RenderBuffers();
		return;
	}

	void RenderBuffers()
	{
		// Bind the vertex array object that stored all the information about the vertex and index buffers.
		Application::GetApplication().GetOpenGL()->glBindVertexArray(m_vertexArrayId);

		// Render the vertex buffer using the index buffer.
		glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, 0);

		return;
	}

	void Shutdown()
	{
		// Release the vertex and index buffers.
		ShutdownBuffers();

		return;
	}

	void ShutdownBuffers()
	{
		// Disable the two vertex array attributes.
		Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(0);
		Application::GetApplication().GetOpenGL()->glDisableVertexAttribArray(1);

		// Release the vertex buffer.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, 0);
		Application::GetApplication().GetOpenGL()->glDeleteBuffers(1, &m_vertexBufferId);

		// Release the index buffer.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		Application::GetApplication().GetOpenGL()->glDeleteBuffers(1, &m_indexBufferId);

		// Release the vertex array object.
		Application::GetApplication().GetOpenGL()->glBindVertexArray(0);
		Application::GetApplication().GetOpenGL()->glDeleteVertexArrays(1, &m_vertexArrayId);

		return;
	}

	float angBill(float xc, float zc)
	{
		float vecx = xc - x;
		float vecz = zc - z;

		float ang = acos(vecz / (sqrt(vecx * vecx + vecz * vecz)));

		if (vecx >= 0)
			ang += 3.1415;

		return ang;
	}

};