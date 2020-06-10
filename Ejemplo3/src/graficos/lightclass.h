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

	void SetDiffuseColor(const vec4 &color);
	void SetDirection(const vec3 &dir);

	inline const vec4& GetDiffuseColor() const { return m_diffuseColor; }
	inline const vec3& GetDirection() const { return m_direction; }

private:
	vec4 m_diffuseColor;
	vec3 m_direction;
};

#endif