////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include "matematicas/vec3.h"
#include "matematicas/vec4.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(const vec4& color);
	void SetDiffuseColor(const vec4 &color);
	void SetSpecularColor(const vec4& color);
	void SetDirection(const vec3 &dir);
	void SetPosition(const vec3& pos);

	inline const vec4& GetAmbientColor() const { return m_ambientColor; }
	inline const vec4& GetDiffuseColor() const { return m_diffuseColor; }
	inline const vec4& GetSpecularColor() const { return m_specularColor; }
	inline const vec3& GetDirection() const { return m_direction; }
	inline const vec3& GetPosition() const { return m_position; }

private:
	vec4 m_ambientColor;
	vec4 m_diffuseColor;
	vec4 m_specularColor;
	vec3 m_direction;
	vec3 m_position;
};

#endif