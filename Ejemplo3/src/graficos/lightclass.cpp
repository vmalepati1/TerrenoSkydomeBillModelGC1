////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(const vec4& color) {
	m_ambientColor = color;
}

void LightClass::SetDiffuseColor(const vec4 &color)
{
	m_diffuseColor = color;
}

void LightClass::SetSpecularColor(const vec4& color) {
	m_specularColor = color;
}

void LightClass::SetDirection(const vec3 &dir)
{
	m_direction = dir;
}

void LightClass::SetPosition(const vec3& pos) {
	m_position = pos;
}