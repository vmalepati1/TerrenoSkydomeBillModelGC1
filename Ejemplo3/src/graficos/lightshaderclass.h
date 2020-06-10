////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <string>
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "matematicas/vec2.h"
#include "matematicas/vec3.h"
#include "matematicas/vec4.h"
#include "matematicas/mat4.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: LightShaderClass
////////////////////////////////////////////////////////////////////////////////
class LightShaderClass
{
public:
	LightShaderClass(const string &nombre1, const string &nombre2);
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize();
	void Shutdown();
	void Bind();	
	void Unbind();
	bool PonMatriz4x4(const string &nombre, const mat4 &matrix);
	bool Pon1Entero(const string& nombre, int);
	bool PonVec2(const string &nombre, const vec2 &vector);
	bool PonVec3(const string &nombre, const vec3 &vector);
	bool PonVec4(const string &nombre, const vec4 &vector);

private:
	bool InitializeShader(const char*, const char*);
	char* LoadShaderSourceFile(const char*);
	void OutputShaderErrorMessage(unsigned int, const char*);
	void OutputLinkerErrorMessage(unsigned int);
	void ShutdownShader();

private:
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_shaderProgram;
	string nombreVs;
	string nombrePS;
};

#endif