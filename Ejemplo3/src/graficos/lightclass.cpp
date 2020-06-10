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


void LightClass::SetDiffuseColor(const vec4 &color)
{
	m_diffuseColor = color;
}


void LightClass::SetDirection(const vec3 &dir)
{
	m_direction = dir;
}