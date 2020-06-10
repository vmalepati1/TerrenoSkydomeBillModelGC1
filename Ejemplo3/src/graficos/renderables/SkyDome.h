#ifndef _Sky
#define _Sky
#include "Geometrias.h"
#include "graficos/Imagenes.h"
#include "platform/Application.h"

class SkyDome :public Primitivos
{
public:
	//En honor a nuestros ancestros llamaremos "Maya" a la malla
	//e suna estructura que contiene a los indices y vertices de la figura
	Maya cuadro;
	//variables locales de la clase para contener los stacks y los slices de la esfera
	int st, sl;
	//el nombre numerico de la textura en cuestion, por lo pronto una
	unsigned int esferaTextura;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	unsigned int m_textureID;
	//El constructor llama al metodo Esfera de la clase geometrias que generara los vertices
	//normales y uvs de la misma, nos regresa la estructura Maya.
	SkyDome(int stacks, int slices, float radio, const wchar_t nombre[])
	{
		this->hWnd = hWnd;
		cuadro = Esfera(stacks, slices, radio, 0.5, 1);
		st = stacks;
		sl = slices;
		//cargamos la textura de la figura
		Carga(nombre);
		int an = Ancho();
		int alt = Alto();
		// Set the unique texture unit in which to store the data.
		Application::GetApplication().GetOpenGL()->glActiveTexture(GL_TEXTURE0 + 2);

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
		

		// Allocate an OpenGL vertex array object.
		Application::GetApplication().GetOpenGL()->glGenVertexArrays(1, &m_vertexArrayId);

		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		Application::GetApplication().GetOpenGL()->glBindVertexArray(m_vertexArrayId);

		// Generate an ID for the vertex buffer.
		Application::GetApplication().GetOpenGL()->glGenBuffers(1, &m_vertexBufferId);

		// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
		Application::GetApplication().GetOpenGL()->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		Application::GetApplication().GetOpenGL()->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(Vertices), cuadro.maya, GL_STATIC_DRAW);

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
			cuadro.indices, GL_STATIC_DRAW);

		// Now that the buffers have been loaded we can release the array data.
		delete[] cuadro.maya;
		cuadro.maya = 0;

		delete[] cuadro.indices;
		cuadro.indices = 0;
	}

	~SkyDome()
	{
		//nos aseguramos de disponer de los recursos previamente reservados
		delete cuadro.maya;
		delete cuadro.indices;
		glDeleteTextures(1, &esferaTextura);
		Shutdown();
	}

	
	void Render()
	{
		//habilitamos el culling para reducir tiempo de procesamiento de las texturas
		//las texturas se analizan pixel a pixel para determinar que se imprimen o no
		//por lo que aunque no la veamos nos costo, por eso la eliminamos.
		
		glDisable(GL_DEPTH_TEST);
		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers();
		//glFrontFace(GL_CW);
		//glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
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
};
#endif