#pragma once

#include "OBJLoader.h"
#include "graficos/renderables/Geometrias.h"
#include "api/openglclass.h"
#include <gl/glu.h>
#include "graficos/renderables/Renderable.h"

class OBJMesh : public Renderable {
private:
	unsigned int m_kaID, m_kdID, m_ksID, m_normalID;
	float m_usingKaMap, m_usingKdMap, m_usingKsMap, m_usingNormalMap;
	bool m_ccw;
	float m_shininess;
	vec3 m_ka;
	vec3 m_kd;
	vec3 m_ks;

	const std::vector<objl::Vertex> *m_vertices;
	const std::vector<unsigned int> *m_indices;

public:
	OBJMesh(const objl::Mesh& mesh, bool ccw, const mat4& transform, LightClass* lightSetup);

	void Render(FPSCamara* camera, const vec4& clipPlane) override;

private:
	void PushBuffers() override;
	void BindBuffers() override;
	void UnbindBuffers() override;
	void Bind(FPSCamara* camera) override;
	void Unbind(FPSCamara* camera) override;

	std::wstring GetWString(const std::string& str);
};