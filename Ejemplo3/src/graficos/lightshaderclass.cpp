////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightshaderclass.h"

#include <iostream>


LightShaderClass::LightShaderClass(const string &vsFilename, const string &fsFilename)
	: nombreVs(vsFilename), nombrePS(fsFilename)
{

}


LightShaderClass::LightShaderClass(const LightShaderClass& other)
{
}


LightShaderClass::~LightShaderClass()
{
}


bool LightShaderClass::Initialize(OpenGLClass* OpenGL, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(nombreVs.c_str(), nombrePS.c_str(), OpenGL, hwnd);
	if(!result)
	{
		return false;
	}

	return true;
}


void LightShaderClass::Shutdown(OpenGLClass* OpenGL)
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader(OpenGL);

	return;
}


void LightShaderClass::Bind(OpenGLClass* OpenGL)
{
	// Install the shader program as part of the current rendering state.
	OpenGL->glUseProgram(m_shaderProgram);
	
	return;
}

void LightShaderClass::Unbind(OpenGLClass* OpenGL)
{
	// Install the shader program as part of the current rendering state.
	OpenGL->glUseProgram(0);

	return;
}

bool LightShaderClass::InitializeShader(const char* vsFilename, const char* fsFilename, OpenGLClass* OpenGL, HWND hwnd)
{
	const char* vertexShaderBuffer;
	const char* fragmentShaderBuffer;
	int status;

	// Load the vertex shader source file into a text buffer.
	vertexShaderBuffer = LoadShaderSourceFile(vsFilename);
	if(!vertexShaderBuffer)
	{
		return false;
	}

	// Load the fragment shader source file into a text buffer.
	fragmentShaderBuffer = LoadShaderSourceFile(fsFilename);
	if(!fragmentShaderBuffer)
	{
		return false;
	}

	// Create a vertex and fragment shader object.
	m_vertexShader = OpenGL->glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = OpenGL->glCreateShader(GL_FRAGMENT_SHADER);

	// Copy the shader source code strings into the vertex and fragment shader objects.
	OpenGL->glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
	OpenGL->glShaderSource(m_fragmentShader, 1, &fragmentShaderBuffer, NULL);

	// Release the vertex and fragment shader buffers.
	delete [] vertexShaderBuffer;
	vertexShaderBuffer = 0;
	
	delete [] fragmentShaderBuffer;
	fragmentShaderBuffer = 0;

	// Compile the shaders.
	OpenGL->glCompileShader(m_vertexShader);
	OpenGL->glCompileShader(m_fragmentShader);

	// Check to see if the vertex shader compiled successfully.
	OpenGL->glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
	if(status != 1)
	{
		// If it did not compile then write the syntax error message out to a text file for review.
		OutputShaderErrorMessage(OpenGL, hwnd, m_vertexShader, vsFilename);
		return false;
	}

	// Check to see if the fragment shader compiled successfully.
	OpenGL->glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
	if(status != 1)
	{
		// If it did not compile then write the syntax error message out to a text file for review.
		OutputShaderErrorMessage(OpenGL, hwnd, m_fragmentShader, fsFilename);
		return false;
	}

	// Create a shader program object.
	m_shaderProgram = OpenGL->glCreateProgram();

	// Attach the vertex and fragment shader to the program object.
	OpenGL->glAttachShader(m_shaderProgram, m_vertexShader);
	OpenGL->glAttachShader(m_shaderProgram, m_fragmentShader);

	// Bind the shader input variables.
	OpenGL->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
	OpenGL->glBindAttribLocation(m_shaderProgram, 1, "inputNormal");
	OpenGL->glBindAttribLocation(m_shaderProgram, 2, "inputTexCoord");
	OpenGL->glBindAttribLocation(m_shaderProgram, 3, "inputTangent");
	OpenGL->glBindAttribLocation(m_shaderProgram, 4, "inputBinormal");
	

	// Link the shader program.
	OpenGL->glLinkProgram(m_shaderProgram);

	// Check the status of the link.
	OpenGL->glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
	if(status != 1)
	{
		// If it did not link then write the syntax error message out to a text file for review.
		OutputLinkerErrorMessage(OpenGL, hwnd, m_shaderProgram);
		return false;
	}

	return true;
}


char* LightShaderClass::LoadShaderSourceFile(const char* filename)
{
	ifstream fin;
	int fileSize;
	char input;
	char* buffer;


	// Open the shader source file.
	fin.open(filename);

	// If it could not open the file then exit.
	if(fin.fail())
	{
		return 0;
	}

	// Initialize the size of the file.
	fileSize = 0;

	// Read the first element of the file.
	fin.get(input);

	// Count the number of elements in the text file.
	while(!fin.eof())
	{
		fileSize++;
		fin.get(input);
	}

	// Close the file for now.
	fin.close();

	// Initialize the buffer to read the shader source file into.
	buffer = new char[fileSize+1];
	if(!buffer)
	{
		return 0;
	}

	// Open the shader source file again.
	fin.open(filename);

	// Read the shader text file into the buffer as a block.
	fin.read(buffer, fileSize);

	// Close the file.
	fin.close();

	// Null terminate the buffer.
	buffer[fileSize] = '\0';

	return buffer;
}


void LightShaderClass::OutputShaderErrorMessage(OpenGLClass* OpenGL, HWND hwnd, unsigned int shaderId, const char* shaderFilename)
{
	int logSize, i;
	char* infoLog;
	ofstream fout;
	wchar_t newString[128];
	unsigned int error;
	size_t convertedChars;


	// Get the size of the string containing the information log for the failed shader compilation message.
	OpenGL->glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

	// Increment the size by one to handle also the null terminator.
	logSize++;

	// Create a char buffer to hold the info log.
	infoLog = new char[logSize];
	if(!infoLog)
	{
		return;
	}

	// Now retrieve the info log.
	OpenGL->glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<logSize; i++)
	{
		fout << infoLog[i];
	}

	// Close the file.
	fout.close();

	// Convert the shader filename to a wide character string.
	error = mbstowcs_s(&convertedChars, newString, 128, shaderFilename, 128);
	if(error != 0)
	{
		return;
	}

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, (LPCSTR)"Error compiling shader.  Check shader-error.txt for message.", 
		(LPCSTR)newString, MB_OK);

	return;
}


void LightShaderClass::OutputLinkerErrorMessage(OpenGLClass* OpenGL, HWND hwnd, unsigned int programId)
{
	int logSize, i;
	char* infoLog;
	ofstream fout;


	// Get the size of the string containing the information log for the failed shader compilation message.
	OpenGL->glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

	// Increment the size by one to handle also the null terminator.
	logSize++;

	// Create a char buffer to hold the info log.
	infoLog = new char[logSize];
	if(!infoLog)
	{
		return;
	}

	// Now retrieve the info log.
	OpenGL->glGetProgramInfoLog(programId, logSize, NULL, infoLog);

	// Open a file to write the error message to.
	fout.open("linker-error.txt");

	// Write out the error message.
	for(i=0; i<logSize; i++)
	{
		fout << infoLog[i];
	}

	// Close the file.
	fout.close();

	// Pop a message up on the screen to notify the user to check the text file for linker errors.
	MessageBox(hwnd, (LPCSTR)"Error compiling linker.  Check linker-error.txt for message.", 
		(LPCSTR)"Linker Error", MB_OK);

	return;
}


void LightShaderClass::ShutdownShader(OpenGLClass* OpenGL)
{
	// Detach the vertex and fragment shaders from the program.
	OpenGL->glDetachShader(m_shaderProgram, m_vertexShader);
	OpenGL->glDetachShader(m_shaderProgram, m_fragmentShader);

	// Delete the vertex and fragment shaders.
	OpenGL->glDeleteShader(m_vertexShader);
	OpenGL->glDeleteShader(m_fragmentShader);

	// Delete the shader program.
	OpenGL->glDeleteProgram(m_shaderProgram);

	return;
}

bool LightShaderClass::PonMatriz4x4(OpenGLClass* OpenGL, const string& nombre, const mat4 &matrix) {
	unsigned int location;


	// Set the world matrix in the vertex shader.
	location = OpenGL->glGetUniformLocation(m_shaderProgram, nombre.c_str());
	if (location == -1)
	{
		return false;
	}
	OpenGL->glUniformMatrix4fv(location, 1, GL_TRUE, matrix.elements);
	return true;
}

bool LightShaderClass::Pon1Entero(OpenGLClass* OpenGL, const string& nombre, int valor)
{
	unsigned int location;

	// Set the texture in the pixel shader to use the data from the first texture unit.
	location = OpenGL->glGetUniformLocation(m_shaderProgram, nombre.c_str());
	if (location == -1)
	{
		return false;
	}
	OpenGL->glUniform1i(location, valor);
	return true;
}

bool LightShaderClass::PonVec2(OpenGLClass* OpenGL, const string& nombre, const vec2& vector) {
	unsigned int location;

	// Set the texture in the pixel shader to use the data from the first texture unit.
	location = OpenGL->glGetUniformLocation(m_shaderProgram, nombre.c_str());
	if (location == -1)
	{
		return false;
	}
	OpenGL->glUniform2f(location, vector.x, vector.y);
	return true;
}

bool LightShaderClass::PonVec3(OpenGLClass* OpenGL, const string& nombre, const vec3 &vector)
{
	unsigned int location;

	// Set the texture in the pixel shader to use the data from the first texture unit.
	location = OpenGL->glGetUniformLocation(m_shaderProgram, nombre.c_str());
	if (location == -1)
	{
		return false;
	}
	OpenGL->glUniform3f(location, vector.x, vector.y, vector.z);
	return true;
}

bool LightShaderClass::PonVec4(OpenGLClass* OpenGL, const string& nombre, const vec4& vector)
{
	unsigned int location;

	// Set the texture in the pixel shader to use the data from the first texture unit.
	location = OpenGL->glGetUniformLocation(m_shaderProgram, nombre.c_str());
	if (location == -1)
	{
		return false;
	}
	OpenGL->glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	return true;
}